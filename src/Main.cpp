#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "ParticlesRenderer.h"
#include "Callbacks.h"
#include "Utils/FileUtils.h"
#include "ParticleFrameData.h"

int main() {
	glfwSetErrorCallback(Callbacks::error_callback);
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Particles Simulator", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ParticlesRenderer particlesRenderer;
	if (!particlesRenderer.initialize()) {
		return -1;
	}

	ParticleFrameData frameData;
	frameData.width = 1920;
	frameData.height = 1080;
	frameData.particleSize = 1.0f;
	frameData.activeParticles = ParticlesRenderer::MAX_NUMBER_OF_PARTICLES;

	double lastFrameTime = glfwGetTime();
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	std::string fpsText = "FPS: 0";
	double currentTime;
	double xpos = 0, ypos = 0;
	ImVec2 wPos = { 0,0 };
	ImVec2 wSize = { 0,0 };

	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		frameData.deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		nbFrames++;
		if (currentTime - lastTime >= 1.0) {
			fpsText = "FPS: " + std::to_string(nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::Begin("Workspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginChild("Controls", ImVec2(300, 0), true);
		ImGui::Text("Adjust particle settings here.");
		ImGui::SliderFloat("Example Slider", &frameData.particleSize, 1.0f, 50.0f);
		ImGui::SliderInt("Number of particles", &frameData.activeParticles, 0, ParticlesRenderer::MAX_NUMBER_OF_PARTICLES);
		ImGui::Text("Delta Time: %f", frameData.deltaTime);
		ImGui::Text("Mouse X: %f", frameData.mousePosX);
		ImGui::Text("Mouse Y: %f", frameData.mousePosY);
		ImGui::Text("Viewport Width: %d", frameData.width);
		ImGui::Text("Viewport Height: %d", frameData.height);
		ImGui::Text("Raw Cursor Position: (%.1f, %.1f)", xpos, ypos);
		ImGui::Text("Raw Window Position: (%.1f, %.1f)", wPos.x, wPos.y);
		ImGui::Text("Window Size: (%.1f, %.1f)", wSize.x, wSize.y);
		ImGui::Text("%s", fpsText.c_str());
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild("Particle Display", ImVec2(-1, -1));
		wPos = ImGui::GetWindowPos();
		wSize = ImGui::GetWindowSize();

		ImGui::EndChild();
		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		frameData.width = wSize.x;
		frameData.height = wSize.y;

		glfwGetCursorPos(window, &xpos, &ypos);
		frameData.mousePosX = static_cast<float>(xpos - wPos.x);
		frameData.mousePosY = static_cast<float>(ypos - wPos.y);


		glViewport(wPos.x, wPos.y, wSize.x, wSize.y);
		particlesRenderer.calculateFrame(frameData);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
