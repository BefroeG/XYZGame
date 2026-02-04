#pragma once
#include "collision_manager.h"
#include "vector2.h"

class Wall {
private:
	Vector2 size;
	Vector2 position;
public:
	Wall(Vector2 size, Vector2 position) : size(size), position(position){
		collision_box = CollisionManager::instance()->create_collision_box();
		collision_box->set_layer_src(CollisionLayer::None);
		collision_box->set_layer_dst(CollisionLayer::Player);
		collision_box->set_size(size);
		collision_box->set_position(position);
		//collision_box->set_on_collide([&]() {
		//	SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "Wall collision_box->set_on_collide");
		//});
	}
private:
	CollisionBox* collision_box = nullptr;
 
};
