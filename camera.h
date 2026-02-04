#pragma once

#include "timer.h"
#include "vector2.h"

#include <SDL3/SDL.h>

class Camera {
public:
	Camera(SDL_Renderer* renderer):renderer(renderer) {
		timer_shake.set_one_shot(true);
		timer_shake.set_on_timeout([&]() {
			is_shaking = false;
			reset();
		});
	}

	~Camera() = default;

	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

	void on_update(float delta) {
		timer_shake.on_update(delta);

		if (is_shaking) {
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void render_texture(SDL_Texture* texture, const SDL_FRect* rect_src,
		const SDL_FRect* rect_dst, double angle = 0, const SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FlipMode::SDL_FLIP_NONE) const {
		SDL_FRect rect_dst_win = *rect_dst;
		rect_dst_win.x -= position.x;
		rect_dst_win.y -= position.y;

		SDL_RenderTextureRotated(renderer, texture,
			rect_src, &rect_dst_win,
			angle, center,
			flip);
	}

	void render_FRect(SDL_FRect* rect) const {
		SDL_FRect rect_dst_win = *rect;
		rect_dst_win.x = rect_dst_win.x - position.x - rect->w / 2;
		rect_dst_win.y = rect_dst_win.y - position.y - rect->h / 2;
		SDL_SetRenderDrawColor(renderer, 115, 115, 175, 100);
		SDL_RenderRect(renderer, &rect_dst_win);
	}

	void set_size(const Vector2 size) {
		this->size = size;
	}

	const Vector2& get_size() const {
		return size;
	}

	void set_position(const Vector2 position) {
		this->position = position;
	}

	const Vector2& get_postion() const {
		return position;
	}

	void look_at(const Vector2& target) {
		position = target - size / 2.0f;
	}

	void reset() {
		position.x = 0;
		position.y = 0;
	}
private:
	Vector2 size;
	
	Timer timer_shake;            //抖动定时器
	bool is_shaking = false;      //是否抖动
	float shaking_strength = 0;   //摄像机抖动幅度
	SDL_Renderer* renderer = nullptr;
public:
	mutable Vector2 position;             //摄像机位置
};