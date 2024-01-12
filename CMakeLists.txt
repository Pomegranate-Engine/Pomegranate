cmake_minimum_required(VERSION 3.27)
project(Pomegranate_Engine)

# Set the path to the SDL library directory
set(SDL_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/Lib/x64)

link_directories("${CMAKE_SOURCE_DIR}/bin/")

# Include directories
include_directories("Include/" "Include/Pomegranate")

# Source files
set(SOURCE_FILES src/main.cpp)

set(POMEGRANATE_DIR ${CMAKE_SOURCE_DIR}/Include/Pomegranate)

include("Include/Pomegranate/Pomegranate.cmake")

# Add executable
add_executable(Pomegranate_Engine ${SOURCE_FILES})



# Link SDL2 libraries
target_link_libraries(Pomegranate_Engine PRIVATE
        ${SDL_LIBRARY_DIR}/SDL3.lib
        ${SDL_LIBRARY_DIR}/SDL3_ttf.lib
        ${SDL_LIBRARY_DIR}/SDL3_image.lib
        ${SDL_LIBRARY_DIR}/SDL3_mixer.lib
        Pomegranate_Library.lib
)



# Set the output directory for the executable
set_target_properties(Pomegranate_Engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)


# Copy SDL DLL files to the output directory
file(GLOB SDL_DLLS ${SDL_LIBRARY_DIR}/*.dll)
foreach(DLL ${SDL_DLLS})
    add_custom_command(TARGET Pomegranate_Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            $<TARGET_FILE_DIR:Pomegranate_Engine>)
endforeach()
