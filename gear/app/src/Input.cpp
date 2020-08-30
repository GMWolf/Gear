//
// Created by felix on 09/07/2020.
//

#include <Input.h>

void gear::InputState::updateKey(gear::KEYS key, KeyEvent event) {
    switch (event) {
        case KeyEvent::PRESS:
            keyPressedFrame[static_cast<int>(key)] = frame;
            break;
        case KeyEvent::RELEASE:
            keyReleasedFrame[static_cast<int>(key)] = frame;
            break;
        case KeyEvent::HELD:
            break;
    }
}

void gear::InputState::updateFrame(int f) {
    frame = f;
}

bool gear::InputState::keyDown(KEYS key) const {
    return keyReleasedFrame[static_cast<int>(key)] < keyPressedFrame[static_cast<int>(key)];
}

bool gear::InputState::keyUp(KEYS key) const {
    return !keyDown(key);
}

int gear::InputState::keyGetFramePressed(gear::KEYS key) const {
    return keyPressedFrame[static_cast<int>(key)];
}

int gear::InputState::keyGetFrameReleased(gear::KEYS key) const {
    return keyReleasedFrame[static_cast<int>(key)];
}
