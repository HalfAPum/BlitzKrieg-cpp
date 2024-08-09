//
// Created by o.narvatov on 8/5/2024.
//

#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include <vector>
#include <godot_cpp/godot.hpp>

class BlitzUnit;

using namespace godot;

class SelectionManager {
private:
    SelectionManager() {}
public:
    SelectionManager(SelectionManager const&) = delete;
    void operator=(SelectionManager const&) = delete;
    static SelectionManager& getInstance()
    {
        static SelectionManager instance;
        return instance;
    }

    void select(BlitzUnit *unit);

    void free_unit(BlitzUnit *unit);
    void unselectAll();

    std::vector<BlitzUnit*> selected_units = {};
};

#endif //SELECTIONMANAGER_H
