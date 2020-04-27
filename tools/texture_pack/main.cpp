//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <ios>
#include <gear/fbs/generated/texture_atlas_generated.h>
#include <tinyxml2.h>
#include <stb_rect_pack.h>
#include <stb_image.h>
#include <stb_image_write.h>


namespace fs = std::filesystem;
namespace xml = tinyxml2;

xml::XMLElement* getCollisionElement(xml::XMLElement* xTile) {
    xml::XMLElement* xObjectGroup = xTile->FirstChildElement("objectgroup");
    for(xml::XMLElement* xObject = xObjectGroup->FirstChildElement("object"); xObject;
    xObject = xObject->NextSiblingElement("object")) {
        if (xObject->Attribute("name", "collision")) {
            return xObject;
        }
    }
    return nullptr;
}


stbrp_rect getRectById(const std::vector<stbrp_rect>& rects, int id) {
    return *std::find_if(rects.begin(), rects.end(), [id](const stbrp_rect& r){return r.id == id;});
}

int main(int argc, char* argv[]) {

    std::string inputPath;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPath, "input file")
            ("input files").cardinality(1, std::numeric_limits<size_t>::max())
            | lyra::opt(outFileName, "output file name").required(1)
            ["-o"]["--output"]
            ("output file name")
            | lyra::opt(printInputs)
            ["-p"]["--printInputs"]
            | lyra::help(showHelp);

    auto result = cli.parse({argc, argv});
    if (!result){
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }

    if (showHelp)
    {
        std::cout << cli << std::endl;
        return 0;
    }

    auto outTexName = (outFileName + ".png");
    auto outAtlasName = (outFileName + ".bin");


    {
        xml::XMLDocument doc;
        doc.LoadFile(inputPath.c_str());
        auto xTileSet = doc.FirstChildElement("tileset");

        std::vector<stbrp_rect> rects;
        int pageWidth = 0, pageHeight = 0;

        //Pack rectangles
        {
            for (auto xTile = xTileSet->FirstChildElement("tile"); xTile;
                 xTile = xTile->NextSiblingElement("tile")) {
                auto xImage = xTile->FirstChildElement("image");
                unsigned short width = xImage->IntAttribute("width");
                unsigned short height = xImage->IntAttribute("height");
                int id = xTile->IntAttribute("id");
                rects.push_back({id, width, height});
            }

            stbrp_context ctx;
            std::vector<stbrp_node> nodes;
            bool packed = false;

            for(int size = 256; !packed && size <= 4096; size*=2) {
                pageWidth = size;
                pageHeight = size;
                nodes.resize(pageWidth);
                stbrp_init_target(&ctx, pageWidth, pageHeight, nodes.data(), pageWidth);
                packed = stbrp_pack_rects(&ctx, rects.data(), rects.size());
            }

            if (!packed) {
                std::cerr << "could not pack sprites\n";
                return 1;
            }
        }

        // Create texture
        {
            std::unique_ptr<uint32_t[]> textureData = std::make_unique<uint32_t[]>(pageWidth * pageHeight);

            auto reldir = fs::path(inputPath).parent_path();
            for (auto xTile = xTileSet->FirstChildElement("tile"); xTile;
                 xTile = xTile->NextSiblingElement("tile")) {
                int id = xTile->IntAttribute("id");
                auto rect = getRectById(rects, id);
                auto xImage = xTile->FirstChildElement("image");
                auto source = xImage->Attribute("source");
                int w, h, c;
                stbi_set_flip_vertically_on_load(1);
                auto imageData = (uint32_t*) stbi_load((reldir / source).c_str(), &w, &h, &c, 4);

                for(int x = 0; x < rect.w; x++) {
                    for(int y = 0; y < rect.h; y++) {
                        int px = rect.x + x;
                        int py = rect.y + y;

                        textureData[px + py * pageWidth] = imageData[x + y * rect.w];
                    }
                }

                stbi_image_free(imageData);
            }

            stbi_flip_vertically_on_write(1);
            stbi_write_png(outTexName.c_str(), pageWidth, pageHeight, 4, textureData.get(), pageWidth * sizeof(uint32_t));
        }

        // Write binary output
        {
            flatbuffers::FlatBufferBuilder builder(2048);
            std::vector<flatbuffers::Offset<gear::bin::Sprite>> sprites;

            for (auto xTile = xTileSet->FirstChildElement("tile"); xTile;
                 xTile = xTile->NextSiblingElement("tile")) {
                int id = xTile->IntAttribute("id");

                std::vector<gear::bin::UVs> uvs;
                std::vector<flatbuffers::Offset<gear::bin::Object>> objects;

                auto xImage = xTile->FirstChildElement("image");
                auto source = xImage->Attribute("source");
                auto xAnimation = xTile->FirstChildElement("animation");
                auto xObjects = xTile->FirstChildElement("objectgroup");

                auto addFrame = [&](int frameId) {
                    auto rect = getRectById(rects, frameId);
                    uvs.emplace_back( (float) rect.x / (float) pageWidth, (float) rect.y / (float) pageHeight,
                                      (float) (rect.x + rect.w) / (float) pageWidth, (float) (rect.y + rect.h) / (float) pageHeight );
                };

                if (xAnimation) {
                    for(auto xFrame = xAnimation->FirstChildElement("frame"); xFrame;
                    xFrame = xFrame->NextSiblingElement("frame")) {
                        auto frameId = xFrame->IntAttribute("tileid");
                        addFrame(frameId);
                    }
                } else {
                   addFrame(id);
                }

                if (xObjects) {
                    for(auto xObject = xObjects->FirstChildElement("object"); xObject;
                    xObject = xObject->NextSiblingElement("object")) {
                        auto name = builder.CreateString(xObject->Attribute("name"));
                        auto ob = gear::bin::ObjectBuilder(builder);
                        ob.add_name(name);
                        ob.add_x(xObject->FloatAttribute("x"));
                        ob.add_y(xObject->FloatAttribute("y"));
                        if (xObject->FirstChildElement("ellipse")) {
                            ob.add_w(xObject->FloatAttribute("width"));
                            ob.add_shape(gear::bin::Shape::Shape_Circle);
                        } else if (xObject->FirstChildElement("point")) {
                            ob.add_shape(gear::bin::Shape_Point);
                        } else { // rectangle
                            ob.add_w(xObject->FloatAttribute("width"));
                            ob.add_h(xObject->FloatAttribute("height"));
                            ob.add_shape(gear::bin::Shape_Rectangle);
                        }

                        objects.push_back(ob.Finish());
                    }
                }


                std::string name = fs::path(source).stem();
                gear::bin::fvec2 size {
                        (float)xImage->IntAttribute("width"),
                        (float)xImage->IntAttribute("height")
                };


                auto sprite = gear::bin::CreateSpriteDirect(builder, name.c_str(), &size, &uvs, &objects);
                sprites.push_back(sprite);
            }

            auto texPathRel = fs::relative(outTexName, fs::path(outAtlasName).parent_path());
            auto atlas = gear::bin::CreateAtlasDirect(builder, texPathRel.c_str(), &sprites);
            builder.Finish(atlas);


            auto buf = builder.GetBufferPointer();
            auto bufSize = builder.GetSize();
            std::ofstream ofs(outAtlasName , std::ios::out | std::ios::binary);
            ofs.write((char*)buf, bufSize);
        }

    }

    return 0;
}