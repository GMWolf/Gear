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

            std::ofstream ofs(outAtlasName+".raw" , std::ios::out | std::ios::binary);
            ofs.write((char*)textureData.get(), pageWidth * pageHeight * 4);


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

                if (xTile->Attribute("type", "excluded")) {
                    continue;
                }

                std::vector<gear::bin::UVs> uvs;
                std::vector<flatbuffers::Offset<gear::bin::Object>> objects;

                auto xImage = xTile->FirstChildElement("image");
                auto source = xImage->Attribute("source");
                auto xAnimation = xTile->FirstChildElement("animation");
                auto xObjects = xTile->FirstChildElement("objectgroup");

                auto addFrame = [&](int frameId) {
                    auto rect = getRectById(rects, frameId);
                    uvs.emplace_back(rect.x, rect.y, (rect.x + rect.w),(rect.y + rect.h));
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
                        gear::bin::Shape shape_type;
                        flatbuffers::Offset<void> shape;
                        if (xObject->FirstChildElement("ellipse")) {
                            shape_type = gear::bin::Shape_circle;
                            shape =builder.CreateStruct(gear::bin::Circle {
                                xObject->FloatAttribute("x"),
                                xObject->FloatAttribute("y"),
                                xObject->FloatAttribute("width") / 2.f
                            }).Union();
                        } else if (xObject->FirstChildElement("point")) {
                            shape_type = gear::bin::Shape_point;
                            shape = builder.CreateStruct(gear::bin::Point {
                                    xObject->FloatAttribute("x"),
                                    xObject->FloatAttribute("y")
                            }).Union();
                        } else { // rectangle
                            shape_type = gear::bin::Shape_rectangle;
                            shape = builder.CreateStruct(gear::bin::Rectangle {
                                    xObject->FloatAttribute("x"),
                                    xObject->FloatAttribute("y"),
                                    xObject->FloatAttribute("width"),
                                    xObject->FloatAttribute("height")
                            }).Union();
                        }
                        objects.push_back(gear::bin::CreateObject(builder, name, shape_type, shape));
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