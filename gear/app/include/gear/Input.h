//
// Created by felix on 09/07/2020.
//

#ifndef GEAR_INPUT_H
#define GEAR_INPUT_H

#include <glm/vec2.hpp>
#include "KeyboardKeys.h"

namespace gear {

    enum class KeyEvent : uint8_t {
        PRESS = 1,
        RELEASE = 0,
        HELD = 2
    };

    class InputState {

        int keyPressedFrame[static_cast<int>(KEYS::COUNT)]{0};
        int keyReleasedFrame[static_cast<int>(KEYS::COUNT)]{0};

        int frame = 1;

    public:

        void updateFrame(int frame);
        void updateKey(KEYS key, KeyEvent event);

        [[nodiscard]] glm::vec2 mousePosition() const;

        [[nodiscard]] int keyGetFramePressed(KEYS key) const;
        [[nodiscard]] int keyGetFrameReleased(KEYS key) const;
        [[nodiscard]] int keyFetHeldDuration(KEYS key) const;
        [[nodiscard]] bool keyDown(KEYS key) const;
        [[nodiscard]] bool keyUp(KEYS key) const;
        [[nodiscard]] bool keyPressed(KEYS key) const;
        [[nodiscard]] bool keyReleased(KEYS key) const;
    };



}


#endif //GEAR_INPUT_H
