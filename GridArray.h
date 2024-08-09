//
// Created by o.narvatov on 8/7/2024.
//

#ifndef GRIDARRAY_H
#define GRIDARRAY_H



class GridArray {
public:
    int grid_size = 0;
    int array_size = 0;

    void increment(int x, int z);
    void decrement(int x, int z);

    int get(int x, int z);

    virtual ~GridArray() = default;

    //debug
    virtual void print() const {}
protected:
    GridArray(const int g_size, const int a_size) : grid_size(g_size), array_size(a_size) {}

    virtual int &get_lvalue_reference(int x, int z) { int &i = x; return i; }
private:
    int &get_lvalue_reference_internal(int x, int z);
};



#endif //GRIDARRAY_H
