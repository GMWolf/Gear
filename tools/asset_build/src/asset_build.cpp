//
// Created by felix on 10/11/2020.
//

#include <lyra/lyra.hpp>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <vector>

namespace fs = std::filesystem;
bool isImageExt(const char* str) {
    const char* imageExtensions[] = {
            ".png",
            ".tif"
    };

    for(auto imgExt : imageExtensions) {
        if (strcmp(str, imgExt) == 0) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {

    std::string inputPathName;
    std::string outFileName = "out";
    {
        bool showHelp = false;
        auto cli = lyra::cli_parser()
                   | lyra::arg(inputPathName, "input path")
                   ("input path").cardinality(1, std::numeric_limits<size_t>::max())
                   | lyra::opt(outFileName, "output file name").required(1)
                   ["-o"]["--output"]("output file name")
                   | lyra::help(showHelp);

        auto result = cli.parse({argc, argv});
        if (!result) {
            std::cerr << result.errorMessage() << "\n";
            return 1;
        }
    }

    fs::path inputPath(inputPathName);

    if (!fs::is_directory(inputPath)) {
        std::cerr << inputPathName << " is not a directory.\n";
        return 1;
    }

    for(auto& entry : fs::recursive_directory_iterator(inputPath)) {
        if (entry.is_regular_file()) {
            const char* ext = entry.path().extension().c_str();
            if (isImageExt(ext)) {

            }
        }
    }

}