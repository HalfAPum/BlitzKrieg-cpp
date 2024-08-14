//
// Created by o.narvatov on 8/14/2024.
//

#include "camerabase.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../Constants.h"

//move
constexpr real_t camera_move_speed = 20;

//zoom
constexpr real_t camera_zoom_speed = 40;
constexpr real_t camera_zoom_min_level = 10;
constexpr real_t camera_zoom_max_level = 25;
constexpr real_t camera_zoom_speed_damp = 0.98;

//pan
constexpr real_t camera_automatic_pan_margin = 16;
constexpr real_t camera_automatic_pan_speed = 12;

void CameraBase::_ready() {
    camera_socket = get_node<CameraSocket>("camera_socket");
    camera_3d = camera_socket->get_node<Camera3D>("Camera3D");
}

void CameraBase::_unhandled_input(const Ref<InputEvent> &p_event) {
    if (p_event->is_action(Constants::getInstance().CAMERA_ZOOM_IN)) {
        camera_zoom_direction = -1;
    } else if (p_event->is_action(Constants::getInstance().CAMERA_ZOOM_OUT)) {
        camera_zoom_direction = 1;
    }
}

void CameraBase::_process(const double p_delta) {
    if (!can_process) return;

    camera_base_move(p_delta);
    camera_zoom_update(p_delta);
    camera_automatic_pan(p_delta);
}

void CameraBase::camera_base_move(const double delta) {
    if (!can_move_base) return;

    auto velocity = Vector3();

    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_FORWARD)) {
        velocity -= get_transform().get_basis().get_column(2);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_BACKWARD)) {
        velocity += get_transform().get_basis().get_column(2);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_RIGHT)) {
        velocity += get_transform().get_basis().get_column(0);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_LEFT)) {
        velocity -= get_transform().get_basis().get_column(0);
    }

    set_position(get_position() + (velocity.normalized() * camera_move_speed * delta));
}

void CameraBase::camera_zoom_update(const real_t delta) {
    if (!can_zoom) return;

    const auto camera_position = camera_3d->get_position();

    const real_t new_zoom = Math::clamp(camera_position.z + camera_zoom_direction * camera_zoom_speed * delta, camera_zoom_min_level, camera_zoom_max_level);

    camera_3d->set_position(Vector3(camera_position.x, camera_position.y, new_zoom));

    camera_zoom_direction *= camera_zoom_speed_damp;
}

void CameraBase::camera_automatic_pan(const real_t delta) {
    if (!can_automatic_pan) return;

    const auto viewport_current = get_viewport();
    auto pan_direction = Vector2();

    const auto viewport_visible_rect = Rect2i(viewport_current->get_visible_rect());
    const auto viewport_size = viewport_visible_rect.size;

    const auto current_mouse_pos = viewport_current->get_mouse_position();
    constexpr auto margin = camera_automatic_pan_margin;

    const auto zoom_factor = camera_3d->get_position().z * 0.1;

    if (current_mouse_pos.x < margin) {
        pan_direction.x = -1;
    } else if (current_mouse_pos.x > viewport_size.x - margin) {
        pan_direction.x = 1;
    }

    if (current_mouse_pos.y < margin) {
        pan_direction.y = -1;
    } else if (current_mouse_pos.y > viewport_size.y - margin) {
        pan_direction.y = 1;
    }

    translate(Vector3(
        pan_direction.x * camera_automatic_pan_speed * zoom_factor * delta,
        0,
        pan_direction.y * camera_automatic_pan_speed * zoom_factor * delta)
    );

}


