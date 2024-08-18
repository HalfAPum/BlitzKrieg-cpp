//
// Created by o.narvatov on 8/5/2024.
//

#ifndef BLITZUNIT_H
#define BLITZUNIT_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

class SelectionManager;
class UnitGrid;
class UnitGridS;
class UnitGridXS;
class UnitGridXXS;

using namespace godot;

class SampleProjectile;

constexpr double DEFAULT_LAST_ROTATION = -999999999.999999999;
constexpr unsigned MAP_SIZE = 256;

class BlitzUnit : public Node3D {
    GDCLASS(BlitzUnit, Node3D)
public:
    ~BlitzUnit() override;

    void _ready() override;
    void _process(double p_delta) override;
    void _physics_process(double p_delta) override;

    //Returns wether finished moving or not.
    bool do_move(const Vector3 &target_position, const Vector3 &translation);

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

    //after spawn properties
    real_t spawn_rotation_radians = 0.0f;
    bool move_disabled_state = false;

    //collision
    const real_t collision_radius = 3;
    const real_t projectile_collision_radius = 1;

    //Previous position
    int old_x = 0;
    int old_z = 0;
    //Isn't global but rather value from YAxisGrid
    float old_y = 0;

    void move_to_the_enemy_then_attack(BlitzUnit *enemy);

    void take_damage(real_t damage);
protected:
    static void _bind_methods();
private:
    //selection
    Node3D *selected_circle = nullptr;
    Node3D *attack_circle = nullptr;

    //moving
    Vector3 movePosition;
    Vector3 moveTranslation;
    void checkRecalculateMoveTranslation();
    void recalculateMoveTranslation(const Vector3 &position);
    bool moveTranslationRecalculated = false;
    bool isMoving = false;

    //rotation
    Node3D *rotatable_node = nullptr;
    bool isRotating = false;
    double last_rotation = DEFAULT_LAST_ROTATION;

    //collision
    Vector3 collisionMovePosition;
    Vector3 collisionMoveTranslation;
    bool isCollisionMoving = false;
    const real_t collision_push_distance = 1;

    //attack
    bool isAttacking = false;

    //timer
    Timer *search_enemy_timer = nullptr;

    //Projectile
    Node3D *bullet_spawn = nullptr;
    Ref<PackedScene> projectile_scene {};

    //health
    real_t heath_points = 100;
    MeshInstance3D *hp_bar = nullptr;

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