project(box)

find_package(SDL2)

add_executable(box box.cpp)
add_executable(collision collision.cpp)

target_link_libraries(box SDL2::SDL2)
target_link_libraries(collision SDL2::SDL2)
