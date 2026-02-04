#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>


int main() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	MIX_Init();
	TTF_Init();
	//SDL_Window*   window   = SDL_CreateWindow(u8"XYZ-Game", 1280, 720, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_Window* window = SDL_CreateWindow(u8"XYZ-Game", 1280, 720, SDL_WINDOW_BORDERLESS);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);


	SDL_Texture* bg_tex_img = IMG_LoadTexture(renderer, "resources/img/background.png");

	SDL_Texture* weapon_tex_img = IMG_LoadTexture(renderer, "resources/img/weapon/weapons3_136.png");
	SDL_FRect weapon_rect = { 300.0f, 200.0f, (float)weapon_tex_img->w, (float)weapon_tex_img->h };

	SDL_Texture* player_tex_img = IMG_LoadTexture(renderer, "resources/img/player/paladin/paladin_7_0.png");
	SDL_FRect player_right_rect = { 400.0f, 400.0f, (float)player_tex_img->w, (float)player_tex_img->h };
	SDL_FRect player_left_rect = { 500.0f, 400.0f, (float)player_tex_img->w, (float)player_tex_img->h };


	MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr); 
	MIX_Audio* bgm_audio = MIX_LoadAudio(mixer, "resources/bgm/bgm_3High.wav", false);
	MIX_PlayAudio(mixer, bgm_audio);

	TTF_Font* font = TTF_OpenFont("resources/font/wqy-zenhei.ttc",32);
	SDL_Color color = { 255, 255, 255 ,255 };
	SDL_Surface * ttf_suf_img = TTF_RenderText_Blended(font, u8"Q骑士", 0, color);
	SDL_Texture*  ttf_tex_img = SDL_CreateTextureFromSurface(renderer, ttf_suf_img);
	SDL_FRect ttf_rect = { 20.0f, 20.0f, (float)ttf_suf_img->w, (float)ttf_suf_img->h };
	SDL_DestroySurface(ttf_suf_img);ttf_suf_img = nullptr;
	

	SDL_Point pos_cursor = { 0,0 };
	SDL_Event event;

	bool is_quit = false;
	while (!is_quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_MOUSE_MOTION) {
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
				//pos_cursor.x = (float)event.motion.x;
				//pos_cursor.y = (float)event.motion.y;

				SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "event.motion=%d %d", event.motion.x, event.motion.y);
			}
		}
		//处理数据


		//渲染绘图
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //执行清屏
		SDL_RenderClear(renderer); //清空渲染器的窗口
		SDL_RenderTexture(renderer, bg_tex_img, nullptr, nullptr);
		SDL_RenderTexture(renderer, ttf_tex_img, nullptr, &ttf_rect);
		SDL_RenderTextureRotated(renderer, weapon_tex_img,
			nullptr,           
			&weapon_rect,      
			45,                //旋转角度
			nullptr,           
			SDL_FLIP_NONE); 

		SDL_RenderTexture(renderer, player_tex_img, nullptr, &player_right_rect);
		SDL_RenderTextureRotated(renderer, player_tex_img,
			nullptr,             // 渲染整个纹理
			&player_left_rect,   // 目标位置
			0,                   // 旋转角度
			nullptr,             // 绕纹理中心旋转
			SDL_FLIP_HORIZONTAL);//水平翻转

		SDL_RenderPresent(renderer);
	}


	TTF_Quit();
	MIX_Quit();
	//IMG_Quit();
	SDL_Quit();

	return 0;
}