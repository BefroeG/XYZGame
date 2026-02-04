#pragma once
#include "config.h"
#include "vector2.h"
#include "atlas.h"
#include "animation.h"
#include "collision_manager.h"

class Weapon {
private:
	double angle = 0;
	Vector2 position;
	Vector2 size;
	Animation animation_attack_right;
	Animation animation_attack_left;
	bool is_facing_right = true;
	Animation* current_animation;

private:
	CollisionBox* collision_box = nullptr;
public:
	Weapon() {
		size = { 104,104 };
		Atlas attack_right;
		attack_right.load("weapons3_139", 0, 0);
		animation_attack_right.add_frame(&attack_right);
		animation_attack_right.set_interval(0.08f);
		animation_attack_right.set_loop(true);
		animation_attack_right.set_center(Vector2(size.x/2 , size.y/2));
		//animation_attack.set_anchor_mode(Animation::AnchorMode::BottomCenterd);

		Atlas attack_left;
		attack_left.load("weapons3_139", 0, 0, true);
		animation_attack_left.add_frame(&attack_left);
		animation_attack_left.set_interval(0.08f);
		animation_attack_left.set_loop(true);
		animation_attack_left.set_center(Vector2(size.x / 2, size.y / 2));

		current_animation = &animation_attack_right;
	}
	~Weapon()= default;

	void set_position(Vector2 position) {
		this->position = position;
	}

	const Vector2& get_size() const{
		return size;
	}

	void set_angle(double angle) {
		this->angle = angle;
	}

	const Vector2& get_position() const{
		return position;
	}

	const double get_angle() const{
		return angle;
	}

	void set_is_facing_right(bool is_right) {
		is_facing_right = is_right;
	}

	void on_update(float delta) {
		if (is_facing_right) {
			current_animation = &animation_attack_right;
		}
		if(!is_facing_right)
			current_animation = &animation_attack_left;
		//SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "on_update delta=%d", delta);
		current_animation->set_position(get_position());
		current_animation->on_update(delta);
	}

	void on_render(const Camera& camera) const {
		current_animation->set_rotation(angle);
		current_animation->on_render(camera);
	}
};
