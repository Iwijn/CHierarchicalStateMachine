#include "integration_tests.h"

#define PUSH_EVENT_DELAY (100 * 1000) // 100 ms

int run_integration_tests()
{
    // Init the state machine
    StateMachine sm;
    initStateMachine(&sm, top); // will enter top, then push init event, which will cause transition into playerAlive
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_INIT!\n");
        return 1;
    }

    // Simulate left arrow press and release    
    pushEvent(&sm, (Event){EVENT_LEFT_ARROW_DOWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerMovingLeft)
    {
        printf("State is not playerMovingLeft after EVENT_LEFT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, (Event){EVENT_LEFT_ARROW_UP});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_LEFT_ARROW_UP!\n");
        return 1;
    }

    
    // Simulate left arrow press, and right arrow press without left release
    pushEvent(&sm, (Event){EVENT_LEFT_ARROW_DOWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerMovingLeft)
    {
        printf("State is not playerMovingLeft after EVENT_LEFT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, (Event){EVENT_RIGHT_ARROW_DOWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerMovingRight)
    {
        printf("State is not playerMovingRight after EVENT_RIGHT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, (Event){EVENT_RIGHT_ARROW_UP});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RIGHT_ARROW_UP!\n");
        return 1;
    }

    // Simulate player dies while standing still and buttons are pressed and released while dead, then respawn
    pushEvent(&sm, (Event){EVENT_PLAYER_DEAD});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerDead)
    {
        printf("State is not playerDead after EVENT_PLAYER_DEAD!\n");
        return 1;
    }
    pushEvent(&sm, (Event){EVENT_RIGHT_ARROW_DOWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerDead)
    {
        printf("State changes after EVENT_RIGHT_ARROW_DOWN but player is dead!\n");
        return 1;
    }
    pushEvent(&sm, (Event){EVENT_RIGHT_ARROW_UP});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerDead)
    {
        printf("State changes after EVENT_RIGHT_ARROW_DOWN but player is dead!\n");
        return 1;
    }
    pushEvent(&sm, (Event){EVENT_RESPAWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RESPAWN!\n");
        return 1;
    }

    // Simulate player dies while moving right and respawns
    pushEvent(&sm, (Event){EVENT_RIGHT_ARROW_DOWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerMovingRight)
    {
        printf("State is not playerMovingRight after EVENT_RIGHT_ARROW_DOWN!\n");
        return 1;
    }
    pushEvent(&sm, (Event){EVENT_PLAYER_DEAD});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerDead)
    {
        printf("State is not playerDead after EVENT_PLAYER_DEAD!\n");
        return 1;
    }
    pushEvent(&sm, (Event){EVENT_RESPAWN});
    usleep(PUSH_EVENT_DELAY);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RESPAWN!\n");
        return 1;
    }

    // Simulate mouse position event
    MousePosition *mp = malloc(sizeof(MousePosition));
    *mp = (MousePosition){123, 456};

    pushEvent(&sm, (Event){EVENT_MOUSE_POSITION, mp});
    usleep(PUSH_EVENT_DELAY);
    if (sm.data->mousePosition.x != 123 || sm.data->mousePosition.y != 456)
    {
        printf("Mouse position was not saved by state machine!\n");
        return 1;
    }

    destroyStateMachine(&sm);

    printf("All integration tests successful\n");
    return 0;
}