#include "unit_tests.h"

int event_entry_counter = 0;
int event_init_counter = 0;
int event_exit_counter = 0;
int event_null_counter = 0;

static void* top(StateMachine *sm, Event e)
{
    printf("top state with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return NULL;
}

void* state1(StateMachine *sm, Event e)
{
    printf("State 1 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return top;
}

void* state2_0(StateMachine *sm, Event e)
{
    printf("State 2_0 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return state1;
}

void* state2_1(StateMachine *sm, Event e)
{
    printf("State 2_1 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return state1;
}

void* state2_2(StateMachine *sm, Event e)
{
    printf("State 2_2 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return state1;
}

void* state3(StateMachine *sm, Event e)
{
    printf("State 3 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return state2_0;
}


void* state4(StateMachine *sm, Event e)
{
    printf("State 3 with event %d\n", e.type);
    if (e.type == EVENT_ENTRY) event_entry_counter++;
    if (e.type == EVENT_INIT) event_init_counter++;
    if (e.type == EVENT_EXIT) event_exit_counter++;
    if (e.type == EVENT_NULL) event_null_counter++;
    return state2_1;
}

int test_event_up_chain()
{
    printf("----- \"Event up chain\" test\n");
    event_null_counter = 0;
    StateMachine sm;
    sm.currentState = state3;
    handleEvent(&sm, (Event){EVENT_NULL, NULL}); // should be handled in state3, state2_0, state1, top
    if (event_null_counter == 4)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n");
        return 1;
    }
}

int test_transition_to_parent()
{
    printf("----- \"Transition to parent\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state3;
    transitionTo(&sm, state2_0);
    if (sm.currentState == state2_0 && event_exit_counter == 1 && event_entry_counter == 0)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state2_0, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}

int test_transition_to_grandparent()
{
    printf("----- \"Transition to grandparent\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state3;
    transitionTo(&sm, state1);
    if (sm.currentState == state1 && event_exit_counter == 2 && event_entry_counter == 0)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state1, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}


int test_transition_to_child()
{
    printf("----- \"Transition to child\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state2_0;
    transitionTo(&sm, state3);
    if (sm.currentState == state3 && event_exit_counter == 0 && event_entry_counter == 1)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state3, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}


int test_transition_to_grandchild()
{
    printf("----- \"Transition to grandchild\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state1;
    transitionTo(&sm, state3);
    if (sm.currentState == state3 && event_exit_counter == 0 && event_entry_counter == 2)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state1, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}


int test_transition_to_sibling()
{
    printf("----- \"Transition to sibling\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state2_0;
    transitionTo(&sm, state2_1);
    if (sm.currentState == state2_1 && event_exit_counter == 1 && event_entry_counter == 1)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state2_1, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}

int test_transition_to_aunt()
{
    printf("----- \"Transition to aunt\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state3;
    transitionTo(&sm, state2_1);
    if (sm.currentState == state2_1 && event_exit_counter == 2 && event_entry_counter == 1)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state2_1, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}

int test_transition_to_niece_lower_generation()
{
    printf("----- \"Transition to niece lower generation\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state2_1;
    transitionTo(&sm, state3);
    if (sm.currentState == state3 && event_exit_counter == 1 && event_entry_counter == 2)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state3, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}


int test_transition_to_niece_same_generation()
{
    printf("----- \"Transition to niece same generation\" test\n");
    event_exit_counter = 0;
    event_entry_counter = 0;
    StateMachine sm;
    sm.currentState = state3;
    transitionTo(&sm, state4);
    if (sm.currentState == state4 && event_exit_counter == 2 && event_entry_counter == 2)
    {
        printf("successful\n");
        return 0;
    }
    else
    {
        printf("failed\n\tcorrect state: %p\n\tactual state: %p\n\texit counter: %d\n\tentry counter: %d\n", state4, sm.currentState, event_exit_counter, event_entry_counter);
        return 1;
    }
}

int run_unit_tests()
{
    if (test_event_up_chain()) return 1;
    if (test_transition_to_parent()) return 1;
    if (test_transition_to_grandparent()) return 1;
    if (test_transition_to_child()) return 1;
    if (test_transition_to_grandchild()) return 1;
    if (test_transition_to_sibling()) return 1;
    if (test_transition_to_aunt()) return 1;
    if (test_transition_to_niece_lower_generation()) return 1;
    if (test_transition_to_niece_same_generation()) return 1;

    printf("All tests successful\n");
    return 0;
}
