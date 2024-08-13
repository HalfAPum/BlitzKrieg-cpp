//
// Created by o.narvatov on 8/5/2024.
//

#ifndef BLITZUNIT_H
#define BLITZUNIT_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/sprite3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>

class SelectionManager;
class UnitGrid;
class UnitGridS;
class UnitGridXS;
class UnitGridXXS;

using namespace godot;

constexpr double DEFAULT_LAST_ROTATION = -999999999.999999999;
constexpr unsigned MAP_SIZE = 32;

class BlitzUnit : public CharacterBody3D {
    GDCLASS(BlitzUnit, CharacterBody3D)
public:
    ~BlitzUnit() override;

    void _ready() override;
    void _process(double p_delta) override;
    void _physics_process(double p_delta) override;

    //Returns wether finished moving or not.
    bool do_move(const Vector3 &target_position, real_t move_speed);

    Vector2 get_position2D() const;

    void select();
    void unselect();

    void move_command(const Vector3 &vector3);

    bool isEnemy = false;
    bool isAlly = false;

    //attack
    const int attack_radius = 9;

    //selection
    bool selected = false;
    const real_t selection_radius = 1.5;


    //Previous position
    int old_x = 0;
    int old_z = 0;

    void move_to_the_enemy_then_attack(BlitzUnit *enemy);
protected:
    static void _bind_methods();
private:
    //selection
    Sprite3D *selected_circle = nullptr;

    //moving
    Vector3 movePosition;
    bool isMoving = false;
    bool isRotating = false;
    double last_rotation = DEFAULT_LAST_ROTATION;

    //collision
    Vector3 collisionMovePosition;
    bool isCollisionMoving = false;
    const real_t collision_radius = 3;
    const real_t collision_push_distance = 1;

    //attack
    bool isAttacking = false;

    //timer
    Timer *search_enemy_timer = nullptr;

    //Projectile
    Node3D *bullet_spawn = nullptr;
    Ref<PackedScene> projectile_scene {};

    void _search_enemy();

    void prepare_to_attack(const BlitzUnit* enemy);

    void start_attack();

    void rotate(double p_delta);

    void check_grid_position_change();

    void on_grid_position_changed();


    void detect_unit_collisions();
    void detect_unit_collisions(const Vector2 &position, UnitGridXXS *grid);
    void collision_push(const BlitzUnit *collision_unit, real_t distance);

};


#endif //BLITZUNIT_H