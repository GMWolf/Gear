//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATION_H
#define GEAR_APPLICATION_H

#include <string>
#include "ApplicationAdapter.h"


namespace gear {

    struct AppConfig {
        int width = 640, height = 480;
        std::string title = "";
    };



    void run(const AppConfig& config, ApplicationAdapter& app);

}


#endif //GEAR_APPLICATION_H
