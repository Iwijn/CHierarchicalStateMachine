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
} EventType;

typedef struct Event {
    int type;
    void *data;
} Event;

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
typedef struct StateMachineData StateMachineData; // Opague data type, is defined by user of statemachine
struct StateMachine {
    State currentState;
    EventQueue *eventQueue;
    pthread_t loop_thread;
    StateMachineData *data;
};


// public
void initStateMachine(StateMachine *sm, State initialState);
void destroyStateMachine(StateMachine *sm);
void pushEvent(StateMachine *sm, Event event);

// private, but public for testing
void handleEvent(StateMachine *sm, Event event);
void transitionTo(StateMachine *sm, State newState);

