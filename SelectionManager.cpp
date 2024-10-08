//
// Created by o.narvatov on 8/5/2024.
//

#include "SelectionManager.h"
#include <algorithm>
#include <godot_cpp/variant/utility_functions.hpp>
#include "blitzunit.h"

void SelectionManager::select(BlitzUnit *unit) {
    unselectAll();

    add_selection(unit);
}

void SelectionManager::selectAll(const std::vector<BlitzUnit*> &select_units) {
    unselectAll();

    for (auto *unit : select_units) {
        add_selection(unit);
    }

    for (auto e : selected_units) {
        char arr[40];
        strcpy(arr, ("Selected entity " + std::to_string(e->get_index())).c_str());
        UtilityFunctions::print(arr);
    }
}

void SelectionManager::add_selection(BlitzUnit *unit) {
    unit->select();
    selected_units.push_back(unit);
}

void SelectionManager::remove_selection(BlitzUnit *unit) {
    unit->unselect();

    const auto position = std::find(selected_units.begin(), selected_units.end(), unit);

    if (position != selected_units.end()) {
        selected_units.erase(position);
    }
}

void SelectionManager::unselectAll() {
    for (auto unit: selected_units) {
        unit->unselect();
    }
    selected_units.clear();
}
