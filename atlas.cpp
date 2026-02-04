#include "atlas.h"

void Atlas::load(const std::string& name, int start, int end, bool is_reverse) {
	texture_list.clear();
	int size = end - start + 1;
	texture_list.resize(size);
	if (size == 1) {
		texture_list[0] =ResMgr::instance()->find_texture(name, is_reverse);
	}
	else {
		for (int i = 0;i < size;++i) {
			std::stringstream ss;  //#include <sstream>
			ss << name << "_" << (start + i);
			texture_list[i] = ResMgr::instance()->find_texture(ss.str() , is_reverse);
		}
	}
}

void Atlas::clear() {
	texture_list.clear();
}

int Atlas::get_size() {
	return (int)texture_list.size();
}

SDL_Texture* Atlas::get_texture(int idx) {
	if (idx < 0 || idx >= texture_list.size()) return nullptr;
	return texture_list[idx];
}

void Atlas::add_texture(SDL_Texture* texture) {
	texture_list.push_back(texture);
}