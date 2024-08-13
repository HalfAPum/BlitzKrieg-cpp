//
// Created by o.narvatov on 8/13/2024.
//

#ifndef VECTOR2_TO_VECTOR2_TRANSLATION_H
#define VECTOR2_TO_VECTOR2_TRANSLATION_H
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>

using namespace godot;

inline Vector3 get_move_translation(const Vector3 &start, const Vector3 &end) {
    const auto start2D = Vector2(start.x, start.z);
    const auto end2D = Vector2(end.x, end.z);

    const auto hypotenuse = start2D.distance_to(end2D);

    const auto x_leg = end2D.x - start2D.x;
    const auto y_leg = end2D.y - start2D.y;

    const auto x_cos = x_leg / hypotenuse;
    const auto y_cos = y_leg / hypotenuse;

    static constexpr auto MULTIPLIER = 0.001f;
    const auto x_increment = x_cos * MULTIPLIER;
    const auto y_increment = y_cos * MULTIPLIER;

    return { x_increment, 0, y_increment };
}

#endif //VECTOR2_TO_VECTOR2_TRANSLATION_H
