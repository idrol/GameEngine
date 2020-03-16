# Expose these includes to dependencies
set_property(TARGET Framework PROPERTY CXX_STANDARD 17)
set_property(TARGET Framework PROPERTY INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include ${PROJECT_SOURCE_DIR}/Dependencies/glm/include)
get_property(Framework_INCLUDES TARGET Framework PROPERTY INCLUDES)
target_include_directories(Framework PUBLIC ${Framework_INCLUDES})

target_include_directories(Framework PRIVATE src)
target_compile_definitions(Framework PUBLIC BUILD_FRAMEWORK_DLL)

add_subdirectory(src) # Do not use include as we are using target_sources which depencds on a correct cmake_source_dir variable

if(BUILD_FRAMEWORK_NETWORKING)
    target_compile_definitions(Framework PUBLIC BUILD_NETWORKING)
	if(BUILD_SERVER_FRAMEWORK)
        target_compile_definitions(Framework PUBLIC BUILD_SERVER)
		target_link_libraries(Framework Networking Physics Library)
	else()
        target_compile_definitions(Framework PUBLIC BUILD_CLIENT)
		target_link_libraries(Framework Networking UI Physics RenderEngine Library)
	endif()
else()
    target_compile_definitions(Framework PUBLIC BUILD_CLIENT)
	target_link_libraries(Framework UI Physics RenderEngine Library)
endif()

#SECTION Package Framework
if(BUILD_PACKAGE)
    add_custom_target(PackageFramework DEPENDS CreatePackageDirectories Framework)
    add_custom_command(TARGET PackageFramework POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Framework>"
                 ${CMAKE_BINARY_DIR}/Dist/
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_LINKER_FILE:Framework>"
                 ${CMAKE_BINARY_DIR}/Dist/lib/
            )

    foreach(INCLUDES ${Framework_INCLUDES})
        add_custom_command(TARGET PackageFramework POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${INCLUDES}"
                ${CMAKE_BINARY_DIR}/Dist/include)
    endforeach()
endif()
#ENDSECTION