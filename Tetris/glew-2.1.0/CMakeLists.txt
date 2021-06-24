add_library(glew INTERFACE)

target_include_directories(glew INTERFACE
        include)

target_link_libraries(glew INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/Release/x64/glew32s.lib)
target_compile_definitions(glew INTERFACE GLEW_STATIC)

#set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${CMAKE_PROJECT_SOURCE_DIR}/lib/libSOIL.lib)
