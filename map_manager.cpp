#include "map_manager.h"


MapManager* MapManager::manager = nullptr;

MapManager* MapManager::instance() {
	if (!manager)
		manager = new MapManager();
	return manager;
}

// 光源结构
typedef struct {
	float x, y;           // 位置
	float radius;         // 半径
	float intensity;      // 强度
	SDL_Color color;      // 颜色
	float flicker_speed;  // 闪烁速度
	float flicker_time;   // 闪烁时间
	SDL_Texture* texture; // 光源纹理
} LightSource;

SDL_Texture* MapManager::create_map(SDL_Renderer* renderer, int width_num, int height_num) {
	// 创建大纹理
	SDL_Texture* bigTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, width_num * 64, (height_num+1) * 64);

	// 保存原始渲染目标
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

	// 设置大纹理为渲染目标
	SDL_SetRenderTarget(renderer, bigTexture);

	// 清空大纹理
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100); //背景色
	SDL_RenderClear(renderer);

	for (int w = 0;w < width_num;++w) {
		for (int h = 0;h < height_num;++h) {
			SDL_FRect dest1 = { w * 64, h * 64 + 32, 64, 64 };  // 位置1
			std::stringstream ss;  //#include <sstream>
			ss << "f60" << (rand() % 6 + 1);
			SDL_Texture* samllTexture = ResMgr::instance()->find_texture(ss.str());
			SDL_RenderTexture(renderer, samllTexture, NULL, &dest1);
		}
	}
	int ran_num = 0;
	for (int w = 1;w < width_num-1;++w) {
		std::stringstream ss;
		ran_num = rand() % 6;
		
		Wall wall_top(Vector2(64,64), Vector2(w * 64, 0));

		SDL_FRect wall_top_up = { w * 64, 0, 64, 96 };
		ss << "w60" << (ran_num > 4 ? "2" : "4");
		SDL_Texture* wall_top_up_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_top_up_tex, NULL, &wall_top_up);
		ss.str("");

		SDL_FRect wall_top_down = { w * 64, 0, 64, 96 };
		ss << "w60" << (ran_num < 4 ? "1" : "3");
		SDL_Texture* wall_top_down_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_top_down_tex, NULL, &wall_top_down);
		ss.str("");

		ran_num = rand() % 6;
		Wall wall_bottom(Vector2(64, 64), Vector2(w * 64, (height_num-1) * 64));

		SDL_FRect wall_bottom_up = { w * 64, (height_num - 1) * 64, 64, 96 };
		ss << "w60" << (ran_num > 4  ? "2" : "4");
		SDL_Texture* wall_bottom_up_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_bottom_up_tex, NULL, &wall_bottom_up);
		ss.str("");

		SDL_FRect wall_bottom_down = { w * 64, (height_num - 1) * 64, 64, 96 };
		ss << "w60" << (ran_num < 4 ? "1" : "3");
		SDL_Texture* wall_bottom_down_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_bottom_down_tex, NULL, &wall_bottom_down);
		ss.str("");
	}
	
	for (int h = 0;h < height_num;++h) {
		ran_num = rand() % 8;
		std::stringstream ss;

		Wall wall_left(Vector2(64, 64), Vector2(0, h * 64));

		SDL_FRect wall_left_up = {0, 64 * h ,64, 96 };
		ss << "w60" << (ran_num >5 ? "2" : "4");
		SDL_Texture* wall_left_up_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_left_up_tex, NULL, &wall_left_up);
		ss.str("");
		if (h == height_num - 1) {
			SDL_FRect wall_left_down = { 0, 64 * h ,64, 96 };
			ss << "w60" << (ran_num > 5 ? "1" : "3");
			SDL_Texture* wall_left_down_tex = ResMgr::instance()->find_texture(ss.str());
			SDL_RenderTexture(renderer, wall_left_down_tex, NULL, &wall_left_down);
			ss.str("");
		}

		ran_num = rand() % 8;
		Wall wall_right(Vector2(64, 64), Vector2((width_num-1) * 64, h * 64));

		SDL_FRect wall_right_up = { (width_num - 1) * 64, h * 64, 64, 96 };
		ss << "w60" << (ran_num > 5 ? "2" : "4");
		SDL_Texture* wall_right_up_tex = ResMgr::instance()->find_texture(ss.str());
		SDL_RenderTexture(renderer, wall_right_up_tex, NULL, &wall_right_up);
		ss.str("");

		if (h == height_num - 1) {
			SDL_FRect wall_right_down = { (width_num - 1) * 64, h * 64, 64, 96 };
			ss << "w60" << (ran_num >5 ? "1" : "3");
			SDL_Texture* wall_right_down_tex = ResMgr::instance()->find_texture(ss.str());
			SDL_RenderTexture(renderer, wall_right_down_tex, NULL, &wall_right_down);
			ss.str("");
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1); // 半透黑色
	SDL_FRect shadow_top = { 64, 96, 64 * (width_num-2), 32};

	SDL_RenderFillRect(renderer, &shadow_top);
	
	// 恢复原始渲染目标
	SDL_SetRenderTarget(renderer, oldTarget);


	/*
	
	// 创建中光源纹理（橙色，100像素半径）
	SDL_Color medium_color = { 255, 180, 100, 255 };
	SDL_Texture*  medium_light_texture = create_light_texture(renderer, 100, medium_color);


	LightSource light = { 0 };
	light.radius = 100;
	light.intensity = 1.0f;
	light.color = (SDL_Color){ 255, 180, 100, 255 }; // 橙色
	light.flicker_speed = 3.0f + (rand() % 100) / 30.0f;
	light.texture = medium_light_texture;

	light.flicker_time = (rand() % 1000) / 1000.0f * 6.283185f; // 随机相位
	
	
	*/

	return bigTexture;
}


// 创建圆形渐变纹理
SDL_Texture* create_light_texture(SDL_Renderer* renderer, int radius, SDL_Color color) {
	int size = radius * 2;
	SDL_Texture* texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET,
		size, size);

	if (!texture) {
		printf("创建纹理失败: %s\n", SDL_GetError());
		return NULL;
	}

	// 设置纹理混合模式为加法混合
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

	// 保存当前渲染目标
	SDL_Texture* old_target = SDL_GetRenderTarget(renderer);
	if (!SDL_SetRenderTarget(renderer, texture)) {
		printf("设置渲染目标失败: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_SetRenderTarget(renderer, old_target);
		return NULL;
	}

	// 清除为透明
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// 设置绘制混合模式为加法
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

	// 绘制径向渐变圆（从中心向外衰减）
	for (int r = radius; r > 0; r -= 2) {
		// 计算透明度（中心最亮，边缘最暗）
		float progress = 1.0f - (float)r / radius;
		// 使用平滑的衰减曲线
		float alpha_factor = 1.0f - (progress * progress);
		Uint8 alpha = (Uint8)(255 * alpha_factor);

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		// 绘制填充圆（用矩形近似）
		SDL_FRect rect = {
			(float)(radius - r),
			(float)(radius - r),
			(float)(r * 2),
			(float)(r * 2)
		};
		SDL_RenderFillRect(renderer, &rect);
	}

	// 恢复设置
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(renderer, old_target);

	return texture;
}



MapManager::MapManager() = default;

MapManager::~MapManager() = default;
