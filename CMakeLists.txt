cmake_minimum_required(VERSION 3.10)

# Set the project name
project(StateMachine)

# Add the executable
add_executable(StateMachine main.c StateMachine.c unit_tests.c StateMachineExample.c)
# Create libraries
add_library(IntegrationTestsLib StateMachineExample.c integration_tests.c)
add_library(UnitTestsLib unit_tests.c)

# Link libraries to the executable
target_link_libraries(StateMachine IntegrationTestsLib UnitTestsLib)