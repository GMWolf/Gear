//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATIONADAPTER_H
#define GEAR_APPLICATIONADAPTER_H

namespace gear {
    class Application;
    class ApplicationAdapter {
    public:
        virtual void init(Application* app) = 0;
        virtual void update() = 0;
        virtual void end() = 0;
    };
}


#endif //GEAR_APPLICATIONADAPTER_H
