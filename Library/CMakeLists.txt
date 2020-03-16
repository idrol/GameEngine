# Expose these includes to dependencies
set_property(TARGET Library PROPERTY CXX_STANDARD 17)
set_property(TARGET Library PROPERTY INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include ${PROJECT_SOURCE_DIR}/Dependencies/glm/include)
get_property(LIBRARY_INCLUDES TARGET Library PROPERTY INCLUDES)
target_include_directories(Library PUBLIC ${LIBRARY_INCLUDES})

target_include_directories(Library PRIVATE src)
target_compile_definitions(Library PUBLIC BUILD_LIBRARY_DLL)
add_subdirectory(src) # Do not use include as we are using target_sources which depencds on a correct cmake_source_dir variable


#SECTION Package library
if(BUILD_PACKAGE)
    add_custom_target(PackageLibrary DEPENDS CreatePackageDirectories Library)
    add_custom_command(TARGET PackageLibrary POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Library>"
                 ${CMAKE_BINARY_DIR}/Dist/
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_LINKER_FILE:Library>"
                 ${CMAKE_BINARY_DIR}/Dist/lib/
            )

    foreach(INCLUDES ${LIBRARY_INCLUDES})
        add_custom_command(TARGET PackageLibrary POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${INCLUDES}"
                ${CMAKE_BINARY_DIR}/Dist/include)
    endforeach()
endif()
#ENDSECTION