#pragma once
#include <SDL3/SDL.h>
#include "Vector2.h"
#include "atlas.h"

#include <sstream>

#include "res_mgr.h"
#include "camera.h"

class Animation {
public:
	// 锚点
	enum class AnchorMode {
		Centered,
		BottomCenterd
	};
private:
	// 动画帧 可以对动画进行裁剪
	struct Frame {
		SDL_FRect rect_src;
		SDL_Texture* texture = nullptr;

		Frame() = default;
		Frame(SDL_Texture* texture, const SDL_FRect& rect_src)
			:texture(texture), rect_src(rect_src) {
		}
		~Frame() = default;
	};

private:
	Vector2 position;
	double angle = 0;
	SDL_FPoint center = { 0 };

	Timer timer;
	bool is_loop = true;
	size_t idx_frame = 0;
	std::vector<Frame> frame_list;
	std::function<void()> on_finished;
	AnchorMode anchor_mode = AnchorMode::Centered;
public:
	Animation();

	~Animation() = default;

	void reset();

	void set_anchor_mode(AnchorMode anchor_mode);

	void set_position(const Vector2& position);

	void set_rotation(double angle);

	void set_center(const Vector2& center);

	void set_loop(bool is_loop);

	void set_interval(float interval);

	void set_on_finished(std::function<void()> on_finished);

	void add_frame(SDL_Texture* texture, int num_h);

	void add_frame(Atlas* atlas, bool is_reverse = false);

	void on_update(float delta);

	void on_render(const Camera& camera, bool is_reverse = false) const;
};


