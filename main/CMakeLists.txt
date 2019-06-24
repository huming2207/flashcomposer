file(GLOB_RECURSE FC_INCLUDES LIST_DIRECTORIES true
        "include"
        "hardware/include"
        "sys/include"
        "web/include"
        "third_party/arduino_json")
file(GLOB_RECURSE FC_SOURCES "*.cpp" "hardware/*.cpp" "sys/*.cpp" "web/*.cpp" "third_party/*.cpp")
file(GLOB_RECURSE FC_HEADERS "*.hpp" "hardware/*.hpp" "sys/*.hpp" "web/*.hpp" "third_party/*.h" "third_party/*.hpp")

set(COMPONENT_SRCS ${FC_HEADERS} ${FC_SOURCES})
set(COMPONENT_ADD_INCLUDEDIRS "" ${FC_INCLUDES})

register_component()