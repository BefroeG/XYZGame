#pragma once

#include <functional>

class Timer {

public:
	Timer() = default;
	~Timer() = default;

	// 重置定时器状态
	void restart() {
		pass_time = 0;
		shotted = false;
	}

	//设置定时器等待时间
	void set_wait_time(float val) {
		wait_time = val;
	}

	//是否单次触发
	void set_one_shot(bool flag) {
		one_shot = flag;
	}

	//设置回调函数
	void set_on_timeout(std::function<void()> on_timeout) {
		this->on_timeout = on_timeout;
	}

	//暂停计时器
	void pause() {
		paused = true;
	}

	//继续计时
	void resume() {
		paused = false;
	}

	void on_update(float delta) {
		if (paused) return;
		pass_time += delta; //累加帧更新实际过去的时间
		if (pass_time >= wait_time) {
			//如果 不是单次触发 或者 单次触发并且没有被触发
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;
			if (can_shot && on_timeout) {
				on_timeout();
			}
			pass_time -= wait_time;
		}
	}
private:
	float pass_time = 0;   //已过时间
	float wait_time = 0;   //等待时间
	bool paused = false;   //是否暂停
	bool shotted = false;  //是否触发
	bool one_shot = false; //单次触发
	std::function<void()> on_timeout;
};