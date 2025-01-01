#include "StateMachineExample.h"

void* top(StateMachine *sm, Event e)
{
    switch (e.type)
    {
        case EVENT_ENTRY:
            printf("ENTRY in state: %s\n", __func__);
            // Allocate statemachine data
            sm->data = malloc(sizeof(StateMachineData));
            return NULL;
        case EVENT_INIT:
            printf("INIT in state: %s\n", __func__);
            transitionTo(sm, playerAlive);
            return NULL;
        case EVENT_EXIT:
            printf("EXIT in state: %s\n", __func__);
            // Deallocate statemachine data
            free(sm->data);
            return NULL;
        case EVENT_PLAYER_DEAD:
            transitionTo(sm, playerDead);
            return NULL;
        case EVENT_MOUSE_POSITION:
            MousePosition *mp = (MousePosition*)e.data;
            printf("Mouse position: x: %d, y: %d\n", mp->x, mp->y);
            sm->data->mousePosition = (MousePosition){mp->x, mp->y};
            free(e.data); // We the catcher of the event is responsible for freeing the data when its done with it
            return NULL;
        default:
            if (e.type != EVENT_NULL) ("!!!Uncaught event %d in state: %s\n", e.type, __func__);
            break;
    }
    return NULL;
}

void* playerAlive(StateMachine *sm, Event e)
{
    switch (e.type)
    {
        case EVENT_ENTRY:
            printf("ENTRY in state: %s\n", __func__);
            return NULL;
        case EVENT_EXIT:
            printf("EXIT in state: %s\n", __func__);
            return NULL;
        case EVENT_LEFT_ARROW_DOWN:
            transitionTo(sm, playerMovingLeft);
            return NULL;
        case EVENT_RIGHT_ARROW_DOWN:
            transitionTo(sm, playerMovingRight);
            return NULL;
    }
    return top;
}

void* playerMovingLeft(StateMachine *sm, Event e)
{
    switch (e.type)
    {
        case EVENT_ENTRY:
            printf("ENTRY in state: %s\n", __func__);
            // Code that starts moving the player left
            return NULL;
        case EVENT_EXIT:
            printf("EXIT in state: %s\n", __func__);
            // Code that stops moving the player left
            return NULL;
        case EVENT_LEFT_ARROW_UP:
            transitionTo(sm, playerAlive);
            return NULL;
    }
    return playerAlive;
}


void* playerMovingRight(StateMachine *sm, Event e)
{
    switch (e.type)
    {
        case EVENT_ENTRY:
            printf("ENTRY in state: %s\n", __func__);
            // Code that starts moving the player right
            return NULL;
        case EVENT_EXIT:
            printf("EXIT in state: %s\n", __func__);
            // Code that stops moving the player right
            return NULL;
        case EVENT_RIGHT_ARROW_UP:
            transitionTo(sm, playerAlive);
            return NULL;
    }
    return playerAlive;
}

void* playerDead(StateMachine *sm, Event e)
{
    switch (e.type)
    {
        case EVENT_ENTRY:
            printf("ENTRY in state: %s\n", __func__);
            return NULL;
        case EVENT_EXIT:
            printf("EXIT in state: %s\n", __func__);
            return NULL;
        case EVENT_RESPAWN:
            transitionTo(sm, playerAlive);
            return NULL;
    }
    return top;
}


