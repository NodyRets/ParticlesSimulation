cmake_minimum_required (VERSION 3.8)
project("ParticlesSimulation" VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(glad STATIC ${PROJECT_SOURCE_DIR}/external/glad/src/gl.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/external/glad/include)

add_executable(ParticlesSimulation
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
    ${PROJECT_SOURCE_DIR}/src/Utils/FileUtils.cpp
    ${PROJECT_SOURCE_DIR}/src/Callbacks.cpp
    ${PROJECT_SOURCE_DIR}/src/Utils/ShaderUtils.cpp
    ${PROJECT_SOURCE_DIR}/src/ParticlesRenderer.cpp
)

target_include_directories(ParticlesSimulation PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(ParticlesSimulation PRIVATE glm::glm glfw glad spdlog::spdlog imgui::imgui)

set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/shaders")

add_custom_command(
    TARGET ParticlesSimulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
    COMMENT "Copying SHADERS to build directory"
)

enable_testing()
add_subdirectory(tests)
