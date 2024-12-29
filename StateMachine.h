#pragma once
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Define events
typedef enum {
    EVENT_ENTRY,
    EVENT_INIT,
    EVENT_EXIT,
    EVENT_NULL
} Event; // TODO make struct that can hold data

// Forward declare the StateMachine struct
typedef struct StateMachine StateMachine;

// Define a State as a function pointer to a function that takes StateMachine and Event, and returns State
typedef void* (*State)(StateMachine* sm, Event e);


typedef struct EventQueue {
    Event *events;
    int capacity;
    int size;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
} EventQueue;


// Define the StateMachine struct
struct StateMachine {
    State currentState;
    EventQueue *eventQueue;
    pthread_t loop_thread;
};


// public
void initStateMachine(StateMachine *sm, State initialState);
void destroyStateMachine(StateMachine *sm);
void pushEvent(StateMachine *sm, Event event);

// private, but public for testing
void handleEvent(StateMachine *sm, Event event);
void transitionTo(StateMachine *sm, State newState);

