add_executable(example example.c)
target_link_libraries(example actor)
add_custom_command(TARGET example POST_BUILD COMMAND elfctl -e +cheric18n $<TARGET_FILE:example>)

add_executable(http_server http_server.c)
target_link_libraries(http_server actor)
add_custom_command(TARGET example POST_BUILD COMMAND elfctl -e +cheric18n $<TARGET_FILE:example>)
