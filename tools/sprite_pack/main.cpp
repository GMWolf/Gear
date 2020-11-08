//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <ios>
#include <gear/assets_generated.h>
#include <tinyxml2.h>
#include <stb_rect_pack.h>
#include <stb_image.h>
#include <flatbuffers/hash.h>
#include "texture.h"

namespace fs = std::filesystem;
namespace xml = tinyxml2;

stbrp_rect getRectById(const std::vector<stbrp_rect>& rects, int id) {
    return *std::find_if(rects.begin(), rects.end(), [id](const stbrp_rect& r){return r.id == id;});
}


std::pair<gear::assets::Shape, flatbuffers::Offset<void>>
parseShape(flatbuffers::FlatBufferBuilder& builder, const xml::XMLElement* element, gear::assets::fvec2 origin, gear::assets::fvec2 spriteSize)
{
    gear::assets::Shape shape_type;
    flatbuffers::Offset<void> shape;
    if (element->FirstChildElement("ellipse")) {
        shape_type = gear::assets::Shape::circle;
        float r = element->FloatAttribute("width") / 2.f;
        float xLeft = element->FloatAttribute("x");
        float yBottom = spriteSize.y() - element->FloatAttribute("y") - 2 * r;
        shape = builder.CreateStruct(gear::assets::Circle{
                (xLeft + r) - origin.x(),
                 (yBottom + r) - origin.y(),
                r
        }).Union();
    } else if (element->FirstChildElement("point")) {
        shape_type = gear::assets::Shape::point;
        shape = builder.CreateStruct(gear::assets::Point{
                element->FloatAttribute("x") - origin.x(),
                element->FloatAttribute("y") - origin.y()
        }).Union();
    } else { // rectangle
        shape_type = gear::assets::Shape::rectangle;
        float w = element->FloatAttribute("width");
        float h = element->FloatAttribute("height");
        float x0 = element->FloatAttribute("x");
        float y0 = spriteSize.y() - element->FloatAttribute("y") - h;
        shape = builder.CreateStruct(gear::assets::Rectangle {
            x0 - origin.x(),
            y0 - origin.y(),
            w,
            h
        }).Union();
    }

    return {shape_type, shape};
}

gear::assets::fvec2 shapeOrigin(const xml::XMLElement* element, gear::assets::fvec2 spriteSize)
{
    if (element->FirstChildElement("ellipse")) {
        float r = element->FloatAttribute("width") / 2.f;
        return {element->FloatAttribute("x") + r, spriteSize.y() - (element->FloatAttribute("y") + r)};
    } else if (element->FirstChildElement("point")) {
       return { element->FloatAttribute("x"), spriteSize.y() - element->FloatAttribute("y")};
    } else { // rectangle
        return {
                element->FloatAttribute("x") + element->FloatAttribute("width") / 2,
                spriteSize.y() - (element->FloatAttribute("y") + element->FloatAttribute("height") / 2)
        };
    }
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
        std::vector<uint32_t> textureData(pageWidth * pageHeight, 0);
        {
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
        }

        // Write binary output
        {
            flatbuffers::FlatBufferBuilder builder(2048);
            std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
            std::vector<flatbuffers::Offset<gear::assets::Ref>> references;

            auto tilesetName = xTileSet->Attribute("name");
            auto texPathRel = fs::relative(outTexName, fs::path(outAtlasName).parent_path());
            auto texName = std::string(tilesetName) + "_texture";
            auto texNameHash = flatbuffers::HashFnv1<uint64_t>(texName.c_str());
            auto texoffset = gear::buildTexture(builder, pageWidth, pageHeight, gear::assets::PixelFormat::RGBA8,
                                                reinterpret_cast<const uint8_t *>(textureData.data()), 4 * pageWidth * pageHeight);
            auto texRef = gear::assets::CreateRef(builder, (uint8_t)gear::assets::Asset::Texture, texNameHash);
            references.push_back(texRef);
            entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(texName.c_str()), gear::assets::Asset::Texture, texoffset.Union()));

            for (auto xTile = xTileSet->FirstChildElement("tile"); xTile;
                 xTile = xTile->NextSiblingElement("tile")) {
                int id = xTile->IntAttribute("id");

                if (xTile->Attribute("type", "excluded")) {
                    continue;
                }

                std::vector<gear::assets::UVs> uvs;
                std::vector<flatbuffers::Offset<gear::assets::Object>> objects;

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

                gear::assets::fvec2 origin = {0,0};
                bool explicitOrigin = false;

                gear::assets::Shape collisionShapeType = gear::assets::Shape::NONE;
                flatbuffers::Offset<void> collisionShape;

                gear::assets::fvec2 size {
                        (float)xImage->IntAttribute("width"),
                        (float)xImage->IntAttribute("height")
                };

                if (xObjects) {

                    //Find the origin
                    for(auto xObject = xObjects->FirstChildElement("object"); xObject;
                        xObject = xObject->NextSiblingElement("object")) {
                        const char *nameStr = xObject->Attribute("name");
                        if (strcmp(nameStr, "origin") == 0 ) {
                            if (xObject->FirstChildElement("point")) {
                                origin = {xObject->FloatAttribute("x"),
                                          xObject->FloatAttribute("y")
                                };
                                explicitOrigin = true;
                            }
                        }
                    }

                    //Rest of objects
                    for(auto xObject = xObjects->FirstChildElement("object"); xObject;
                    xObject = xObject->NextSiblingElement("object")) {
                        const char *nameStr = xObject->Attribute("name");
                        if (strcmp(nameStr, "origin") == 0 ) {
                            //Do nothing!
                        } else if (strcmp(nameStr, "collision") == 0) {
                            if (!explicitOrigin) {
                                origin = shapeOrigin(xObject, size);
                            }
                            auto[shape_type, shape] = parseShape(builder, xObject, origin, size);
                            collisionShapeType = shape_type;
                            collisionShape = shape;
                        } else {
                                auto name = builder.CreateString(nameStr);
                                auto[shape_type, shape] = parseShape(builder, xObject, origin, size);
                                objects.push_back(gear::assets::CreateObject(builder, name, shape_type, shape));
                            }
                        }
                    }

                std::string name = fs::path(source).stem();

                auto objectsOffset = builder.CreateVector(objects);
                auto uvsOffset = builder.CreateVectorOfStructs(uvs);

                auto sprite = gear::assets::CreateSprite(builder, texRef, &size, uvsOffset, &origin, collisionShapeType, collisionShape, objectsOffset);
                entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(name.c_str()), gear::assets::Asset::Sprite, sprite.Union()));
            }

            auto assetVec = builder.CreateVectorOfSortedTables(&entries);
            auto refVec = builder.CreateVector(references);
            auto bundle = gear::assets::CreateBundle(builder, assetVec, 0, refVec);
            gear::assets::FinishBundleBuffer(builder, bundle);
            builder.Finish(bundle);

            auto buf = builder.GetBufferPointer();
            auto bufSize = builder.GetSize();
            std::ofstream ofs(outAtlasName , std::ios::out | std::ios::binary);
            ofs.write((char*)buf, bufSize);
        }

    }

    return 0;
}