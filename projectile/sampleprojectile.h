//
// Created by o.narvatov on 8/12/2024.
//

#ifndef SAMPLEPROJECTILE_H
#define SAMPLEPROJECTILE_H
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>

using namespace godot;

class BlitzUnit;
class UnitGridXXS;

class SampleProjectile : public RigidBody3D {
    GDCLASS(SampleProjectile, RigidBody3D)
public:
    BlitzUnit* owner = nullptr;
    real_t collision_damage = 20;
    real_t speed = 15;
    bool use_remote_timer = false;
    Vector3 *target_position = nullptr;

    void _ready() override;
    void _physics_process(double p_delta) override;

    ~SampleProjectile() override;
private:
    Timer *destruction_timer = nullptr;

    void _free();
protected:
    static void _bind_methods();
};



#endif //SAMPLEPROJECTILE_H
