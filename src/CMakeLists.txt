project( httpfs )

# CMake setup
cmake_minimum_required( VERSION 2.8 )
find_package( PkgConfig )

# sources
file( GLOB fuse_api fuse_api/*.c )
file( GLOB core *.c )
file( GLOB templates RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/templates/ templates/* )

# process templates
set( templates_h "" )
foreach( template ${templates} )
set( template_h ${template}.h )
list( APPEND templates_h ${template_h} )
add_custom_command(
  OUTPUT ${template_h}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/templates/${template} ${template}
  COMMAND xxd -i ${template} ${template_h}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/templates/${template}
)
endforeach( template )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# external libraries
find_package( CURL )
pkg_check_modules( FUSE REQUIRED fuse )
add_definitions( ${FUSE_CFLAGS} -DFUSE_USE_VERSION=29 )

# compilation
add_executable( httpfs ${core} ${fuse_api} ${templates_h} )
add_definitions( -Wall )
target_link_libraries( httpfs ${CURL_LIBRARY} ${FUSE_LIBRARIES} )

# aux targets
add_custom_target(
  test-local
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../test/test-local.sh
  DEPENDS httpfs
)

install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/httpfs DESTINATION bin )
