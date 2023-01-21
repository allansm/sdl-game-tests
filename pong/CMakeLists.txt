project(box)

find_package(SDL2)

add_executable(pong pong.cpp)

target_link_libraries(pong SDL2::SDL2)
