# C Hierarchical State Machine

## Rules

* The data of an event should always live on the heap. Passing such event transfers ownership to the statamachine. The top state should catch all data events and dealloc them, just in case.