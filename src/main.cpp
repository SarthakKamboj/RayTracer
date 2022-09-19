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
#include <math.h>
#include "materials.h"

void write_color(std::ostream& out, const color_t& pixel, int samples_per_pixel);
void render_to_img();
color_t get_ray_color(const ray_t& ray, const hittable_manager_t& world, int depth);

color_t get_ray_color(const ray_t& ray, const hittable_manager_t& world, int depth) {

	if (depth <= 0) return color_t(0, 0, 0);

	hit_info_t hit_info;
	if (world.hit(ray, 0.0001f, infinity, hit_info)) {
		// point_t new_ray_target = hit_info.point + hit_info.normal + random_unit_vector();
		// point_t new_ray_target = random_in_hemisphere(hit_info.normal);
		// vec3_t dir = new_ray_target - hit_info.point;
		// send ray in random direction and use color of whatever it hits
		color_t atten;
		ray_t scatter_ray;
		if (hit_info.mat_ptr->scatter(ray, hit_info, atten, scatter_ray)) {
			return atten * get_ray_color(scatter_ray, world, depth - 1);
		}
		return color_t(0, 0, 0);
		// return 0.5f * get_ray_color(ray_t(hit_info.point, dir), world, depth - 1);
	}

	vec3_t unit_direction = unit_vector(ray.dir);
	auto t = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - t) * color_t(1.0, 1.0, 1.0) + t * color_t(0.5, 0.7, 1.0);
}

void render_to_img() {

	const float aspect_ratio = 16.0f / 9.0f;
	const float image_width = 200;
	const float image_height = image_width / aspect_ratio;

	float focal_len = 1.0f;
	point_t origin(0, 0, 0);

	const vec3_t viewport_height(0.0f, 2.0f, 0.0f);
	const vec3_t viewport_width(viewport_height.y * aspect_ratio, 0, 0);

	point_t lower_left = origin - viewport_height / 2 - viewport_width / 2 - point_t(0, 0, focal_len);

	// std::shared_ptr<lambertian_t> lam_mat = std::make_shared<lambertian_t>(color_t(0.5f, 0.5f, 0.5f));

	// std::shared_ptr<sphere_t> sphere = std::make_shared<sphere_t>(point_t(0, 0, -1.0f), 0.5f, lam_mat);
	// std::shared_ptr<sphere_t> sphere2 = std::make_shared<sphere_t>(point_t(0, -100.5f, -1.0f), 100.0f, lam_mat);

	hittable_manager_t world;

#if 0
	auto material_ground = std::make_shared<lambertian_t>(color_t(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian_t>(color_t(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<dielectric_t>(1.5f);
	auto material_right = std::make_shared<metal_t>(color_t(0.8, 0.6, 0.2));
	// auto material_ground = std::make_shared<lambertian_t>(color_t(0.8f, 0.8f, 0.0f));
	// auto material_center = std::make_shared<lambertian_t>(color_t(0.7f, 0.3f, 0.3f));
	// auto material_left = std::make_shared<metal_t>(color_t(0.8f, 0.8f, 0.8f));
	// auto material_right = std::make_shared<dielectric_t>(1.5f);
	// auto material_left = std::make_shared<dielectric_t>(1.5f);
	// auto material_right = std::make_shared<metal_t>(color_t(0.8f, 0.6f, 0.2f));

	auto sphere1 = std::make_shared<sphere_t>(point_t(0.0f, -100.5f, -1.0f), 100.0f, material_ground);
	// auto sphere1 = std::make_shared<sphere_t>(point_t(0.0, -100.5, -1.0), 100.0, material_ground);
	auto sphere2 = std::make_shared<sphere_t>(point_t(0.0, 0.0, -1.0), 0.5, material_center);
	// auto sphere2 = std::make_shared<sphere_t>(point_t(0.0f, 0.0f, -1.0f), 0.5f, material_left);
	auto sphere3 = std::make_shared<sphere_t>(point_t(-1.0f, 0.0f, -1.0f), 0.5f, material_left);
	auto sphere4 = std::make_shared<sphere_t>(point_t(1.0f, 0.0f, -1.0f), 0.5f, material_right);
	// auto sphere4 = std::make_shared<sphere_t>(point_t(0.0f, 100.0f, -1.0f), 0.5f, material_right);

	world.add(sphere1);
	world.add(sphere2);
	world.add(sphere3);
	world.add(sphere4);
	world.add(std::make_shared<sphere_t>(point_t(-1.0, 0.0, -1.0), -0.4f, material_left));

	// world.add(sphere);
	// world.add(sphere2);
#endif

#if 0
	auto R = cos(pi / 4);

	auto material_left = std::make_shared<lambertian_t>(color_t(0, 0, 1));
	auto material_right = std::make_shared<lambertian_t>(color_t(1, 0, 0));

	world.add(std::make_shared<sphere_t>(point_t(-R, 0, -1), R, material_left));
	world.add(std::make_shared<sphere_t>(point_t(R, 0, -1), R, material_right));
#endif

	auto material_ground = std::make_shared<lambertian_t>(color_t(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian_t>(color_t(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<dielectric_t>(1.5);
	auto material_right = std::make_shared<metal_t>(color_t(0.8, 0.6, 0.2));

	world.add(std::make_shared<sphere_t>(point_t(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<sphere_t>(point_t(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(std::make_shared<sphere_t>(point_t(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(std::make_shared<sphere_t>(point_t(-1.0, 0.0, -1.0), -0.45, material_left));
	world.add(std::make_shared<sphere_t>(point_t(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera

	camera_t camera(aspect_ratio, 90.0f, point_t(-2, 2, 1), point_t(0, 0, -1));

	std::ofstream image;
	image.open("new.ppm");
	image << "P3\n" << image_width << " " << image_height << "\n255\n";

	// camera_t camera;
	int samples_per_pixel = 100;
	int max_depth = 50;

	for (int row = image_height - 1; row >= 0; row--) {
		for (int col = 0; col < image_width; col++) {
			std::cerr << "scanning row " << row << " and col " << col << "\n" << std::flush;

			color_t color(0, 0, 0);
			for (int i = 0; i < samples_per_pixel; i++) {
				float y = (row + random_float()) / (image_height - 1);
				float x = (col + random_float()) / (image_width - 1);

				ray_t ray = camera.get_ray(x, y);
				color_t sample_color = get_ray_color(ray, world, max_depth);
				color += sample_color;
			}

			write_color(image, color, samples_per_pixel);
		}
	}
	image.close();

}

void write_color(std::ostream& out, const color_t& color, int samples_per_pixel) {
	out << static_cast<int>(256 * clamp(sqrt(color.x / samples_per_pixel), 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(sqrt(color.y / samples_per_pixel), 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(sqrt(color.z / samples_per_pixel), 0.0f, 0.999f)) << '\n';
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