#include <iostream>
#include "SDL.h"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <fstream>
#include <stdio.h>
#include "vec3.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "helper.h"
#include "hittable_manager.h"
#include <memory>
#include "camera.h"

void write_color(std::ostream& out, const color_t& pixel, int samples_per_pixel);
void render_to_img();
color_t get_ray_color(const ray_t& ray, const hittable_manager_t& world);

color_t get_ray_color(const ray_t& ray, const hittable_manager_t& world) {

	hit_info_t hit_info;
	if (world.hit(ray, 0.0f, infinity, hit_info)) {
		return 0.5f * (hit_info.normal + vec3_t(1.0f, 1.0f, 1.0f));
	}

	vec3_t unit_direction = unit_vector(ray.dir);
	auto t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * color_t(1.0, 1.0, 1.0) + t * color_t(0.5, 0.7, 1.0);
}

void render_to_img() {

	const float aspect_ratio = 16.0f / 9.0f;
	const float image_width = 800;
	const float image_height = image_width / aspect_ratio;

	float focal_len = 1.0f;
	point_t origin(0, 0, 0);

	const vec3_t viewport_height(0.0f, 2.0f, 0.0f);
	const vec3_t viewport_width(viewport_height.y * aspect_ratio, 0, 0);

	point_t lower_left = origin - viewport_height / 2 - viewport_width / 2 - point_t(0, 0, focal_len);

	std::shared_ptr<sphere_t> sphere = std::make_shared<sphere_t>(point_t(0, 0, -1.0f), 0.5f);
	std::shared_ptr<sphere_t> sphere2 = std::make_shared<sphere_t>(point_t(0, -100.5f, -1.0f), 100.0f);

	hittable_manager_t world;
	world.add(sphere);
	world.add(sphere2);

	std::ofstream image;
	image.open("output.ppm");
	image << "P3\n" << image_width << " " << image_height << "\n255\n";

	camera_t camera;
	int samples_per_pixel = 100;

	for (int row = image_height - 1; row >= 0; row--) {
		std::cerr << "scanning row " << row << "\n" << std::flush;
		for (int col = 0; col < image_width; col++) {

			color_t color(0, 0, 0);
			for (int i = 0; i < samples_per_pixel; i++) {
				float y = (row + random_float()) / (image_height - 1);
				float x = (col + random_float()) / (image_width - 1);

				ray_t ray = camera.get_ray(x, y);
				color_t sample_color = get_ray_color(ray, world);
				color += sample_color;
			}

			write_color(image, color, samples_per_pixel);
		}
	}
	image.close();

}

void write_color(std::ostream& out, const color_t& color, int samples_per_pixel) {
	out << static_cast<int>(256 * clamp(color.x / samples_per_pixel, 0, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(color.y / samples_per_pixel, 0, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(color.z / samples_per_pixel, 0, 0.999f)) << '\n';
}

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw std::exception("could not initialize sdl");
	}

	render_to_img();

	// set opengl attributes through sdl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	uint32_t winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Window* sdlWindow = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, winFlags);

	SDL_GLContext sdlContext = SDL_GL_CreateContext(sdlWindow);

	SDL_GL_MakeCurrent(sdlWindow, sdlContext);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow, sdlContext);
	const char* glslVersion = "#version 330";
	ImGui_ImplOpenGL3_Init(glslVersion);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				running = false;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdlWindow)) {
				running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode keyDown = event.key.keysym.sym;
				if (keyDown == SDLK_RETURN) {
					std::cout << "hit return key" << std::endl;
				}
				else if (keyDown == SDLK_RIGHT) {
				}
				else if (keyDown == SDLK_LEFT) {
				}
				else if (keyDown == SDLK_UP) {
				}
				else if (keyDown == SDLK_DOWN) {
				}
				else if (keyDown == SDLK_SLASH) {
				}
				else if (keyDown == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("window");
		ImGui::Button("btn1");
		ImGui::End();

		ImGui::Begin("window2");
		ImGui::Button("btn2");
		ImGui::End();

		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, 800, 800);

		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backupCurWin = SDL_GL_GetCurrentWindow();
			SDL_GLContext backupCurContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backupCurWin, backupCurContext);
		}

		SDL_GL_SwapWindow(sdlWindow);
	}


	return -1;
}