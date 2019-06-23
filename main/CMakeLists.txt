file(GLOB_RECURSE FC_INCLUDES LIST_DIRECTORIES true "include" "hardware/include" "third_party/arduino_json")
file(GLOB_RECURSE FC_SOURCES "*.cpp" "hardware/*.cpp" "third_party/*.cpp")
file(GLOB_RECURSE FC_HEADERS "*.hpp" "hardware/*.hpp" "third_party/*.h" "third_party/*.hpp")

set(COMPONENT_SRCS ${FC_HEADERS} ${FC_SOURCES})
set(COMPONENT_ADD_INCLUDEDIRS "" ${FC_INCLUDES})

register_component()