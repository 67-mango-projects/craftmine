# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project(craftmine_project)
add_executable(craftmine
        "src/main.cpp"
        "src/game.cpp"
        "src/game.h"
        "src/renderer/Buffer.h"
        "src/renderer/IndexBuffer.h"
        "src/renderer/IndexBuffer.cpp"
        "src/renderer/Renderer.h"
        "src/renderer/Renderer.cpp"
        "src/renderer/VertexArray.cpp"
        "src/renderer/VertexArray.h"
        "src/renderer/VertexBuffer.h"
        "src/renderer/VertexBuffer.cpp"
        "src/renderer/VertexBufferLayout.h"
        src/save/SaveSystem.cpp
        src/save/SaveSystem.h
        src/save/SaveSystem.h
 "src/renderer/Shader.h" "src/renderer/Shader.cpp")
target_include_directories(craftmine PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLFW" PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLEW" , PRIVATE "${CMAKE_SOURCE_DIR}/dependencies" , PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/spdlog")
target_link_libraries(craftmine
        PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLEW/glew32s.lib"
        PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLFW/glfw3.lib"
        opengl32.lib
)
target_compile_definitions(craftmine PRIVATE GLEW_STATIC)
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
if (MSVC)
    target_compile_options(craftmine PRIVATE "/utf-8")
endif ()


if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET craftmine PROPERTY CXX_STANDARD 20)
endif ()





