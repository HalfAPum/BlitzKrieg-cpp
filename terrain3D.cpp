//
// Created by o.narvatov on 8/5/2024.
//

#include "terrain3D.h"

#include <set>
#include <stack>
#include <unordered_set>
#include <godot_cpp/classes/resource_loader.hpp>

#include "blitzunit.h"
#include "Constants.h"
#include "SelectionManager.h"
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "UnitGirdFactory.h"
#include "UnitGridXXS.h"
#include "vector2/hashfunction.h"
#include "vector2/scan_inner_quadrilateral_grids.h"
#include "vector2/scan_line.h"
#include "fill_reachable_cells.h"


namespace godot {
    class PackedScene;
}

void Terrain3D::_ready() {
    main_camera = get_parent()->get_node<Node3D>("camera_base")->get_node<Node3D>("camera_socket")->get_node<Camera3D>("Camera3D");
    ui_rect = get_node<NinePatchRect>("SelectRect");
    ui_rect->set_visible(false);
}

constexpr int MIN_DRAG_LENGH_SQ = 128;

void Terrain3D::_process(double p_delta) {
    if (leftButtonDrag) {
        drag_rect_area.set_size(get_viewport()->get_mouse_position() - drag_rect_area.position);

        update_ui_rect();

        if (!ui_rect->is_visible()) {
            if (drag_rect_area.size.length_squared() > MIN_DRAG_LENGH_SQ) {
                ui_rect->set_visible(true);
            }
        }
    }
}

void Terrain3D::update_ui_rect() const {
    ui_rect->set_size(drag_rect_area.size.abs());

    if (drag_rect_area.size.x < 0) {
        ui_rect->set_scale(Vector2(-1, ui_rect->get_scale().y));
    } else {
        ui_rect->set_scale(Vector2(1, ui_rect->get_scale().y));
    }

    if (drag_rect_area.size.y < 0) {
        ui_rect->set_scale(Vector2(ui_rect->get_scale().x, -1));
    } else {
        ui_rect->set_scale(Vector2(ui_rect->get_scale().x, 1));
    }
}

void Terrain3D::_input(const Ref<InputEvent> &p_event) {
    if (p_event->is_action_pressed(Constants::getInstance().SHIFT)) {
        shiftButtonPressed = true;
    } else if (p_event->is_action_released(Constants::getInstance().SHIFT)) {
        shiftButtonPressed = false;
    }
}



void Terrain3D::_input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) {
    if (!p_event->is_action_type()) {
        if (leftButtonPressed) {
            leftButtonPressed = false;
            if (!leftButtonDrag) {
                leftButtonDrag = true;
                ui_rect->set_position(drag_rect_area.position);
            }
        }

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
        drag_rect_area.set_position(get_viewport()->get_mouse_position());
    } else if (leftButtonPressed && p_event->is_action_released(Constants::getInstance().LEFT_CLICK)) {
        process_left_button_released();
    } else if (leftButtonDrag && p_event->is_action_released(Constants::getInstance().LEFT_CLICK)) {
        leftButtonDrag = false;
        ui_rect->set_visible(false);
        if (drag_rect_area.size.length_squared() > MIN_DRAG_LENGH_SQ) {
            ray_cast_quadrilateral = true;
        } else if (!shiftButtonPressed) {
            SelectionManager::getInstance().unselectAll();
        }
    } else if (p_event->is_action_pressed(Constants::getInstance().RIGHT_CLICK)) {
        rightButtonPressed = true;
        pressedPosition = p_position;
    } else if (rightButtonPressed && p_event->is_action_released(Constants::getInstance().RIGHT_CLICK)) {
        if (SelectionManager::getInstance().selected_units.empty()) return;

        rightButtonPressed = false;

        const auto unit = SelectionManager::getInstance().selected_units[0];
        unit->move_command(pressedPosition);
    }
}

BlitzUnit* find_selectable_unit(stack<GridCell> &stack, const Vector2 &press_position) {
    while (!stack.empty()) {
        const auto grid_cell = stack.top();
        stack.pop();

        const int x = grid_cell.x;
        const int z = grid_cell.z;

        if (x < 0 || x >= MAP_SIZE / grid_cell.unit_grid->grid_size) continue;
        if (z < 0 || z >= MAP_SIZE / grid_cell.unit_grid->grid_size) continue;

        for (auto *unit : grid_cell.unit_grid->get_units(x, z)) {
            const auto unit_position3D = unit->get_position();
            const auto unit_position2D = Vector2(unit_position3D.x, unit_position3D.z);

            const auto distance = press_position.distance_to(unit_position2D);

            if (distance <= unit->selection_radius) {
                return unit;
            }
        }
    }

    return nullptr;
}

void Terrain3D::process_left_button_released() {
    leftButtonPressed = false;

    std::stack<GridCell> stack;

    const auto press_position = get_position3d_from(get_viewport()->get_mouse_position());

    fill_reachable_cells(
        stack,
        press_position.x,
        press_position.y,
        UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xxs
    );

    fill_reachable_cells(
        stack,
        press_position.x,
        press_position.y,
        UnitGridFactory::instance().player_unit_grid_abstract_factory->grid_xxs
    );

    const auto selectable_unit = find_selectable_unit(stack, press_position);

    if (selectable_unit == nullptr) {
        //If we miss with shif pressed don't deselect current items
        if (!shiftButtonPressed) return

        SelectionManager::getInstance().unselectAll();
    } else if (selectable_unit->isEnemy) {
        if (SelectionManager::getInstance().selected_units.empty()) {
            SelectionManager::getInstance().select(selectable_unit);
        } else {
            for (auto unit: SelectionManager::getInstance().selected_units) {
                unit->move_to_the_enemy_then_attack(selectable_unit);
            }
        }
    } else {
        if (shiftButtonPressed) {
            if (selectable_unit->selected) {
                SelectionManager::getInstance().remove_selection(selectable_unit);
            } else {
                SelectionManager::getInstance().add_selection(selectable_unit);
            }
        } else {
            SelectionManager::getInstance().select(selectable_unit);
        }
    }
}


Vector2 Terrain3D::get_position3d_from(const Vector2 &position2d) const {
    const auto space_state = get_world_3d()->get_direct_space_state();

    const auto ray_origin = main_camera->project_ray_origin(position2d);
    const auto ray_end = ray_origin + main_camera->project_ray_normal(position2d) * 1000;
    const auto query = PhysicsRayQueryParameters3D::create(ray_origin, ray_end);

    auto result = space_state->intersect_ray(query);

    if (result.has("position")) {
        const Vector3 vec3_res = result["position"];
        return { vec3_res.x, vec3_res.z };
    }
    return {};
}

void Terrain3D::_physics_process(double p_delta) {
    if (ray_cast_quadrilateral) {
        const auto quadrilateral = ray_cast_ui_rect();
        select_in_quadrilateral(quadrilateral);

        ray_cast_quadrilateral = false;
    }
}

std::array<Vector2, 4> Terrain3D::ray_cast_ui_rect() const {
    return {
        get_position3d_from(drag_rect_area.get_position()),
        get_position3d_from(Vector2(drag_rect_area.get_end().x, drag_rect_area.get_position().y)),
        get_position3d_from(drag_rect_area.get_end()),
        get_position3d_from(Vector2(drag_rect_area.get_position().x, drag_rect_area.get_end().y))
    };
}

//If this is perfomance concern you can consider checking not the whole quadrilateral
//but only single line which intersects grid (if only 1 line intersects it).
bool is_inside_quadrilateral(const BlitzUnit* unit, const std::array<Vector2, 4> &quadrilateral) {
    const auto unit_position = unit->get_position();
    const auto unit_position2D = Vector2(unit_position.x, unit_position.z);

    PackedVector2Array packed_vector2_array;
    for (auto vertice : quadrilateral) {
        packed_vector2_array.append(vertice);
    }

    return Geometry2D::get_singleton()->is_point_in_polygon(unit_position2D, packed_vector2_array);
}

void save_select_units(
    std::unordered_set<Vector2, HashFunction> &scan_set,
    std::vector<BlitzUnit*> &select_units,
    const std::array<Vector2, 4> &quadrilateral,
    const bool validate_position_in_qiadrilateral
) {
    for (const auto &grid : scan_set) {
        const int x = grid.x;
        const int z = grid.y;

        if (x < 0 || x >= MAP_SIZE / GRID_SIZE_XXS) continue;
        if (z < 0 || z >= MAP_SIZE / GRID_SIZE_XXS) continue;

        auto grid_units = UnitGridFactory::instance().player_unit_grid_abstract_factory->
            grid_xxs->get_units(x, z);

        for (auto *unit : grid_units) {
            if (validate_position_in_qiadrilateral && !is_inside_quadrilateral(unit, quadrilateral)) {
                continue;
            }

            select_units.push_back(unit);
        }
    }
}

void Terrain3D::select_in_quadrilateral(const std::array<Vector2, 4> &quadrilateral) const {
    std::unordered_set<Vector2, HashFunction> frame_scan_set {};
    std::unordered_set<Vector2, HashFunction> inside_scan_set {};

    auto emplace_lambda = [](const int axis_coordinate) { return axis_coordinate / GRID_SIZE_XXS; };

    //Scan quadrilateral frame
    scan_line(quadrilateral[0], quadrilateral[1], frame_scan_set, emplace_lambda);
    scan_line(quadrilateral[1], quadrilateral[2], frame_scan_set, emplace_lambda);
    scan_line(quadrilateral[2], quadrilateral[3], frame_scan_set, emplace_lambda);
    scan_line(quadrilateral[3], quadrilateral[0], frame_scan_set, emplace_lambda);

    std::vector<BlitzUnit*> select_units {};

    //Save units of quadrilateral frame
    save_select_units(frame_scan_set, select_units, quadrilateral, true);

    //Scan inner quadrilateral grids
    scan_inner_quadrilateral_grids(quadrilateral, drag_rect_area, frame_scan_set, inside_scan_set, emplace_lambda);

    //Save units inside quadrilateral frame
    save_select_units(inside_scan_set, select_units, quadrilateral, false);

    //Select units
    SelectionManager::getInstance().selectAll(select_units);
}
