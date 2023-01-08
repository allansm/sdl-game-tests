project(box)

find_package(SDL2)

add_executable(box box.cpp)

target_link_libraries(box SDL2::SDL2) #SDL2::Image)
