#pragma once

#include "vector2.h"
#include "config.h"
#include "weapon.h"
#include "collision_manager.h"

class Player {
private:
	const float run_velocity  = 1500.0f;        // player移动速度
	const float walk_velocity = 300.0f;        // player移动速度
	const int SHADOW_WIDTH = 65;             // player阴影宽度

protected:
	Vector2 position;       // 角色位置
	Vector2 last_safe_position;
	Vector2 velocity;		// 角色速度
	Vector2 size;           // 角色尺寸
private:         
	Animation animation_run_right; 
	Animation animation_back_run_right;
	Animation animation_idle_right;

	Animation animation_run_left;
	Animation animation_back_run_left;
	Animation animation_idle_left;

	Animation* current_animation = nullptr;
private:
	Weapon* weapon = nullptr;
	double weapon_angle = 0;
	double mouse_angle = 0;
private:
	// 移动控制
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

	bool is_attack = false;
	bool is_facing_right = true;
	bool is_colliding = false;
private:
	CollisionBox* collision_box = nullptr;

public:
	Player() {
		size = { 156, 160 };
		position = { 200,200 };
		Atlas idle_right;
		idle_right.load("knight_22", 0, 7);
		animation_idle_right.add_frame(&idle_right);
		animation_idle_right.set_interval(0.10f);
		animation_idle_right.set_loop(true);

		Atlas idle_left;
		idle_left.load("knight_22", 0, 7, true);
		animation_idle_left.add_frame(&idle_left);
		animation_idle_left.set_interval(0.10f);
		animation_idle_left.set_loop(true);

		Atlas run_right;
		run_right.load("knight_22", 8, 15);
		animation_run_right.add_frame(&run_right);
		animation_run_right.set_interval(0.06f);
		animation_run_right.set_loop(true);

		Atlas run_left;
		run_left.load("knight_22", 8, 15, true);
		animation_run_left.add_frame(&run_left);
		animation_run_left.set_interval(0.06f);
		animation_run_left.set_loop(true);

		animation_back_run_right.add_frame(&run_right,true);
		animation_back_run_right.set_interval(0.12f);
		animation_back_run_right.set_loop(true);

		animation_back_run_left.add_frame(&run_left, true);
		animation_back_run_left.set_interval(0.12f);
		animation_back_run_left.set_loop(true);

		current_animation = &animation_idle_right;

		weapon = new Weapon();
		weapon->set_is_facing_right(is_facing_right);
		weapon->set_position(get_position());

		collision_box = CollisionManager::instance()->create_collision_box();
		collision_box->set_layer_src(CollisionLayer::Player);
		collision_box->set_layer_dst(CollisionLayer::Background);
		collision_box->set_size(size);
		collision_box->set_on_collide([this](){
			//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "collision_box->set_on_collide");
			is_colliding = true;
		});
	}
	~Player() = default;

	void on_input(const SDL_Event& event) {
		if (event.type == SDL_EVENT_KEY_DOWN) {
			//std::cout << event.type << std::endl;
			switch (event.key.key) {
			case SDLK_W:
				is_move_up = true;
				break;
			case SDLK_S:
				is_move_down = true;
				break;
			case SDLK_A:
				is_move_left = true;
				break;
			case SDLK_D:
				is_move_right = true;
				break;
			}
		}
		else if (event.type == SDL_EVENT_KEY_UP) {
			//std::cout << event.type << std::endl;
			switch (event.key.key) {
			case SDLK_W:
				is_move_up = false;
				break;
			case SDLK_S:
				is_move_down = false;
				break;
			case SDLK_A:
				is_move_left = false;
				break;
			case SDLK_D:
				is_move_right = false;
				break;
			}
		}
		else if (event.type == SDL_EVENT_MOUSE_MOTION) {
			mouse_angle = std::atan2(event.motion.y - WINDOW_HEIGHT / 2,
				event.motion.x - WINDOW_WIDTH / 2) * 180 / 3.14159265;
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			is_attack = true;
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
			is_attack = false;
		}
	}

	void on_update(float delta) {
		last_safe_position = position;
		Vector2 direction;
		direction.x = is_move_right - is_move_left;
		direction.y = is_move_down - is_move_up;

		if (direction.length() != 0) {
			if (direction.x == 0 || (is_facing_right && direction.x > 0) || (!is_facing_right && direction.x < 0)) {
				position += direction.normalize() * run_velocity * delta;
				current_animation = &animation_run_right;
			}
			else {
				position += direction.normalize() * walk_velocity * delta;
				current_animation = &animation_back_run_right;
			}
			if (direction.x > 0) {
				is_facing_right = true;
				current_animation = &animation_run_right;
			}
			if (direction.x < 0) {
				is_facing_right = false;
				current_animation = &animation_run_left;
			}	
		}
		else {
			if(is_facing_right)
				current_animation = &animation_idle_right;
			if(!is_facing_right)
				current_animation = &animation_idle_left;
		}

		if (is_facing_right) {
			weapon_angle = mouse_angle - 45;
			//TODO:
		}
		else {
			weapon_angle = mouse_angle - 135;
			float mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			//TODO:
			SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "mouse_xy=%f | %f", mouse_x, mouse_y);
			//weapon_angle = mouse_angle + 45;
			//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "weapon_angle=%f", weapon_angle);
			//if (weapon_angle < -45) weapon_angle = -45;

		}
		current_animation->set_position(get_position());
		current_animation->on_update(delta);	
		
		Vector2 weapon_position = position - weapon->get_size();
		if(is_facing_right)
			weapon_position = position + Vector2(weapon->get_size().x * 2 / 5.0, weapon->get_size().y * 1/ 5.0);
		else
			weapon_position = position + Vector2(weapon->get_size().x * (-2) / 5.0, weapon->get_size().y * 1 / 5.0);
		weapon->set_is_facing_right(is_facing_right);

		weapon->set_position(weapon_position);
		weapon->set_angle(weapon_angle);
		weapon->on_update(delta);

		collision_box->set_position(position);

		if (is_colliding) {
			position = last_safe_position;
			is_colliding = false;
		}
	}

	void on_render(const Camera& camera) const {
		current_animation->on_render(camera);
		weapon->on_render(camera);
	}

	void set_position(const Vector2& position) {
		this->position = position;
	}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	void on_collision() {

	}

};

