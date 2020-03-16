find_package(OpenGL REQUIRED)

# SECTION INCLUDES

	# Include dependency headers
	#get_property(LIBRARY_INCLUDES TARGET Library PROPERTY INCLUDES)
	#target_include_directories(RenderEngine PRIVATE ${LIBRARY_INCLUDES})

	# Expose and include render engine headers
	set_property(TARGET RenderEngine PROPERTY CXX_STANDARD 17)
	set_property(TARGET RenderEngine PROPERTY INCLUDES
			${CMAKE_CURRENT_LIST_DIR}/include
			${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/include
			)
	get_property(RENDERENGINE_INCLUDES TARGET RenderEngine PROPERTY INCLUDES)
	target_include_directories(RenderEngine PUBLIC ${RENDERENGINE_INCLUDES})

	target_compile_definitions(RenderEngine PUBLIC BUILD_RENDERENGINE_DLL)

	# Include engine source (this is not exposed to other projects and is thus private
	target_include_directories(RenderEngine PRIVATE src ${PROJECT_SOURCE_DIR}/Dependencies/stb/include ${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/include ${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/include)
	add_subdirectory(src)

#ENDSECTION

#SECTION STATIC LIBS

	if(MSVC)
		if(BUILD_DEBUG)
			#message(FATAL_ERROR "No libs compile for msvc debug")
			set_property(TARGET RenderEngine PROPERTY LIBS
					${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/msvc142-x64/debug/lib/glew32.lib
					${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/msvc142-x64/debug/lib/glfw3.lib
					${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/msvc142-x64/debug/lib/assimp-vc142-mtd.lib
					${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/msvc142-x64/debug/lib/IrrXMLd.lib
					${PROJECT_SOURCE_DIR}/Dependencies/assimp-5.0.1/msvc142-x64/debug/lib/zlibstaticd.lib
					)
					set_property(TARGET RenderEngine PROPERTY BINS
					${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/msvc142-x64/debug/bin/glew32.dll
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for msvc release")
			set_property(TARGET RenderEngine PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	elseif(BUILD_MINGW)
		if(BUILD_DEBUG)
			set_property(TARGET RenderEngine PROPERTY LIBS
					${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
			set_property(TARGET RenderEngine PROPERTY BINS
					${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/bin/glew32d.dll
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for mingw release")
			set_property(TARGET RenderEngine PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	else()
		message(FATAL_ERROR "Render engine can only be built with msvc or mingw")
	endif()

	get_property(RENDERENGINE_LIBS TARGET RenderEngine PROPERTY LIBS)
	get_property(RENDERENGINE_BINS TARGET RenderEngine PROPERTY BINS)
	target_link_libraries(RenderEngine ${RENDERENGINE_LIBS} opengl32 Library)

#ENDSECTION

if(BUILD_PACKAGE)
	add_custom_target(PackageRenderEngine DEPENDS CreatePackageDirectories RenderEngine)
	add_custom_command(TARGET PackageRenderEngine POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:RenderEngine>"
				${CMAKE_BINARY_DIR}/Dist/
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_LINKER_FILE:RenderEngine>"
				${CMAKE_BINARY_DIR}/Dist/lib/
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${RENDERENGINE_BINS}"
				${CMAKE_BINARY_DIR}/Dist/
			)

	foreach(INCLUDES ${RENDERENGINE_INCLUDES})
		add_custom_command(TARGET PackageRenderEngine POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${INCLUDES}"
				${CMAKE_BINARY_DIR}/Dist/include)
	endforeach()
endif()