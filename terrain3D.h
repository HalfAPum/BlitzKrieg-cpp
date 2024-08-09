//
// Created by o.narvatov on 8/5/2024.
//

#ifndef TERRAIN3D_H
#define TERRAIN3D_H

#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class Terrain3D : public StaticBody3D {
    GDCLASS(Terrain3D, StaticBody3D)
public:
    void _input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) override;

protected:
    static void _bind_methods() {}
private:
    bool leftButtonPressed = false;
    bool rightButtonPressed = false;
    Vector3 pressedPosition;
};



#endif //TERRAIN3D_H
