//
// Created by o.narvatov on 8/14/2024.
//

#ifndef CAMERASOCKET_H
#define CAMERASOCKET_H
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class CameraSocket : public Node3D {
    GDCLASS(CameraSocket, Node3D)
public:
    void _ready() override {}
protected:
    static void _bind_methods() {}
};



#endif //CAMERASOCKET_H
