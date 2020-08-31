//
// Created by felix on 07/06/2020.
//
#include <iostream>
#include <fstream>
#include <gear/fbs/generated/assets_generated.h>
#include <flatbuffers/hash.h>

using namespace gear::assets;

int main(int argc, char* argv[]) {


    flatbuffers::FlatBufferBuilder builder(4096);
    std::vector<flatbuffers::Offset<NestedBundle>> bundles;
    {
        for(int i = 2; i < argc; i++) {
            char *fileName = argv[i];
            std::ifstream in(fileName, std::ios::binary);
            in.seekg(0, std::ios::end);
            size_t bufferSize = in.tellg();
            uint8_t * buffer;
            auto vec = builder.CreateUninitializedVector(bufferSize, &buffer);
            in.seekg(0, std::ios::beg);
            in.read((char*)buffer, bufferSize);
            
            auto nb = CreateNestedBundle(builder, vec);
            bundles.push_back(nb);
        }
    }

    auto bundle = CreateBundleDirect(builder, nullptr, &bundles);
    builder.Finish(bundle);

    {
        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(argv[1], std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    return 0;

}