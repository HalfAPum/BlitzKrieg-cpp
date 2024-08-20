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
#include "Constants.h"
#include "UnitGrid.h"
#include "UnitGridXXS.h"
#include "fill_reachable_cells.h"
#include "GridCell.h"
#include "SelectionManager.h"
#include "split_grid_cell.h"
#include "UnitGirdFactory.h"
#include "collision/collision_detection.h"
#include "projectile/sampleprojectile.h"
#include "translation/vector2_to_vector2_translation.h"
#include "ygrid/YAxisGrid.h"

void BlitzUnit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_search_enemy"), &BlitzUnit::_search_enemy);
}

void BlitzUnit::_ready() {
    rotatable_node = get_node<Node3D>("rotatable_node");

    rotatable_node->set_rotation(Vector3(0, spawn_rotation_radians, 0));

    hp_bar = get_node<MeshInstance3D>("hp_bar");
    if (isEnemy) {
        hp_bar->set_instance_shader_parameter(Constants::getInstance().BAR_COLOR, Vector3(1.0, 0.0, 0.0));
    } else {
        hp_bar->set_instance_shader_parameter(Constants::getInstance().BAR_COLOR, Vector3(0.0, 1.0, 0.0));
    }

    if (isEnemy) {
        selected_circle = get_node<Node3D>("selected_circle_enemy");
    } else {
        selected_circle = get_node<Node3D>("selected_circle");
    }

    selected_circle->set_visible(false);

    attack_circle = get_node<Node3D>("attack_circle");
    attack_circle->set_visible(false);

    bullet_spawn = rotatable_node->get_node<Node3D>("BulletSpawn");
    projectile_scene = ResourceLoader::get_singleton()->load("res://sample_projectile.tscn");

    get_unit_grid_factory(this)->grid_xxs->add_enemy(this);

    const auto position = get_position();
    old_x = position.x;
    old_z = position.z;

    old_physics_x = position.x;
    old_physics_z = position.z;
    old_physics_y = position.y;

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

constexpr real_t MOVE_SPEED = 10;
constexpr real_t COLLISION_MOVE_SPEED = 2;
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
}

void BlitzUnit::_process(double p_delta) {
    const auto new_position = get_position();

    if (isCollisionMoving) {
        const auto isFinishedMoving = do_move(collisionMovePosition, collisionMoveTranslation);
        isCollisionMoving = !isFinishedMoving;

        old_x = new_position.x;
        old_z = new_position.z;

        return;
    }

    if (!isMoving || isRotating) return;

    const auto isFinishedMoving = do_move(movePosition, moveTranslation);

    if (isFinishedMoving) {
        move_disabled_state = false;
        isMoving = false;
    }

    old_x = new_position.x;
    old_z = new_position.z;
}

bool BlitzUnit::do_move(const Vector3 &target_position, const Vector3 &translation) {
    static constexpr auto MOVEMENT_FINISH_DISTANCE_DIFFERECE = 0.1f;

    check_grid_position_change();

    const auto position = get_position2D();

    float x_distance = abs(target_position.x - position.x);
    float z_distance = abs(target_position.z - position.y);

    if (x_distance > MOVEMENT_FINISH_DISTANCE_DIFFERECE) {
        //do nothing
    } else if (z_distance > MOVEMENT_FINISH_DISTANCE_DIFFERECE) {
        //do nothing
    } else if (abs(position.distance_to(Vector2(target_position.x, target_position.z))) < MOVEMENT_FINISH_DISTANCE_DIFFERECE) {
        return true;
    }

    translate(translation);

    return false;
}


void BlitzUnit::rotate(const double p_delta) {
    const auto currentRotation = rotatable_node->get_rotation();
    const double currentRotation_y = currentRotation.y;

    const auto direction = get_position().direction_to(movePosition);
    const double target_angle = atan2(-direction.x, -direction.z);

    if (abs(currentRotation_y - last_rotation) <= 0.001) {
        rotatable_node->set_rotation(Vector3(currentRotation.x, target_angle, currentRotation.z));
        isRotating = false;
        last_rotation = DEFAULT_LAST_ROTATION;

        if (isAttacking) start_attack();
        isAttacking = false;
        return;
    }

    last_rotation = currentRotation_y;

    double speed;
    if (abs(currentRotation_y - target_angle) < 0.5) speed = FINISHING_ROTATION_SPEED;
    else speed = NORMAL_ROTATION_SPEED;

    const double lerp = Math::lerp_angle(currentRotation_y, target_angle, p_delta * speed);

    rotatable_node->set_rotation(Vector3(currentRotation.x, lerp, currentRotation.z));
}

float get_angle_from_vectors(const Vector2 &angle_from, const Vector2 &angle_to) {
    float angle;
    if (angle_from.x < angle_to.x) {
        angle = angle_from.angle_to_point(angle_to);
    } else {
        angle = angle_to.angle_to_point(angle_from) * -1;
    }

    constexpr int ANGLE_SMOTHENER = 4;

    return angle / ANGLE_SMOTHENER;
}

float get_angle_from_vectors2(const Vector2 &angle_from, const Vector2 &angle_to) {
    float angle;
    if (angle_from.x < angle_to.x) {
        angle = angle_from.angle_to_point(angle_to) * -1;
    } else {
        angle = angle_to.angle_to_point(angle_from);
    }

    constexpr int ANGLE_SMOTHENER = 8;

    return angle / ANGLE_SMOTHENER;
}


void BlitzUnit::_physics_process(const double p_delta) {
    if (!isCollisionMoving && isRotating) {
        rotate(p_delta);
    }

    //detect unit collision
    vector<collision_pair> collided_units;
    detect_entity_collisions(collided_units, get_position2D(), collision_radius);

    for (const auto &collided : collided_units) {
        collision_push(collided.first, collided.second);
        collided.first->collision_push(this, collided.second);
    }

    if (!isEnemy) {
        //Adjust Y position based on floot collision
        auto position = get_position();
        const auto expected_y = YAxisGrid::instance().get_y_value(position);

        if (expected_y != old_physics_y) {
            set_position(Vector3(position.x, expected_y, position.z));

            const float angle_x = get_angle_from_vectors(
                Vector2(old_physics_x, old_physics_y),
                Vector2(position.x, expected_y)
            );

            const float angle_z = get_angle_from_vectors2(
                Vector2(old_physics_z, old_physics_y),
                Vector2(position.z, expected_y)
            );

            const auto rotation = rotatable_node->get_rotation();

            rotatable_node->set_rotation(Vector3(angle_x, rotation.y, angle_z));
        }

        old_physics_x = position.x;
        old_physics_z = position.z;
        old_physics_y = expected_y;
    }
}

void BlitzUnit::collision_push(const BlitzUnit *collision_unit, const real_t distance) {
    const auto unit_position = get_position();

    const auto untis_coordinate_difference = get_position2D() - collision_unit->get_position2D();

    const auto collision_vector = untis_coordinate_difference / distance * collision_push_distance;

    collisionMovePosition = unit_position + Vector3(collision_vector.x, 0, collision_vector.y);
    collisionMoveTranslation = get_move_translation(unit_position, collisionMovePosition) * COLLISION_MOVE_SPEED;
    isCollisionMoving = true;

    //disable move_disabled_state when collide with other unit
    move_disabled_state = false;
}



void BlitzUnit::select() {
    selected = true;
    selected_circle->set_visible(true);
    attack_circle->set_visible(true);
}

void BlitzUnit::unselect() {
    selected = false;
    selected_circle->set_visible(false);
    attack_circle->set_visible(false);
}

void BlitzUnit::move_command(const Vector3 &vector3) {
    if (move_disabled_state) return;

    movePosition = vector3;
    moveTranslation = get_move_translation(get_position(), vector3) * MOVE_SPEED;
    isRotating = true;
    isMoving = true;
}


BlitzUnit::~BlitzUnit() {
    get_unit_grid_factory(this)->grid_xxs->remove_enemy(this);

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
    projectile->owner = this;

    add_sibling(projectile);

    projectile->set_global_transform(bullet_spawn->get_global_transform());

    projectile->set_linear_velocity(projectile->get_position().direction_to(movePosition) * projectile->speed);
}

void BlitzUnit::move_to_the_enemy_then_attack(BlitzUnit *enemy) {
    UtilityFunctions::print("Move to the enemy then attack");
}

void BlitzUnit::take_damage(const real_t damage) {
    heath_points -= damage;
    hp_bar->set_instance_shader_parameter(Constants::getInstance().PROGRESS, heath_points / 100);

    if (heath_points <= 0) {
        queue_free();
    }
}