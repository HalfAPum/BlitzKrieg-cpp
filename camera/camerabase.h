//
// Created by o.narvatov on 8/14/2024.
//

#ifndef CAMERABASE_H
#define CAMERABASE_H
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "camerasocket.h"
#include "../Constants.h"

using namespace godot;

class CameraBase : public Node3D {
    GDCLASS(CameraBase, Node3D)
public:
    void _ready() override;
    void _process(double p_delta) override;
    void _unhandled_input(const Ref<InputEvent> &p_event) override;
    void camera_base_move(double delta);
    void camera_zoom_update(real_t delta);
    void camera_automatic_pan(real_t delta);
    void camera_base_rotate(real_t delta);
    void camera_base_rotate_left_right(real_t delta, real_t direction);
    void camera_socket_rotate_x(real_t delta, real_t direction) const;
    void camera_rotate_to_mouse_offsets(real_t delta);
private:
    CameraSocket *camera_socket = nullptr;
    Camera3D *camera_3d = nullptr;

    bool can_move_base = true;
    bool can_process = true;
    bool can_zoom = true;
    bool can_automatic_pan = true;
    bool can_rotate_base = true;
    bool camera_can_rotate_socket_x = true;
    bool camera_can_rotate_to_mouse_offsets = true;

    bool camera_is_rotating_base = false;
    bool camera_is_rotating_mouse = false;

    real_t camera_zoom_direction = 0.0f;

    real_t camera_rotation_direction = 0.0f;

    Vector2 mouse_last_position = Vector2(0, 0);
protected:
    static void _bind_methods() {}
};



#endif //CAMERABASE_H
