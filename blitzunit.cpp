//
// Created by o.narvatov on 8/5/2024.
//

#include "blitzunit.h"

#include <queue>
#include <stack>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include "can_attack_enemy_algorithm.h"
#include "UnitGrid.h"
#include "UnitGridXXS.h"
#include "fill_reachable_cells.h"
#include "GridCell.h"
#include "SelectionManager.h"
#include "split_grid_cell.h"
#include "UnitGirdFactory.h"
#include "projectile/sampleprojectile.h"

void BlitzUnit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_search_enemy"), &BlitzUnit::_search_enemy);
}

void BlitzUnit::_ready() {
    if (isEnemy) {
        selected_circle = get_node<Sprite3D>("selected_circle_enemy");
    } else {
        selected_circle = get_node<Sprite3D>("selected_circle");
    }

    selected_circle->set_visible(false);

    bullet_spawn = get_node<Node3D>("BulletSpawn");
    projectile_scene = ResourceLoader::get_singleton()->load("res://sample_projectile.tscn");

    get_unit_grid_factory(this)->grid_xxs->add_enemy(this);

    const auto position = get_position();
    old_x = position.x;
    old_z = position.z;

    if (isEnemy) return;

    search_enemy_timer = new Timer;

    add_child(search_enemy_timer);

    search_enemy_timer->set_one_shot(true);


    search_enemy_timer->set_timer_process_callback(Timer::TimerProcessCallback::TIMER_PROCESS_PHYSICS);
    search_enemy_timer->connect("timeout", Callable(this, "_search_enemy"));

    search_enemy_timer->set_wait_time(5);
    search_enemy_timer->start();
}

Vector2 BlitzUnit::get_position2D() const {
    const auto position = get_position();

    return { position.x, position.z };
}


void BlitzUnit::on_grid_position_changed() {
    get_unit_grid_factory(this)->grid_xxs->remove_enemy(this);
    get_unit_grid_factory(this)->grid_xxs->add_enemy(this);
}

constexpr real_t MOVE_SPEED = 4;
constexpr real_t COLLISION_MOVE_SPEED = 1;
constexpr double NORMAL_ROTATION_SPEED = 3;
constexpr double FINISHING_ROTATION_SPEED = 5;

void BlitzUnit::check_grid_position_change() {
    const auto position = get_position();

    const int x = position.x;
    const int z = position.z;

    if (x != old_x && x != 0 && old_x != 0) {
        if (x % GRID_SIZE_XXS == 0 && x == old_x + 1) {
            on_grid_position_changed();
        } else if (old_x % GRID_SIZE_XXS == 0 && old_x == x + 1) {
            on_grid_position_changed();
        }
    } else if (z != old_z && z != 0 && old_z != 0) {
        if (z % GRID_SIZE_XXS == 0 && z == old_z + 1) {
            on_grid_position_changed();
        } else if (old_z % GRID_SIZE_XXS == 0 && old_z == z + 1) {
            on_grid_position_changed();
        }
    }

    old_x = x;
    old_z = z;
}

void BlitzUnit::_process(double p_delta) {
    if (isCollisionMoving) {
        const auto isFinishedMoving = do_move(collisionMovePosition, COLLISION_MOVE_SPEED);
        isCollisionMoving = !isFinishedMoving;

        return;
    }

    if (!isMoving || isRotating) return;

    const auto isFinishedMoving = do_move(movePosition, MOVE_SPEED);
    isMoving = !isFinishedMoving;
}

bool BlitzUnit::do_move(const Vector3 &target_position, const real_t move_speed) {
    check_grid_position_change();

    const auto position = get_position();

    if (abs(position.distance_to(target_position)) < 0.55) {
        return true;
    }

    const auto direction = position.direction_to(target_position);
    const auto target_velocity = Vector3(direction.x * move_speed, 0, direction.z * move_speed);
    // very fun jumping effect
    // set_velocity(direction * move_speed);
    set_velocity(target_velocity);

    move_and_slide();

    return false;
}


void BlitzUnit::rotate(const double p_delta) {
    const double currentRotation = get_rotation().y;

    if (abs(currentRotation - last_rotation) <= 0.001) {
        isRotating = false;
        last_rotation = DEFAULT_LAST_ROTATION;

        if (isAttacking) start_attack();
        isAttacking = false;
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
    if (!isCollisionMoving && isRotating) {
        rotate(p_delta);
    }

    //detect collision
    detect_unit_collisions();

    // if (is_on_floor()) return;
    //
    // auto velocity = get_velocity();
    // velocity.y -= p_delta * Constants::getInstance().gravity;
    // set_velocity(velocity);
    //
    // move_and_slide();
}

auto *enemy_grid_xxs = UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xxs;
auto *ally_grid_xxs = UnitGridFactory::instance().ally_unit_grid_abstract_factory->grid_xxs;
auto *player_grid_xxs = UnitGridFactory::instance().player_unit_grid_abstract_factory->grid_xxs;

void BlitzUnit::detect_unit_collisions() {
    const auto position = get_position2D();

    detect_unit_collisions(position, enemy_grid_xxs);
    detect_unit_collisions(position, ally_grid_xxs);
    detect_unit_collisions(position, player_grid_xxs);
}

void BlitzUnit::detect_unit_collisions(const Vector2 &position, UnitGridXXS *grid) {
    stack<GridCell> stack;

    fill_reachable_cells(stack, position.x, position.y, grid);

    while (!stack.empty()) {
        const auto cell = stack.top();
        stack.pop();

        const int x = cell.x;
        const int z = cell.z;

        if (x < 0 || x >= MAP_SIZE / GRID_SIZE_XXS) continue;
        if (z < 0 || z >= MAP_SIZE / GRID_SIZE_XXS) continue;

        for (auto *unit : grid->get_units(x, z)) {
            if (this == unit) continue;

            const auto unit_position = unit->get_position2D();

            if (abs(unit_position.x - position.x) > collision_radius) continue;
            //position.y is actually position.z but it's Vector2.
            if (abs(unit_position.y - position.y) > collision_radius) continue;

            const auto distance = position.distance_to(unit_position);

            if (distance <= collision_radius) {
                collision_push(unit, distance);
                unit->collision_push(this, distance);
            }
        }
    }
}

void BlitzUnit::collision_push(const BlitzUnit *collision_unit, const real_t distance) {
    const auto unit_position = get_position();

    const auto untis_coordinate_difference = get_position2D() - collision_unit->get_position2D();

    const auto collision_vector = untis_coordinate_difference / distance * collision_push_distance;

    collisionMovePosition = unit_position + Vector3(collision_vector.x, 0, collision_vector.y);
    isCollisionMoving = true;
}



void BlitzUnit::select() {
    selected = true;
    selected_circle->set_visible(true);
}

void BlitzUnit::unselect() {
    selected = false;
    selected_circle->set_visible(false);
}

void BlitzUnit::move_command(const Vector3 &vector3) {
    movePosition = vector3;
    isRotating = true;
    isMoving = true;
}


BlitzUnit::~BlitzUnit() {
    if (selected) {
        SelectionManager::getInstance().remove_selection(this);
    }

    delete search_enemy_timer;
}

UnitGrid* get_enemy_grid(const real_t radius) {
    const real_t minumum_grid_size = radius;

    if (minumum_grid_size <= 4) {
        return UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xxs_cast;
    }
    if (minumum_grid_size <= 8) {
        return UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xs;
    }
    return UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_s;
}


BlitzUnit* findEnemy(const BlitzUnit *unit) {
    auto position = unit->get_position2D();

    stack<GridCell> stack;

    fill_reachable_cells(stack, position.x, position.y, get_enemy_grid(unit->attack_radius));

    while (!stack.empty()) {
        auto grid_cell = stack.top();

        stack.pop();

        if (grid_cell.x < 0 || grid_cell.x >= MAP_SIZE / grid_cell.unit_grid->grid_size) continue;
        if (grid_cell.z < 0 || grid_cell.z >= MAP_SIZE / grid_cell.unit_grid->grid_size) continue;

        const int enemy_count = grid_cell.get_enemy_count();

        if (enemy_count == 0) {
            continue;
        }

        if (enemy_count <= 4 || grid_cell.unit_grid->is_last_level_of_precision()) {
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
    isAttacking = true;
    isRotating = true;
}

void BlitzUnit::start_attack() {
    auto *projectile = dynamic_cast<SampleProjectile*>(projectile_scene->instantiate());
    projectile->use_remote_timer = true;

    add_sibling(projectile);

    projectile->set_global_transform(bullet_spawn->get_global_transform());

    projectile->set_linear_velocity(projectile->get_position().direction_to(movePosition) * projectile->speed);
}

void BlitzUnit::move_to_the_enemy_then_attack(BlitzUnit *enemy) {
    UtilityFunctions::print("Move to the enemy then attack");
}

