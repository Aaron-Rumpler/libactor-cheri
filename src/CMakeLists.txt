find_package(Threads REQUIRED)

SET(LIBRARY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(list SHARED list.c)
set_target_properties(list PROPERTIES VERSION 0.0.1 SOVERSION 1)
target_include_directories(list PUBLIC $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

add_library(actor SHARED actor.c)
set_target_properties(actor PROPERTIES VERSION 0.0.1 SOVERSION 1)
target_include_directories(actor PUBLIC $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
target_link_libraries(actor PRIVATE list Threads::Threads)

install(TARGETS list actor DESTINATION lib)
install(DIRECTORY ${LIBRARY_INCLUDE_DIR}/libactor DESTINATION include)
