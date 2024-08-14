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
private:
    CameraSocket *camera_socket = nullptr;
    Camera3D *camera_3d = nullptr;

    bool can_move_base = true;
    bool can_process = true;
protected:
    static void _bind_methods() {}
};



#endif //CAMERABASE_H
