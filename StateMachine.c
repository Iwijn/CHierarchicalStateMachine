#include "StateMachine.h"

#define STATEMACHINE_PRINT_DEBUG 0

#define MAX_DEPTH (10)
#define EVENT_QUEUE_CAPACITY (10)

////// Event loop logic
void* event_loop(void *arg);
EventQueue* create_event_queue(int capacity);
void destroy_event_queue(EventQueue *queue);
void enqueue(EventQueue *queue, Event event);
Event dequeue(EventQueue *queue);

// The event loop function running in a separate thread
void* event_loop(void *arg) {
    StateMachine *sm = (StateMachine *)arg;

    while (1) {
        Event event = dequeue(sm->eventQueue); // Get an event from the queue
        #if STATEMACHINE_PRINT_DEBUG
        printf("Processing event ID: %d\n", event); // Process the event
        #endif
        
        // -1 to break out of this loop for graceful shutdown.
        if (event == -1) {
            break;
        }

        handleEvent(sm, event);
    }
    
    return NULL;
}


// Function to initialize the event queue
EventQueue* create_event_queue(int capacity) {
    EventQueue *queue = malloc(sizeof(EventQueue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    queue->events = malloc(capacity * sizeof(Event));
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond_var, NULL);
    return queue;
}

// Function to destroy the event queue
void destroy_event_queue(EventQueue *queue) {
    free(queue->events);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond_var);
    free(queue);
}

// Function to add an event to the queue
void enqueue(EventQueue *queue, Event event) {
    pthread_mutex_lock(&queue->mutex);
    
    // Wait if the queue is full
    while (queue->size == queue->capacity) {
        printf("Event queue is full, waiting for space\n");
        pthread_cond_wait(&queue->cond_var, &queue->mutex);
    }

    // Add the event to the queue
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->events[queue->rear] = event;
    queue->size++;

    // Signal that an event has been added
    pthread_cond_signal(&queue->cond_var);
    
    pthread_mutex_unlock(&queue->mutex);
}

// Function to remove an event from the queue
Event dequeue(EventQueue *queue) {
    pthread_mutex_lock(&queue->mutex);

    // Wait if the queue is empty
    while (queue->size == 0) {
        pthread_cond_wait(&queue->cond_var, &queue->mutex);
    }

    // Remove an event from the front of the queue
    Event event = queue->events[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    // Signal that space is available in the queue
    pthread_cond_signal(&queue->cond_var);

    pthread_mutex_unlock(&queue->mutex);
    
    return event;
}

///////// State machine logic

void initStateMachine(StateMachine *sm, State initialState)
{
    sm->currentState = initialState;
    sm->eventQueue = create_event_queue(EVENT_QUEUE_CAPACITY);
    pthread_create(&sm->loop_thread, NULL, event_loop, (void *)sm);
    enqueue(sm->eventQueue, EVENT_ENTRY);
}

void destroyStateMachine(StateMachine *sm)
{
    enqueue(sm->eventQueue, -1); // Signal the event loop to exit
    pthread_join(sm->loop_thread, NULL);
    destroy_event_queue(sm->eventQueue);
}

void pushEvent(StateMachine *sm, Event event)
{
    enqueue(sm->eventQueue, event);
}

void handleEvent(StateMachine *sm, Event event)
{
    // If child state does not catch, pass event to parent state.
    State parentState = sm->currentState(sm, event);
    while(parentState != NULL)
    {
        parentState = parentState(sm, event);
    }
}

void findCommonAncestor(State* currentStateHierarchy, State* newStateHierarchy, StateMachine *sm, State newState, int* currentHierarchyCommonAncestorIndex,  int* newHierarchyCommonAncestorIndex)
{
    // TODO: make this efficient
    currentStateHierarchy[0] = sm->currentState;
    newStateHierarchy[0] = newState;
    #if STATEMACHINE_PRINT_DEBUG
    printf("Finding common ancestor\n");
    printf("Current state: %p\n", currentStateHierarchy[0]);
    printf("New state: %p\n", newStateHierarchy[0]);
    #endif

    int i = 0;
    while(1)
    {
        // Check if there is any common parent state yet
        for(int x = 0; x <= i; x++)
        {
            for (int y = 0; y <= i; y++)
            {
                #if STATEMACHINE_PRINT_DEBUG
                printf("Comparing new: %p with current: %p\n", newStateHierarchy[x], currentStateHierarchy[y]);
                #endif
                if(newStateHierarchy[x] == currentStateHierarchy[y])
                {
                    // If so, the common ancestor is found
                    *currentHierarchyCommonAncestorIndex = y;
                    *newHierarchyCommonAncestorIndex = x;
                    return; 
                }    
            }
        }
        // Common ancestor not found, progress 1 up the hierachy of both current and new state
        currentStateHierarchy[i+1] = currentStateHierarchy[i](sm, EVENT_NULL);
        newStateHierarchy[i+1] = newStateHierarchy[i](sm, EVENT_NULL);
        i++;
    }

}

void transitionTo(StateMachine *sm, State newState)
{
    // TODO: dynamic memory alloc so hierarchy can be of any size
    // Find the commen ancestor of the current state and the new state
    State currentStateHierarchy[MAX_DEPTH];
    State newStateHierarchy[MAX_DEPTH];
    int currentHierarchyCommonAncestorIndex = 0;
    int newHierarchyCommonAncestorIndex = 0;
    findCommonAncestor(currentStateHierarchy, newStateHierarchy, sm, newState, &currentHierarchyCommonAncestorIndex, &newHierarchyCommonAncestorIndex);
    
    #if STATEMACHINE_PRINT_DEBUG
    printf("Exiting parent states starting from current state until common ancestor\n");
    #endif
    // Exit the current state and all its parents up to the common ancestor
    // We do not exit out of the common state, as it is a parent of new state as well
    for (int k = 0; k < currentHierarchyCommonAncestorIndex; k++)
    {
        #if STATEMACHINE_PRINT_DEBUG
        printf("Exiting state: %p\n", currentStateHierarchy[k]);
        #endif
        currentStateHierarchy[k](sm, EVENT_EXIT);
    }

    #if STATEMACHINE_PRINT_DEBUG
    printf("Entering states starting from common ancestor until new state\n");
    #endif
    // Enter the common state and all its children down to the new state
    // We do not enter the common state, as it is a parent of current state
    for (int k = newHierarchyCommonAncestorIndex-1; k >= 0; k--)
    {
        #if STATEMACHINE_PRINT_DEBUG
        printf("Entering state: %p\n", newStateHierarchy[k]);
        #endif
        newStateHierarchy[k](sm, EVENT_ENTRY);
    }

    // Update state
    sm->currentState = newState;
}
