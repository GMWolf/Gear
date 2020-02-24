//
// Created by felix on 23/02/2020.
//

#include <cstdlib>
#include <lyra/lyra.hpp>
#include <iostream>
#include <stb.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <stb_image_write.h>
#include <vector>
#include <string>

struct FontChar {
    float advance;
    signed char xoff, yoff;
    unsigned char w, h;
    unsigned char* data;
};

int main(int argc, char* argv[]) {

    std::string inputPath;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
               | lyra::arg(inputPath, "input file")
                       ("input files")
               | lyra::opt(outFileName, "output file name").required(1)
               ["-o"]["--output"]
                       ("output file name")
               | lyra::opt(printInputs)
               ["-p"]["--printInputs"]
               | lyra::help(showHelp);
    auto result = cli.parse({argc, argv});

    if (!result) {
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }

    if (showHelp)
    {
        std::cout << cli << std::endl;
        return 0;
    }

    const unsigned char* ttf_data = (unsigned char*)stb_file(const_cast<char*>(inputPath.c_str()), nullptr);

    if (!ttf_data) {
        std::cerr << "could not open " << inputPath.c_str() << "\n";
        return 1;
    }

    int bitmapWidth = 512;
    int bitmapHeight = 512;
    float lineHeight = 32.0f;

    auto* bitmap = static_cast<unsigned char*>(calloc(bitmapWidth * bitmapHeight, sizeof(unsigned char)));

    stbtt_packedchar pdata[256*2];
    memset(pdata, 0, sizeof(pdata));
    stbtt_pack_context spc;
    stbtt_PackBegin(&spc, bitmap, bitmapWidth, bitmapHeight, 0, 1, nullptr);
    stbtt_PackSetOversampling(&spc, 1, 1);
    stbtt_PackFontRange(&spc, ttf_data, 0, lineHeight, 32, 95, pdata);
    stbtt_PackEnd(&spc);
    stbi_write_png("out.png", bitmapWidth, bitmapHeight, 1, bitmap, 0);

    for(int i = 0; i < 95; i++) {

    }
}