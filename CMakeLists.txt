cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 11)
file(GLOB SOURCES
        *.h
        *.cpp
        grid/array/*.cpp
        grid/array/*.h
        vector2/*.cpp
        vector2/*.h
        projectile/*.cpp
        projectile/*.h
        collision/*.cpp
        collision/*.h
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
        fill_reachable_cells.h
        grid/array/GridArray.cpp)

target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/gdextension")
target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/gen/include")
target_include_directories(src PRIVATE "C:/Tools/godot/gdext_orig_tut/gdextension_cpp_example/godot-cpp/include")