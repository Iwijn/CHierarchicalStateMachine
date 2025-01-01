#include "StateMachine.h"


typedef enum {
    EVENT_LEFT_ARROW_DOWN = EVENT_NULL + 1,
    EVENT_LEFT_ARROW_UP,
    EVENT_RIGHT_ARROW_DOWN,
    EVENT_RIGHT_ARROW_UP,
    EVENT_PLAYER_DEAD,
    EVENT_RESPAWN,
    EVENT_MOUSE_POSITION,
} GAME_EVENT;

typedef struct MousePosition {
    int x;
    int y;
} MousePosition;

// public
void* top(StateMachine *sm, Event e);
// private, public for testing
    void* playerAlive(StateMachine *sm, Event e);
        void* playerMovingLeft(StateMachine *sm, Event e);
        void* playerMovingRight(StateMachine *sm, Event e);
    void* playerDead(StateMachine *sm, Event e);

