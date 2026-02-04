#pragma once

#include <algorithm>
#include "collision_box.h"
#include "camera.h"

class CollisionManager {
public:
	static CollisionManager* instance();

	CollisionBox* create_collision_box();

	void destroy_collision_box(CollisionBox* collison_box);

	void process_collide();

	void on_debug_render(const Camera& camera);
private:
	static CollisionManager* manager;

	std::vector<CollisionBox*> collision_box_list;
private:
	CollisionManager();

	~CollisionManager();
};