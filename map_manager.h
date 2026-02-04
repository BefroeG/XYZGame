#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <sstream>
#include <random>

#include "wall.h"
#include "res_mgr.h"

class MapManager {
public:
	static MapManager* instance();
	SDL_Texture* create_map(SDL_Renderer* renderer, int width_num, int height_num);

private:
	static MapManager* manager;

private:
	MapManager();
	~MapManager();
};