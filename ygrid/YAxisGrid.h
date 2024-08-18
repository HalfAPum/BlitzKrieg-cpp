//
// Created by o.narvatov on 8/18/2024.
//

#ifndef ZAXISGRID_H
#define ZAXISGRID_H
#include <array>
#include <godot_cpp/variant/vector3.hpp>
#include "../lib/obj_vec3_height_array_parser/height_array.h"

using namespace std;
using namespace godot;

class YAxisGrid {

    //Represents array of y position for each x,z combination
    z_axis_array *y_grid_array = nullptr;


    YAxisGrid();
public:
    YAxisGrid(YAxisGrid const&) = delete;
    void operator=(YAxisGrid const&) = delete;

    static YAxisGrid& instance()
    {
        static YAxisGrid instance;
        return instance;
    }


    [[nodiscard]] float get_y_value(const Vector3 &entity_position) const;

};



#endif //ZAXISGRID_H
