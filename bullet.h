#pragma once

#include "config.h"
#include "vector2.h"
#include "atlas.h"
#include "animation.h"
#include "collision_manager.h"

class Bullet {
private:
	double angle = 0;
	Vector2 position;
	Vector2 last_safe_position;
	Vector2 size;
	Vector2 velocity;
	bool is_valid = true;	

	Animation animation_shoot;
	float speed = 600.0f;
private:
	CollisionBox* collision_box = nullptr;
private:
	bool is_colliding = false;
	double max_alive_time = 0.8f;
	double alive_time = 0.0f;
public:
	Bullet(Vector2 position, Vector2 velocity) {
		//this->angle = angle;
		//double radians = angle * 3.14159265 / 180;
		//velocity.x = (float)std::cos(radians) * speed;
		//velocity.y = (float)std::sin(radians) * speed;
		this->velocity = velocity;
		this->position = position;

		Atlas atlas_shoot;
		atlas_shoot.load("bullet_37", 0, 0);
		animation_shoot.add_frame(&atlas_shoot);
		animation_shoot.set_interval(0.08f);
		animation_shoot.set_loop(true);

		size.x = 15, size.y = 15;

		collision_box = CollisionManager::instance()->create_collision_box();
		collision_box->set_layer_src(CollisionLayer::Player);
		collision_box->set_layer_dst(CollisionLayer::Background);
		collision_box->set_size(size);
		collision_box->set_on_collide([this]() {
			//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "Bullet->set_on_collide");
			is_colliding = true;
		});
	}

	~Bullet() = default;

	void set_position(const Vector2 position) {
		this->position = position;
	}

	const Vector2& get_position() const {
		return position;
	}

	void on_update(float delta) {
		last_safe_position = position;
		position += velocity * speed * delta;

		///SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "position=%f, %f | delta= %f", position.x, position.y, delta);
		alive_time += delta;

		if (alive_time >= max_alive_time) {
			is_valid = false;
			collision_box->is_valid = false;
		}

		//if (is_colliding) {
		//	position = last_safe_position;
		//	//is_valid = false;
		//	//collision_box->is_valid = false;
		//}

		animation_shoot.set_position(position);
		collision_box->set_position(position);
		

		//if (alive_time >= max_alive_time) {
		//	is_valid = false;
		//	collision_box->is_valid = false;
		//}
		//else {
			//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "position=%f, %f | delta= %f", position.x, position.y, delta);
		//}
		//if (is_colliding) {
		//	is_valid = false;
		//	collision_box->is_valid = false;
		//}
	}

	void on_render(const Camera& camera) const {
		animation_shoot.on_render(camera);
	}

	void on_hit() {
		is_valid = false;
	}

	bool can_remove() const {
		return !is_valid;
	}

};

