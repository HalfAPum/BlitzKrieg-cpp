//
// Created by o.narvatov on 8/5/2024.
//

#include "blitzunit.h"

#include <queue>
#include <stack>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>

#include "can_attack_enemy_algorithm.h"
#include "Constants.h"
#include "EnemyGrid.h"
#include "EnemyGridS.h"
#include "EnemyGridXS.h"
#include "EnemyGridXXS.h"
#include "fill_initial_adjacent_cells_algorithm.h"
#include "GridCell.h"
#include "SelectionManager.h"
#include "split_grid_cell.h"

void BlitzUnit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_search_enemy"), &BlitzUnit::_search_enemy);
}

void BlitzUnit::_ready() {
    const auto position = get_position();
    old_x = position.x;
    old_z = position.z;

    if (isEnemy) return;

    search_enemy_timer = new Timer;

    add_child(search_enemy_timer);

    search_enemy_timer->set_one_shot(true);


    search_enemy_timer->set_timer_process_callback(Timer::TimerProcessCallback::TIMER_PROCESS_PHYSICS);
    search_enemy_timer->connect("timeout", Callable(this, "_search_enemy"));

    search_enemy_timer->set_wait_time(10);
    search_enemy_timer->start();
}

void BlitzUnit::on_grid_position_changed() {
    if (!isEnemy) return;

    EnemyGridXXS::instance()->remove_enemy(this);
    EnemyGridXXS::instance()->add_enemy(this);
}

constexpr int MOVE_SPEED = 4;
constexpr double NORMAL_ROTATION_SPEED = 3;
constexpr double FINISHING_ROTATION_SPEED = 5;

void BlitzUnit::check_grid_position_change() {
    const auto position = get_position();

    const int x = position.x;
    const int z = position.z;

    if (x != old_x && x != 0 && old_x != 0) {
        if (x % GRID_SIZE_XXS == 0 || old_x % GRID_SIZE_XXS == 0) {
            on_grid_position_changed();
        }
    } else if (z != old_z && z != 0 && old_z != 0) {
        if (z % GRID_SIZE_XXS == 0 || old_z % GRID_SIZE_XXS == 0) {
            on_grid_position_changed();
        }
    }

    old_x = x;
    old_z = z;
}


void BlitzUnit::_process(double p_delta) {
    if (!isMoving || isRotating) return;

    check_grid_position_change();

    const auto position = get_position();

    if (abs(position.distance_to(movePosition)) < 0.55) {
        isMoving = false;
    }

    const auto direction = position.direction_to(movePosition);
    set_velocity(direction * MOVE_SPEED);

    move_and_slide();
}

void BlitzUnit::rotate(const double p_delta) {
    const double currentRotation = get_rotation().y;

    if (abs(currentRotation - last_rotation) <= 0.001) {
        isRotating = false;
        last_rotation = DEFAULT_LAST_ROTATION;
        return;
    }

    last_rotation = currentRotation;

    const auto direction = get_position().direction_to(movePosition);
    const double target_angle = atan2(-direction.x, -direction.z);

    double speed;
    if (abs(currentRotation - target_angle) < 0.5) speed = FINISHING_ROTATION_SPEED;
    else speed = NORMAL_ROTATION_SPEED;

    const double lerp = Math::lerp_angle(currentRotation, target_angle, p_delta * speed);

    set_rotation(Vector3(0, lerp, 0));

}


void BlitzUnit::_physics_process(const double p_delta) {
    if (isRotating) {
        rotate(p_delta);
    }

    // if (is_on_floor()) return;
    //
    // auto velocity = get_velocity();
    // velocity.y -= p_delta * Constants::getInstance().gravity;
    // set_velocity(velocity);
    //
    // move_and_slide();
}


void BlitzUnit::select() {
    selected = true;
}

void BlitzUnit::unselect() {
    selected = false;
}

void BlitzUnit::_input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) {
    if (!p_event->is_action_type()) {
        leftButtonPressed = false;
        return;
    }

    if (selected) return;


    if (p_event->is_action_pressed(Constants::getInstance().LEFT_CLICK)) {
        leftButtonPressed = true;
    } else if (leftButtonPressed && p_event->is_action_released(Constants::getInstance().LEFT_CLICK)) {
        leftButtonPressed = false;
        select();
        SelectionManager::getInstance().select(this);
    }
}

void BlitzUnit::move_command(const Vector3 &vector3) {
    movePosition = vector3;
    isRotating = true;
    isMoving = true;
}


BlitzUnit::~BlitzUnit() {
    if (selected) {
        SelectionManager::getInstance().free_unit(this);
    }

    delete search_enemy_timer;
}

EnemyGrid* get_enemy_grid(const real_t radius) {
    const real_t minumum_grid_size = radius;

    if (minumum_grid_size <= 4) {
        return EnemyGridXXS::instance();
    }
    if (minumum_grid_size <= 8) {
        return EnemyGridXS::instance();
    }
    return EnemyGridS::instance();
}

constexpr unsigned MAP_SIZE = 32;

BlitzUnit* findEnemy(const BlitzUnit *unit) {
    auto position = unit->get_position();

    stack<GridCell> stack;

    fill_initial_adjacent_cells(stack, position.x, position.z, get_enemy_grid(unit->attack_radius));

    while (!stack.empty()) {
        auto grid_cell = stack.top();

        stack.pop();

        if (grid_cell.x < 0 || grid_cell.x >= MAP_SIZE / grid_cell.enemy_grid->grid_size) continue;
        if (grid_cell.z < 0 || grid_cell.z >= MAP_SIZE / grid_cell.enemy_grid->grid_size) continue;

        const int enemy_count = grid_cell.get_enemy_count();

        if (enemy_count == 0) {
            continue;
        }

        if (enemy_count <= 4 || grid_cell.enemy_grid->is_last_level_of_precision()) {
            const auto found_unit = grid_cell.find_if(unit, can_attack_enemy);

            if (found_unit != nullptr) return found_unit;
        } else {
            auto precision_grid_cells = split_grid_cell(grid_cell, unit);

            for (auto new_cell : precision_grid_cells) {
                stack.push(new_cell);
            }
        }
    }

    return nullptr;
}

void BlitzUnit::_search_enemy() {
    const auto enemy = findEnemy(this);

    if (enemy != nullptr) {
        prepare_to_attack(enemy);
    } else {
    }
    search_enemy_timer->start();

}

void BlitzUnit::prepare_to_attack(const BlitzUnit* enemy) {
    isMoving = false;
    movePosition = enemy->get_position();
    isRotating = true;
}