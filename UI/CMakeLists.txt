set_property(TARGET UI PROPERTY CXX_STANDARD 17)
set_property(TARGET UI PROPERTY INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
get_property(UI_INCLUDES TARGET UI PROPERTY INCLUDES)
target_include_directories(UI PUBLIC ${UI_INCLUDES} ${PROJECT_SOURCE_DIR}/Dependencies/freetype-2.9.1/include)

target_include_directories(UI PRIVATE src)
target_compile_definitions(UI PUBLIC BUILD_UI_DLL)
add_subdirectory(src)

#SECTION STATIC LIBS

	if(MSVC)
		if(BUILD_DEBUG)
			#message(FATAL_ERROR "No libs compile for msvc debug")
			set_property(TARGET UI PROPERTY LIBS
					${PROJECT_SOURCE_DIR}/Dependencies/freetype-2.9.1/msvc142-x64/debug/lib/freetyped.lib
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for msvc release")
			set_property(TARGET UI PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	elseif(BUILD_MINGW)
		if(BUILD_DEBUG)
			message(FATAL_ERROR "No libs compile for mingw debug")
			set_property(TARGET UI PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		elseif(BUILD_RELEASE)
			message(FATAL_ERROR "No libs compile for mingw release")
			set_property(TARGET UI PROPERTY LIBS
					#${PROJECT_SOURCE_DIR}/Dependencies/glew-2.1.0/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglew32d.dll.a
					#${PROJECT_SOURCE_DIR}/Dependencies/glfw-3.3.1/mingw-w64-x86_64-7.2.0-posix-seh/debug/lib/libglfw3.a
					)
		else()
			message(FATAL_ERROR "Unrecognized build type ${CMAKE_BUILD_TYPE}")
		endif()
	else()
		message(FATAL_ERROR "Render engine can only be built with msvc or mingw")
	endif()

	get_property(UI_LIBS TARGET UI PROPERTY LIBS)
	target_link_libraries(UI  ${UI_LIBS} RenderEngine opengl32 Library)

#ENDSECTION

if(BUILD_PACKAGE)
	add_custom_target(PackageUI DEPENDS CreatePackageDirectories UI)
	add_custom_command(TARGET PackageUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:UI>"
                 ${CMAKE_BINARY_DIR}/Dist/
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_LINKER_FILE:UI>"
                 ${CMAKE_BINARY_DIR}/Dist/lib/
            )

	foreach(INCLUDES ${UI_INCLUDES})
		add_custom_command(TARGET PackageUI POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${INCLUDES}"
				${CMAKE_BINARY_DIR}/Dist/include)
	endforeach()
endif()