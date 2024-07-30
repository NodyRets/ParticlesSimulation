
add_library(glad STATIC ${PROJECT_SOURCE_DIR}/external/glad/src/gl.c)

target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/external/glad/include)

add_executable(ParticlesSimulation
    Main.cpp
    Utils/FileUtils.cpp
     
 "Callbacks.cpp" "Utils/ShaderUtils.cpp" "ParticlesRenderer.cpp")

target_include_directories(ParticlesSimulation PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(ParticlesSimulation PRIVATE glm::glm glfw glad spdlog::spdlog imgui::imgui)

