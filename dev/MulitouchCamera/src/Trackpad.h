#pragma once

extern "C" {
	typedef void (*trackpad_rotate_callback_func)(float rotation);
	typedef void (*trackpad_swipe_callback_func)(float delta_x, float delta_y);
	typedef void (*trackpad_magnify_callback_func)(float magnification);
	typedef void (*trackpad_begin_gesture_callback_func)(void);
	typedef void (*trackpad_end_gesture_callback_func)(void);
	typedef void (*trackpad_mouse_move_callback_func)(float x, float y);
	typedef void (*trackpad_mouse_scroll_callback_func)(float delta_x, float delta_y);
	typedef void (*trackpad_mouse_down_callback_func)(float x, float y);
	typedef void (*trackpad_mouse_up_callback_func)(float x, float y);
	typedef void (*trackpad_mouse_dragged_callback_func)(float x, float y, float delta_x, float delta_y);
	
	void trackpad_set_rotate_callback(trackpad_rotate_callback_func);
	void trackpad_set_swipe_callback(trackpad_swipe_callback_func);
	void trackpad_set_magnify_callback(trackpad_magnify_callback_func);
	void trackpad_set_begin_gesture_callback(trackpad_begin_gesture_callback_func);
	void trackpad_set_end_gesture_callback(trackpad_end_gesture_callback_func);
	void trackpad_set_mouse_move_callback(trackpad_mouse_move_callback_func);
	void trackpad_set_mouse_scroll_callback(trackpad_mouse_scroll_callback_func);
	void trackpad_set_left_mouse_down_callback(trackpad_mouse_down_callback_func);
	void trackpad_set_right_mouse_down_callback(trackpad_mouse_down_callback_func);
	void trackpad_set_left_mouse_up_callback(trackpad_mouse_up_callback_func);
	void trackpad_set_right_mouse_up_callback(trackpad_mouse_up_callback_func);
	void trackpad_set_left_mouse_dragged_callback(trackpad_mouse_dragged_callback_func);
	void trackpad_set_right_mouse_dragged_callback(trackpad_mouse_dragged_callback_func);
	
	void trackpad_init();
}