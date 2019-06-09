file(GLOB_RECURSE FC_INCLUDES LIST_DIRECTORIES true "include" "lib/include")
file(GLOB_RECURSE FC_SOURCES "*.cpp" "lib/*.cpp")
file(GLOB_RECURSE FC_HEADERS "*.hpp" "lib/*.hpp")

set(COMPONENT_SRCS ${FC_HEADERS} ${FC_SOURCES})
set(COMPONENT_ADD_INCLUDEDIRS "" ${FC_INCLUDES})

register_component()