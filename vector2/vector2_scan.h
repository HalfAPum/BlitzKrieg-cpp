//
// Created by o.narvatov on 8/11/2024.
//

#ifndef VECTOR2_SCAN_H
#define VECTOR2_SCAN_H
#include <algorithm>
#include <utility>
#include <set>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

inline std::pair<real_t, real_t> get_axis_increments(const Vector2 &start, const Vector2 &end, const unsigned multiplier = 1) {
    const auto hypotenuse = start.distance_to(end);

    const auto x_leg = end.x - start.x;
    const auto y_leg = end.y - start.y;

    const auto x_cos = x_leg / hypotenuse;
    const auto y_cos = y_leg / hypotenuse;

    auto x_increment = x_cos * multiplier;
    auto y_increment = y_cos * multiplier;

    return { x_increment, y_increment };
}

constexpr int DEFAULT_SCAN_JUMP = 1;

inline void scan_line(
    const Vector2 &start,
    const Vector2 &end,
    std::set<pair<int, int>> &scan_set,
    int (*emplace_lambda) (int axis_coordinate)
) {
    const auto axis_increment_pair = get_axis_increments(start, end, DEFAULT_SCAN_JUMP);

    const auto x_increment = axis_increment_pair.first;
    const auto y_increment = axis_increment_pair.second;

    auto x_scan = start.x;
    auto y_scan = start.y;

    while (true) {
        if (x_increment > 0) {
            x_scan = std::min(x_scan, end.x);
        } else {
            x_scan = std::max(x_scan, end.x);
        }

        if (y_increment > 0) {
            y_scan = std::min(y_scan, end.y);
        } else {
            y_scan = std::max(y_scan, end.y);
        }

        //scan position
        scan_set.emplace(emplace_lambda(x_scan), emplace_lambda(y_scan));

        if (x_scan == end.x && x_increment != 0) {
            break;
        }

        if (y_scan == end.y && y_increment != 0) {
            break;
        }

        x_scan += x_increment;
        y_scan += y_increment;
    }
}

#endif //VECTOR2_SCAN_H
