//
// Created by o.narvatov on 8/5/2024.
//

#include "terrain3D.h"

#include "blitzunit.h"
#include "Constants.h"
#include "SelectionManager.h"

void Terrain3D::_input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) {
    if (SelectionManager::getInstance().selected_units.empty()) return;

    if (!p_event->is_action_type()) {
        leftButtonPressed = false;

        if (rightButtonPressed) {
            const auto distance = pressedPosition.distance_to(p_position);

            if (distance < 1) {
                return;
            }

            rightButtonPressed = false;
        }

        return;
    }

    if (p_event->is_action_pressed(Constants::getInstance().LEFT_CLICK)) {
        leftButtonPressed = true;
    } else if (leftButtonPressed && p_event->is_action_released(Constants::getInstance().LEFT_CLICK)) {
        leftButtonPressed = false;
        SelectionManager::getInstance().unselectAll();
    } else if (p_event->is_action_pressed(Constants::getInstance().RIGHT_CLICK)) {
        rightButtonPressed = true;
        pressedPosition = p_position;
    } else if (rightButtonPressed && p_event->is_action_released(Constants::getInstance().RIGHT_CLICK)) {
        rightButtonPressed = false;

        const auto unit = SelectionManager::getInstance().selected_units[0];
        unit->move_command(pressedPosition);
    }
}
