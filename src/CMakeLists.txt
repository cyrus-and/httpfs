cmake_minimum_required( VERSION 2.8 )
find_package( PkgConfig )

project( phpfs )

file( GLOB fuse_api fuse_api/*.c )
file( GLOB core *.c )
add_executable( phpfs ${core} ${fuse_api} template.php.h )

add_definitions( -Wall )

find_package( CURL )
pkg_check_modules( FUSE REQUIRED fuse )
add_definitions( ${FUSE_CFLAGS} -DFUSE_USE_VERSION=29 )

target_link_libraries( phpfs ${CURL_LIBRARY} ${FUSE_LIBRARIES} )

add_custom_command(
  OUTPUT template.php.h
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/template.php template.php
  COMMAND xxd -i template.php template.php.h
  DEPENDS template.php
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_custom_target(
  test-local
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../test/test-local.sh
  DEPENDS phpfs
)

install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/phpfs DESTINATION bin )
