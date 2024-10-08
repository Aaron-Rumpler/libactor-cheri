add_executable(example example.c)
target_link_libraries(example actor)

add_executable(http_server http_server.c)
target_link_libraries(http_server actor)
