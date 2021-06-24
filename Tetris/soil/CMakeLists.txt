add_library(soil
        src/image_DXT.c
        src/image_helper.c
        src/stb_image_aug.c
        src/SOIL.c
        )

target_include_directories(soil PUBLIC
        include)



#add_library(soil_interface INTERFACE)
#add_library(soil STATIC IMPORTED)
#add_library(soil INTERFACE)

#target_include_directories(soil_interface INTERFACE
#        include)

#target_link_libraries(soil INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/SOIL.lib)
#set_property(TARGET soil PROPERTY IMPORTED_LOCATION ${CMAKE_PROJECT_SOURCE_DIR}/soil/lib/libSOIL.lib)

#target_link_libraries(soil INTERFACE soil_interface)
#target_link_libraries(soil INTERFACE ${CMAKE_PROJECT_SOURCE}/lib/libSOIL)