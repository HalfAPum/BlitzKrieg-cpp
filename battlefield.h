//
// Created by o.narvatov on 8/6/2024.
//

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;


class Battlefield : public Node3D {
    GDCLASS(Battlefield, Node3D)
public:
    void _ready() override;
protected:
    static void _bind_methods() {}
};



#endif //BATTLEFIELD_H
