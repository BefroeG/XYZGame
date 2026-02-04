#define SDL_MAIN_HANDLED 

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "map_manager.h"
#include "res_mgr.h"
#include "camera.h"
#include "atlas.h"
#include "animation.h"
#include "player.h"
#include "bullet.h"

//全局摄像机
Camera* camera = nullptr;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
MIX_Mixer* mixer = nullptr;

bool is_quit = false;

int FPS = 144;

Vector2 pos_cursor = { 0,0 };

Player* player = nullptr;

std::vector<Bullet> bullet_list;
double angle_bullet;
Vector2 bullet_v;

bool is_fire_key_down = false;

SDL_Texture* bg_tex_img = nullptr;

void init();
void mainloop();
void deinit();

void on_update(float delta);
void on_render(const Camera& camera);

int main() {
	init();
	mainloop();
	deinit();
	return 0;
}

void init() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	MIX_Init();
	TTF_Init();
	// SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP SDL_WINDOW_RESIZABLE );
	window = SDL_CreateWindow(u8"XYZ-Game Q骑士", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	//SDL_Window* window = SDL_CreateWindow(u8"XYZ-Game", 1280, 720, SDL_WINDOWPOS_CENTERED);
	renderer = SDL_CreateRenderer(window, nullptr);
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

	ResMgr::instance()->load(renderer, mixer);

	camera = new Camera(renderer);
	camera->set_size(Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));

	MIX_Audio* bgm_audio = ResMgr::instance()->find_audio("bgm_3High");
	//MIX_PlayAudio(mixer, bgm_audio);

	bg_tex_img = MapManager::instance()->create_map(renderer, 30, 30);

	SDL_Surface* cursor_surface = IMG_Load("resources/crosshair.png");
	SDL_Cursor* cursor = SDL_CreateColorCursor(cursor_surface, 0, 0);
	SDL_SetCursor(cursor);

}

void deinit() {
	delete camera;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	MIX_Quit();
	SDL_Quit();
}

void mainloop(){
	player = new Player();

	using namespace std::chrono;
	const nanoseconds frame_duration(1000000000 / FPS);// FPS 每一帧理论上应该占用的目标时间
	steady_clock::time_point last_tick = steady_clock::now();

	SDL_Event event;
	int frame_count = 0; // 帧计数器
	while (!is_quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					is_quit = true;
					break;
				case SDL_EVENT_KEY_DOWN: {
					player->on_input(event);
					break;
				}
				case SDL_EVENT_KEY_UP: {
					player->on_input(event);
					break;
				}
				case SDL_EVENT_MOUSE_MOTION: {
					pos_cursor.x = event.motion.x;
					pos_cursor.y = event.motion.y;
					player->on_input(event);
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN: {

					is_fire_key_down = true;
					player->on_input(event);
					//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "is_fire_key_down = true");
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP: {
					is_fire_key_down = false;
					player->on_input(event);
					//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "is_fire_key_down = false");
					break;
				}
			}
		}
		steady_clock::time_point frame_start = steady_clock::now();
		frame_count++;
		
		duration<float> delta = duration<float>(frame_start - last_tick);

		on_update(delta.count());

		on_render(*camera);

		SDL_RenderPresent(renderer);

		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0)) {
			std::this_thread::sleep_for(sleep_duration);
		}

		if (frame_count % 50 == 0) { 
			auto total_frame_duration = steady_clock::now() - frame_start;
			float current_fps = 1000000.0 / duration_cast<microseconds>(total_frame_duration).count();
			//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, u8"第%d帧 | FPS = %f", frame_count, current_fps);
		}
	}
}

void on_update(float delta) {
	//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, " player->get_position() =%f %f", player->get_position().x, player->get_position().y);
	player->on_update(delta);

	CollisionManager::instance()->process_collide();
	//处理数据

	if (is_fire_key_down) {
		Vector2 direction = pos_cursor - Vector2(WINDOW_WIDTH, WINDOW_HEIGHT) / 2;
		if (direction.length() > 0) {
			bullet_v = direction.normalize();
		}
		//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, " pos_cursor =%f %f", pos_cursor.x, pos_cursor.y);
		//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, " player->get_position() =%f %f", player->get_position().x, player->get_position().y);
		bullet_list.emplace_back(player->get_position(),bullet_v);
	}
	
	for (Bullet& bullet : bullet_list) {
		bullet.on_update(delta);
	}

	bullet_list.erase(std::remove_if(
		bullet_list.begin(), bullet_list.end(),
		[](const Bullet& bullet) {
			return bullet.can_remove();
		}), bullet_list.end());

	camera->look_at(player->get_position());
}
//渲染绘图
void on_render(const Camera& camera) {
	SDL_SetRenderDrawColor(renderer, 8, 27, 50, 100); //背景色 //执行清屏
	SDL_RenderClear(renderer);                      //清空渲染器的窗口

	// 绘制背景图
	SDL_FRect bg_rect = { 0.0f, 0.0f, (float)bg_tex_img->w, (float)bg_tex_img->h };
	camera.render_texture(bg_tex_img, nullptr, &bg_rect, 0, nullptr);
	
	player->on_render(camera);

	for (const Bullet& bullet : bullet_list) {
		bullet.on_render(camera);
	}

	CollisionManager::instance()->on_debug_render(camera);


}

