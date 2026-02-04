#include "animation.h"

Animation::Animation() {
	timer.set_one_shot(false);// 设置为循环触发
	timer.set_on_timeout(
		[&]() {
			idx_frame++;
			if (idx_frame >= frame_list.size()) {
				//重置帧索引到列表头部
				idx_frame = is_loop ? 0 : frame_list.size() - 1;
				if (!is_loop && on_finished)
					// 不是循环播放 并且存在回调函数
					on_finished();
			}
		});
}

void Animation::reset() {
	timer.restart();
	idx_frame = 0;
}

void Animation::set_anchor_mode(AnchorMode anchor_mode) {
	this->anchor_mode = anchor_mode;
}

void Animation::set_position(const Vector2& position) {
	this->position = position;
}

void Animation::set_rotation(double angle) {
	this->angle = angle;
}

void Animation::set_center(const Vector2& center) {
	this->center.x = center.x;
	this->center.y = center.y;
}

void Animation::set_loop(bool is_loop) {
	this->is_loop = is_loop;
}

void Animation::set_interval(float interval) {
	timer.set_wait_time(interval);
}

void Animation::set_on_finished(std::function<void()> on_finished) {
	this->on_finished = on_finished;
}

void Animation::add_frame(SDL_Texture* texture, int num_h) {
	//一张大图片分解成帧

}

void Animation::add_frame(Atlas* atlas, bool is_reverse) {
	if (is_reverse) {
		for (int i = atlas->get_size() - 1;i > 0;--i) {
			SDL_Texture* texture = atlas->get_texture(i);
			SDL_FRect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = (float)texture->w, rect_src.h = (float)texture->h;
			frame_list.emplace_back(texture, rect_src);
		}
	}
	else {
		for (int i = 0;i < atlas->get_size();++i) {
			SDL_Texture* texture = atlas->get_texture(i);
			SDL_FRect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = (float)texture->w, rect_src.h = (float)texture->h;
			frame_list.emplace_back(texture, rect_src);
		}
	}
}

void Animation::on_update(float delta) {
	timer.on_update(delta);
}

void Animation::on_render(const Camera& camera, bool is_reverse) const {
	const Frame& frame = frame_list[idx_frame];

	SDL_FRect rect_dst;
	rect_dst.x = position.x - frame.rect_src.w / 2;
	rect_dst.y = (anchor_mode == AnchorMode::Centered) ?
		(position.y - frame.rect_src.h / 2) : (position.y - frame.rect_src.h);
	rect_dst.w = (float)frame.rect_src.w, rect_dst.h = (float)frame.rect_src.h;

	SDL_FlipMode flip = SDL_FlipMode::SDL_FLIP_NONE;
	if (is_reverse)
		flip = SDL_FlipMode::SDL_FLIP_HORIZONTAL;
	camera.render_texture(frame.texture, &frame.rect_src, &rect_dst, angle, &center, flip);

}
