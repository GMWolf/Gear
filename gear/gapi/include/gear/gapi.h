//
// Created by felix on 25/10/2020.
//

#ifndef GEAR_GAPI_H
#define GEAR_GAPI_H

#include <variant>

namespace gear {

    struct Gapi {

        struct Ogl {
            using LoadProcFn = void* (const char*);
            using LoadGLLoaderFn = int(LoadProcFn*);
            int version_major;
            int version_minor;
            LoadGLLoaderFn* loadGLLoader;
        };

        std::variant<Ogl> api;
    };

}

#endif //GEAR_GAPI_H
