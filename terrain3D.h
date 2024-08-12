//
// Created by o.narvatov on 8/5/2024.
//

#ifndef TERRAIN3D_H
#define TERRAIN3D_H

#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/nine_patch_rect.hpp>

using namespace godot;

class Terrain3D : public StaticBody3D {
    GDCLASS(Terrain3D, StaticBody3D)
public:
    void _ready() override;
    void update_ui_rect() const;

    void _process(double p_delta) override;

    void _input(const Ref<InputEvent> &p_event) override;
    void _input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) override;
    Vector2 get_position3d_from(const Vector2 &position2d) const;

    void _physics_process(double p_delta) override;

    std::array<Vector2, 4> ray_cast_ui_rect() const;
    void select_in_quadrilateral(const std::array<Vector2, 4> &quadrilateral) const;
protected:
    static void _bind_methods() {}
private:
    //press
    bool shiftButtonPressed = false;
    bool leftButtonPressed = false;
    bool rightButtonPressed = false;
    Vector3 pressedPosition;

    //drag
    NinePatchRect *ui_rect = nullptr;
    bool leftButtonDrag = false;
    Rect2 drag_rect_area = Rect2();
    bool ray_cast_quadrilateral = false;
    Camera3D *main_camera = nullptr;

    void process_left_button_released();
};



#endif //TERRAIN3D_H
