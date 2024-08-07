project(client)

set(CMAKE_CXX_STANDARD 17)

add_executable(client client_main.cpp client.cpp)
target_link_libraries(client PRIVATE common_pch)
