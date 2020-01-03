//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATIONADAPTER_H
#define GEAR_APPLICATIONADAPTER_H

namespace gear {
    class ApplicationAdapter {
    public:
        virtual void init() = 0;
        virtual void update() = 0;
    };
}


#endif //GEAR_APPLICATIONADAPTER_H
