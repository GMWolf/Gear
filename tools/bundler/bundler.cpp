//
// Created by felix on 07/06/2020.
//
#include <iostream>
#include <fstream>
#include<gear/fbs/generated/assets_generated.h>

int main(int argc, char* argv[]) {

    flatbuffers::FlatBufferBuilder builder(2048);
    {
        std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;

        for(int i = 0; i < argc - 2; i++) {
            char* fileName = argv[i + 2];
            std::ifstream in(fileName, std::ios::binary);
            in.seekg(0, std::ios::end);
            size_t bufferSize = in.tellg();
            in.seekg(0, std::ios::beg);
            auto buffer = (char*)malloc(bufferSize);
            in.read(buffer, bufferSize);
            auto vec = builder.CreateVector((uint8_t*)buffer, bufferSize);
            free(buffer);
            auto nb = gear::assets::CreateNestedBundle(builder, vec);
            auto entry = gear::assets::CreateAssetEntryDirect(builder, fileName, gear::assets::Asset_NestedBundle, nb.Union());
            entries.push_back(entry);
        }

        auto bundle = gear::assets::CreateBundleDirect(builder, &entries);

        builder.Finish(bundle);
    }

    {
        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(argv[1], std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    return 0;

}