add_executable(capability_sharing capability_sharing.c)
target_link_libraries(capability_sharing actor)
add_custom_command(TARGET capability_sharing POST_BUILD COMMAND elfctl -e +cheric18n $<TARGET_FILE:capability_sharing>)

add_executable(message_editing message_editing.c)
target_link_libraries(message_editing actor)
add_custom_command(TARGET message_editing POST_BUILD COMMAND elfctl -e +cheric18n $<TARGET_FILE:message_editing>)

