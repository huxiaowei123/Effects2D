#ifndef EFFECTS2D_ENUMTYPE_H
#define EFFECTS2D_ENUMTYPE_H

enum TouchActionType {
    TOUCH_DOWN =0,
    TOUCH_UP,
    TOUCH_MOVE,
    TOUCH_CANCEL
};

enum MessageType {
    NONE = 0,
    TOUCH_EVENT,
    SET_WINDOW,
    DO_ANIMATION,
    STOP_ANIMATION,
    EXIT
};

enum ShaderType {
    NOT_SET = 0,
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    COMPUTER_SHADER
};

enum EffectType {
    DEFAULT_EFFECT = 0,
};

#endif //EFFECTS2D_ENUMTYPE_H
