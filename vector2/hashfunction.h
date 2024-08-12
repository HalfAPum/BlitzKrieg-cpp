//
// Created by o.narvatov on 8/11/2024.
//

#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H
#include <functional>
#include <godot_cpp/variant/vector2.hpp>

struct HashFunction
{
    size_t operator()(const godot::Vector2& vector2) const
    {
        const size_t xHash = std::hash<int>()(vector2.x);
        const size_t yHash = std::hash<int>()(vector2.y) << 1;
        return xHash ^ yHash;
    }
};

#endif //HASHFUNCTION_H
