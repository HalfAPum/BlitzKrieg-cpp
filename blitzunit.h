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

    void _input_event(Camera3D *p_camera, const Ref<InputEvent> &p_event, const Vector3 &p_position, const Vector3 &p_normal, int32_t p_shape_idx) override;

    void select();
    void unselect();

    void move_command(const Vector3 &vector3);

    bool isEnemy = false;
    bool isAlly = false;

    //attack
    const int attack_radius = 100;

    //Previous position
    int old_x = 0;
    int old_z = 0;
protected:
    static void _bind_methods();
private:
    //selection
    bool leftButtonPressed = false;
    bool selected = false;
    Sprite3D *selected_circle = nullptr;

    //moving
    Vector3 movePosition;
    bool isMoving = false;
    bool isRotating = false;
    double last_rotation = DEFAULT_LAST_ROTATION;

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
};


#endif //BLITZUNIT_H
