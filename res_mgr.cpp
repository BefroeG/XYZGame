#include "res_mgr.h"

ResMgr* ResMgr::manager = nullptr;

ResMgr* ResMgr::instance() {
	if (!manager)
		manager = new ResMgr();
	return manager;
}

void ResMgr::load(SDL_Renderer* renderer, MIX_Mixer* mixer) {
	this->renderer = renderer;
	this->mixer = mixer;
	using namespace std::filesystem;
	for (const auto& entry : recursive_directory_iterator("resources")) {
		if (entry.is_regular_file()) {
			const path& path = entry.path();
			if (path.extension() == ".png") {
				//SDL_Texture* texture = IMG_LoadTexture(renderer, path.u8string().c_str());
				SDL_Surface* surface = SDL_LoadPNG(path.u8string().c_str());
				std::cout << path.stem().u8string()<< std::endl;
				//texture_pool[path.stem().u8string()] = texture;
				surface_pool[path.stem().u8string()] = surface;
			}
			else if (path.extension() == ".wav") {
				MIX_Audio* bgm_audio = MIX_LoadAudio(mixer, path.u8string().c_str(), false);
				std::cout << path.stem().u8string() << std::endl;
				audio_pool[path.stem().u8string()] = bgm_audio;
			}
		}
	}
}

SDL_Texture* ResMgr::find_texture(const std::string& name, bool is_reverse) {
	SDL_Surface* src_surface = surface_pool[name];
	SDL_Texture* texture;
	if (!src_surface) return nullptr;
	if (!is_reverse) {
		texture = SDL_CreateTextureFromSurface(renderer, src_surface);
		
	}
	else {
		SDL_Surface* dst_surface = SDL_CreateSurface(src_surface->w, src_surface->h, src_surface->format);
		int bpp = SDL_BYTESPERPIXEL(src_surface->format);
		for (int y = 0; y < src_surface->h; y++) {
			for (int x = 0; x < src_surface->w; x++) {
				// 计算源像素位置和目标像素位置
				Uint8* srcPixel = (Uint8*)src_surface->pixels + y * src_surface->pitch + x * bpp;
				Uint8* dstPixel = (Uint8*)dst_surface->pixels + y * dst_surface->pitch + (dst_surface->w - 1 - x) * bpp;

				// 复制像素
				memcpy(dstPixel, srcPixel, bpp);
			}
		}
		texture = SDL_CreateTextureFromSurface(renderer, dst_surface);
	}
	return texture;
}
MIX_Audio* ResMgr::find_audio(const std::string& name) {
	return audio_pool[name];
}

ResMgr::ResMgr() = default;
ResMgr::~ResMgr() = default;