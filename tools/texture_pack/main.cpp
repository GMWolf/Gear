//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <indicators/progress_bar.hpp>
#include <stb_rect_pack.h>
#include <memory>
#include <vector>
#include <stb_image_write.h>

int main(int argc, char* argv[]) {

    int test = 0;
    int pageWidth = 256, pageHeight = 256;
    auto cli = lyra::cli_parser()
            | lyra::opt( test, "test" )
            ["-t"]["--test"]
            ("Test option");

    auto result = cli.parse({argc, argv});
    if (!result){
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }

    indicators::ProgressBar bar;
    bar.set_bar_width(50);


    stbrp_context ctx;

    int numNodes = pageWidth;
    std::unique_ptr<stbrp_node[]> nodes = std::make_unique<stbrp_node[]>(numNodes);

    stbrp_init_target(&ctx, pageWidth, pageHeight, nodes.get(), numNodes);

    std::vector<stbrp_rect> rects;
    for(int i = 0; i < 10; i++) {
        stbrp_coord w = rand() % 150;
        stbrp_coord h = rand() % 150;
        rects.push_back({i, w, h});
    }

    bar.set_postfix_text("Packing rectangles");
    if (!stbrp_pack_rects(&ctx, rects.data(), rects.size())) {
        bar.set_progress(33);
        bar.mark_as_completed();
        std::cerr << "Could not pack\n";
    } else {
        bar.set_progress(33);
        bar.set_postfix_text("Writing page");

        std::vector<unsigned char[4]> data(pageWidth*pageHeight);
        for(auto& p : data) {
            p[0] = p[1] = p[2] = p[3] = 0;
        }
        for(int i = 0; i < 10; i++) {
            auto rect = rects[i];
            for(int ix = 0; ix < rect.w; ix++) {
                for(int iy = 0; iy < rect.h; iy++) {
                    auto px = rect.x + ix;
                    auto py = rect.y + iy;
                    auto& p = data[px + py * pageWidth];
                    p[0] = (rect.id *25);
                    p[1] = p[0];
                    p[2] = p[0];
                    p[3] = 255;
                }
            }
        }

        bar.set_progress(66);
        bar.set_postfix_text("Writing file");

        stbi_write_png("out.png", pageWidth, pageHeight, 4, data.data(), pageWidth*4);

        bar.set_progress(100);
        bar.set_postfix_text("done!");
        bar.mark_as_completed();
    }

    return 0;
}