#include "integration_tests.h"

int run_integration_tests()
{
    // We need the sleeps because the other thread needs to handle the events we push

    // Init the state machine
    StateMachine sm;
    initStateMachine(&sm, top);
    sleep(0.1);
    if (sm.currentState != top)
    {
        printf("State is not top after init!\n");
        return 1;
    }

    pushEvent(&sm, EVENT_INIT);
    sleep(0.1);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_INIT!\n");
        return 1;
    }


    // Simulate left arrow press and release    
    pushEvent(&sm, EVENT_LEFT_ARROW_DOWN);
    sleep(0.1);
    if (sm.currentState != playerMovingLeft)
    {
        printf("State is not playerMovingLeft after EVENT_LEFT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, EVENT_LEFT_ARROW_UP);
    sleep(0.1);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_LEFT_ARROW_UP!\n");
        return 1;
    }

    
    // Simulate left arrow press, and right arrow press without left release
    pushEvent(&sm, EVENT_LEFT_ARROW_DOWN);
    sleep(0.1);
    if (sm.currentState != playerMovingLeft)
    {
        printf("State is not playerMovingLeft after EVENT_LEFT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, EVENT_RIGHT_ARROW_DOWN);
    sleep(0.1);
    if (sm.currentState != playerMovingRight)
    {
        printf("State is not playerMovingRight after EVENT_RIGHT_ARROW_DOWN!\n");
        return 1;
    }

    pushEvent(&sm, EVENT_RIGHT_ARROW_UP);
    sleep(0.1);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RIGHT_ARROW_UP!\n");
        return 1;
    }

    // Simulate player dies while standing still and buttons are pressed and released while dead, then respawn
    pushEvent(&sm, EVENT_PLAYER_DEAD);
    sleep(0.1);
    if (sm.currentState != playerDead)
    {
        printf("State is not playerDead after EVENT_PLAYER_DEAD!\n");
        return 1;
    }
    pushEvent(&sm, EVENT_RIGHT_ARROW_DOWN);
    sleep(0.1);
    if (sm.currentState != playerDead)
    {
        printf("State changes after EVENT_RIGHT_ARROW_DOWN but player is dead!\n");
        return 1;
    }
    pushEvent(&sm, EVENT_RIGHT_ARROW_UP);
    sleep(0.1);
    if (sm.currentState != playerDead)
    {
        printf("State changes after EVENT_RIGHT_ARROW_DOWN but player is dead!\n");
        return 1;
    }
    pushEvent(&sm, EVENT_RESPAWN);
    sleep(0.1);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RESPAWN!\n");
        return 1;
    }

    // Simulate player dies while moving right and respawns
    pushEvent(&sm, EVENT_RIGHT_ARROW_DOWN);
    sleep(0.1);
    if (sm.currentState != playerMovingRight)
    {
        printf("State is not playerMovingRight after EVENT_RIGHT_ARROW_DOWN!\n");
        return 1;
    }
    pushEvent(&sm, EVENT_PLAYER_DEAD);
    sleep(0.1);
    if (sm.currentState != playerDead)
    {
        printf("State is not playerDead after EVENT_PLAYER_DEAD!\n");
        return 1;
    }
    pushEvent(&sm, EVENT_RESPAWN);
    sleep(0.1);
    if (sm.currentState != playerAlive)
    {
        printf("State is not playerAlive after EVENT_RESPAWN!\n");
        return 1;
    }

    
    destroyStateMachine(&sm);

    printf("All integration tests successful\n");
    return 0;
}