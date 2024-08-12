//
// Created by o.narvatov on 8/11/2024.
//

#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

struct Quadrilateral {
    Vector2 bottom_left_vertex;
    Vector2 bottom_right_vertex;
    Vector2 top_right_vertex;
    Vector2 top_left_vertex;

    Quadrilateral(
        const Vector2 &_bottom_left_vertex,
        const Vector2 &_bottom_right_vertex,
        const Vector2 &_top_right_vertex,
        const Vector2 &_top_left_vertex
    ) : bottom_left_vertex(_bottom_left_vertex),
    bottom_right_vertex(_bottom_right_vertex),
    top_right_vertex(_top_right_vertex),
    top_left_vertex(_top_left_vertex)
    {}
};

#endif //QUADRILATERAL_H
