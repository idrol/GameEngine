# Expose these includes to dependencies
set_property(TARGET Physics PROPERTY CXX_STANDARD 17)
set_property(TARGET Physics PROPERTY INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
get_property(PHYSICS_INCLUDES TARGET Physics PROPERTY INCLUDES)
target_include_directories(Physics PUBLIC ${PHYSICS_INCLUDES} ${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/include)

target_include_directories(Physics PRIVATE src)
target_compile_definitions(Physics PUBLIC BUILD_PHYSICS_DLL)
add_subdirectory(src) # Do not use include as we are using target_sources which depencds on a correct cmake_source_dir variable

#SECTION STATIC LIBS

	if(MSVC)
		if(BUILD_DEBUG)
			#message(FATAL_ERROR "No libs compile for msvc debug")
			set_property(TARGET Physics PROPERTY LIBS
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet2FileLoader_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet3Collision_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet3Common_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet3Dynamics_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet3Geometry_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/Bullet3OpenCL_clew_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/BulletCollision_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/BulletDynamics_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/BulletFileLoader_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/BulletInverseDynamics_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/BulletInverseDynamicsUtils_Debug.lib
					${PROJECT_SOURCE_DIR}/Dependencies/bullet3-2.88/msvc142-x64/debug/lib/LinearMath_Debug.lib
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for msvc release")
			set_property(TARGET Physics PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	elseif(BUILD_MINGW)
		if(BUILD_DEBUG)
			message(FATAL_ERROR "No libs compile for mingw debug")
			set_property(TARGET Physics PROPERTY LIBS
			
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for mingw release")
			set_property(TARGET Physics PROPERTY LIBS

					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	else()
		message(FATAL_ERROR "Render engine can only be built with msvc or mingw")
	endif()

	get_property(PHYSICS_LIBS TARGET Physics PROPERTY LIBS)
	#get_property(PHYSICS_BINS TARGET Physics PROPERTY BINS)
	target_link_libraries(Physics ${PHYSICS_LIBS} Library)

#ENDSECTION

#SECTION Package library
if(BUILD_PACKAGE)
    add_custom_target(PackagePhysics DEPENDS CreatePackageDirectories Physics)
    add_custom_command(TARGET PackagePhysics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Physics>"
                 ${CMAKE_BINARY_DIR}/Dist/
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_LINKER_FILE:Physics>"
                 ${CMAKE_BINARY_DIR}/Dist/lib/
            )

    foreach(INCLUDES ${PHYSICS_INCLUDES})
        add_custom_command(TARGET PackagePhysics POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${INCLUDES}"
                ${CMAKE_BINARY_DIR}/Dist/include)
    endforeach()
endif()
#ENDSECTION