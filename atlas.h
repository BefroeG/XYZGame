#pragma once
#include <SDL3/SDL.h>
#include <sstream>
#include "res_mgr.h"


class Atlas {
public:
	Atlas() = default;

	~Atlas() = default;

	void load(const std::string& name, int start, int end, bool is_reverse = false);

	void clear();

	int get_size();

	SDL_Texture* get_texture(int idx);

	void add_texture(SDL_Texture* texture);

public:
	std::vector<SDL_Texture*> texture_list;
};
