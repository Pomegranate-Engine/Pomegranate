# Add the following lines to enable OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(POMEGRANATE_SOURCES
        src/engine/engine.cpp
        src/engine/standard_ecs.cpp
        src/engine/ecs.cpp
        src/engine/input_manager.cpp
        src/engine/vec_math.cpp
        src/engine/window.cpp
        src/engine/event_system.cpp
        src/engine/color.cpp
        src/engine/standard_ecs_rendering.cpp
        src/engine/standard_ecs_audio.cpp
        src/engine/standard_ecs_physics.cpp
)