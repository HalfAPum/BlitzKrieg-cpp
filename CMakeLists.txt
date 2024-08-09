cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 11)
file(GLOB SOURCES
        *.h
        *.cpp
        grid_array/*.cpp
        grid_array/*.h
)

add_executable(src ${SOURCE_FILES} ${SOURCES}
        Constants.h
        Terrain3D.h
        Battlefield.h
        Battlefield.cpp
        Battlefield.h
        UnitGridXXS.h
        UnitGridXS.h
        grid/array/GridArray.h
        fill_initial_adjacent_cells_algorithm.h
        grid/array/GridArray.cpp)
#add_executable(src
#        gdexample.cpp
#        gdexample.h
#        register_types.cpp
#        register_types.h
#        Test3D.h
#        Test3D.cpp
#        BlitzUnit.h
#        BlitzUnit.cpp
#)

target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/gdextension")
target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/gen/include")
target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/include")