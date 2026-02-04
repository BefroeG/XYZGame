#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_image/SDL_image.h>

#include <string>
#include <unordered_map>
#include <filesystem>
#include <iostream>

class ResMgr {

public:
	static ResMgr* instance();

	void load(SDL_Renderer* renderer, MIX_Mixer* mixer);
	SDL_Texture* find_texture(const std::string& name, bool is_reverse = false);
	MIX_Audio* find_audio(const std::string& name);

private:
	ResMgr();
	~ResMgr();

private:
	SDL_Renderer* renderer;
	MIX_Mixer* mixer;
	static ResMgr* manager;
	//std::unordered_map<std::string, SDL_Texture*> texture_pool;
	std::unordered_map<std::string, SDL_Surface*> surface_pool;
	std::unordered_map<std::string, MIX_Audio*>   audio_pool;


};