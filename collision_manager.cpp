#include "collision_manager.h"

CollisionManager* CollisionManager::manager = nullptr;

CollisionManager* CollisionManager::instance() {
	if (!manager)
		manager = new CollisionManager();
	return manager;
}
CollisionManager::CollisionManager() = default;

CollisionManager::~CollisionManager() = default;

CollisionBox* CollisionManager::create_collision_box() {
	CollisionBox* collison_box = new CollisionBox();
	collision_box_list.push_back(collison_box);
	return collison_box;
}

void CollisionManager::destroy_collision_box(CollisionBox* collision_box) {
	collision_box_list.erase(std::remove(collision_box_list.begin(),
		collision_box_list.end(), collision_box), collision_box_list.end());
	delete collision_box;
}

void CollisionManager::process_collide() {
	for (CollisionBox* collision_box_src : collision_box_list) {
		//¿ªÆôÅö×² Åö×²²ã¼¶²»Îª¿Õ
		if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None)
			continue;
		for (CollisionBox* collision_box_dst : collision_box_list) {
			if (!collision_box_dst->enabled || collision_box_src == collision_box_dst
				|| collision_box_src->layer_dst != collision_box_dst->layer_src)
				continue;
			bool is_collide_x = 
			(std::max(collision_box_src->position.x + collision_box_src->size.x / 2,
				      collision_box_dst->position.x + collision_box_dst->size.x / 2)
		   - std::min(collision_box_src->position.x - collision_box_src->size.x / 2,
					  collision_box_dst->position.x - collision_box_dst->size.x / 2)) 
				  <= (collision_box_src->size.x + collision_box_dst->size.x);
			bool is_collide_y =
			(std::max(collision_box_src->position.y + collision_box_src->size.y / 2,
					  collision_box_dst->position.y + collision_box_dst->size.y / 2)
		   - std::min(collision_box_src->position.y - collision_box_src->size.y / 2,
					  collision_box_dst->position.y - collision_box_dst->size.y / 2))
				  <= (collision_box_src->size.y + collision_box_dst->size.y);
			if (is_collide_x && is_collide_y && collision_box_dst->on_collide)
				collision_box_dst->on_collide();

		}
	}
}
void CollisionManager::on_debug_render(const Camera& camera) {
	collision_box_list.erase(std::remove_if(
		collision_box_list.begin(), collision_box_list.end(),
		[](CollisionBox* collision_box) {
			return !collision_box->is_valid;
		}), collision_box_list.end());

	for (CollisionBox* collision_box : collision_box_list) {
		//if (collision_box->enabled)
		//	SDL_SetRenderDrawColor(camera.renderer, 255, 195, 195, 100);
		//else
		//	SDL_SetRenderDrawColor(renderer, 115, 115, 175, 100);
		SDL_FRect rect;
		rect.x = collision_box->position.x;
		rect.y = collision_box->position.y;
		rect.w = collision_box->size.x;
		rect.h = collision_box->size.y;
		camera.render_FRect(&rect);
	}
}
