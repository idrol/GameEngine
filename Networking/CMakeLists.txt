set_property(TARGET Networking PROPERTY CXX_STANDARD 17)
set_property(TARGET Networking PROPERTY INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
get_property(NETWORKING_INCLUDES TARGET Networking PROPERTY INCLUDES)
target_include_directories(Networking PUBLIC ${NETWORKING_INCLUDES})

target_include_directories(Networking PRIVATE src)
target_compile_definitions(Networking PUBLIC BUILD_NETWORK_DLL)
add_subdirectory(src)

#get_property(NETWORKING_LIBS TARGET Networking PROPERTY LIBS)
#get_property(NETWORKING_BINS TARGET Networking PROPERTY BINS)
if(WIN32)
	target_link_libraries(Networking ${NETWORKING_LIBS} Library wsock32 ws2_32)
else()
	target_link_libraries(Networking ${NETWORKING_LIBS} Library)
endif()

if(BUILD_PACKAGE)
	add_custom_target(PackageNetworking DEPENDS CreatePackageDirectories Networking)
	add_custom_command(TARGET PackageNetworking POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:Networking>"
				${CMAKE_BINARY_DIR}/Dist/
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_LINKER_FILE:Networking>"
				${CMAKE_BINARY_DIR}/Dist/lib/
			#COMMAND ${CMAKE_COMMAND} -E copy_if_different
			#	"${NETWORKING_BINS}"
			#	$<TARGET_FILE_DIR:Networking>/Dist/
			)

	foreach(INCLUDES ${NETWORKING_INCLUDES})
		add_custom_command(TARGET PackageNetworking POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${INCLUDES}"
				${CMAKE_BINARY_DIR}/Dist/include)
	endforeach()

endif()