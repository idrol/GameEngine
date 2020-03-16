cmake_minimum_required(VERSION 3.13) # Needs to be 3.13 to allow nicer source configuration features
project(Engine)

option(CMAKE_BUILD_TYPE "The build type" Debug)

# Check for present global variables defining different compile environments
# NOTE Mingw does not have a defined global variable so the assumption must be made that if MSVC, CYGWIN and UNIX is not present we are on a mingw toolchain
#	This can still be a mingw 32 bit compile or mingw with win32 api or any other combination that cannot be reliably checked
if(MSVC)
	message(STATUS "Using Microsoft Visual C++")
elseif(CYGWIN)
	message(FATAL_ERROR "CYGWIN not supported")
elseif(UNIX)
	message(FATAL_ERROR "Unix platform detected currently not supported")
else()
	message(STATUS "Neither MSVC, CYGWIN or unix platform detected assuming MINGW-W64 7.2.0 posix-seh NOTE build will fail if this assumption is not correct")
	add_compile_definitions(_WIN32_WINNT=0x0601)
	add_compile_definitions(WINVER=0x0601)
	set(BUILD_MINGW 1)
endif()

#Global compile flags
#TODO refactor these into targets on a per dependecy basis as to not clutter all projects with these
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-parentheses -Wno-strict-aliasing -msse2 -Werror=return-type")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wno-parentheses -Wno-strict-aliasing -msse2 -Werror=return-type") # No parentheses is required from boost as this mostly complains about parentheses that improve code readability

# Prevents console window from opening
if(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Building engine with release flags and running compile optimizations")
	#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -Wl,-allow-multiple-definition")
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ftree-vectorize -ffast-math -ftree-slp-vectorize ") #-ftree-vectorizer-verbose=1
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -ffast-math -ftree-slp-vectorize") #-fopt-info-vec-missed
	set(BUILD_RELEASE 1)
	# Global compile defines
	add_compile_definitions(DEBUG=0)
	add_compile_definitions(RELEASE=1)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Building engine with debug flags")
	# Global compile defines
	add_compile_definitions(DEBUG=1)
	add_compile_definitions(RELEASE=0)
	#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-allow-multiple-definition")

        # fstack-protector enables stack protection checks that make sure that the stack isn't invalidated
        # Wno-format because %zl throws a warning for unknown format even though it is valid
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wno-sign-compare -Wno-unused-variable -Wno-format")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wno-sign-compare -Wno-unused-variable -Wno-format")
	set(BUILD_DEBUG 1)
else()
	message(FATAL_ERROR "Unsupported build type")
endif()



option(BUILD_FRAMEWORK_NETWORKING "Enable networking in the framework library")
option(BUILD_SERVER_FRAMEWORK "Should the framework library be targeted for servers. (Doesn't build the rendering and client side libraries)")

# Base Engine library
add_library(Library SHARED)
add_subdirectory(Library)

# Networking lib
add_library(Networking SHARED)
add_subdirectory(Networking)
add_dependencies(Networking Library)

# Render Engine
add_library(RenderEngine SHARED)
add_subdirectory(RenderEngine)
add_dependencies(RenderEngine Library CopyAssets) #RenderEngine require engine assets
# Below is a hack due to glew being dynamic linked
get_property(RenderEngineBINS TARGET RenderEngine PROPERTY BINS)
foreach(BIN ${RenderEngineBINS})
	add_custom_command(TARGET RenderEngine POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${BIN}"
			$<TARGET_FILE_DIR:RenderEngine>)
endforeach()

# Physics lib
add_library(Physics SHARED)
add_subdirectory(Physics)
add_dependencies(Physics Library)

# UI lib
add_library(UI SHARED)
add_subdirectory(UI)
add_dependencies(UI RenderEngine Library)

# Framework lib
add_library(Framework SHARED)
add_subdirectory(Framework)
if(BUILD_FRAMEWORK_NETWORKING)
	if(BUILD_SERVER_FRAMEWORK)
		add_dependencies(Framework Networking Physics Library)
	else()
		add_dependencies(Framework Networking UI Physics RenderEngine Library)
	endif()
else()
	add_dependencies(Framework UI Physics RenderEngine Library)
endif()

add_custom_target(CopyAssets)
add_custom_command(TARGET CopyAssets POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/Assets"
		${CMAKE_BINARY_DIR}/Assets)
add_dependencies(CopyAssets CopyNoDistAssets)
add_custom_target(CopyNoDistAssets)
add_custom_command(TARGET CopyNoDistAssets POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/AssetsNoDist"
		${CMAKE_BINARY_DIR}/AssetsNoDist
)
option(BUILD_TESTS "Build test executables")
if(BUILD_TESTS)
	# RenderEngineTest executable
	add_executable(RenderEngineTest RenderEngine/test/main.cpp)
	set_property(TARGET RenderEngineTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}) #Redirect exe file to build root
	target_link_libraries(RenderEngineTest RenderEngine)
	add_subdirectory(RenderEngine/test)

	#RenderEngineClassroom
	add_executable(RenderEngineClassroom RenderEngine/classroom/main.cpp)
	set_property(TARGET RenderEngineClassroom PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}) #Redirect exe file to build root
	target_link_libraries(RenderEngineClassroom RenderEngine)
	add_dependencies(RenderEngineClassroom CopyNoDistAssets)
	add_subdirectory(RenderEngine/classroom)

	# Networking ClientTest
	add_executable(NetworkClientTest Networking/ClientTest/main.cpp)
	set_property(TARGET NetworkClientTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
	target_link_libraries(NetworkClientTest Networking)
	add_subdirectory(Networking/ClientTest)

	# Networking ServerTest
	add_executable(NetworkServerTest Networking/ServerTest/main.cpp)
	set_property(TARGET NetworkServerTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
	target_link_libraries(NetworkServerTest Networking)
	add_subdirectory(Networking/ServerTest)

	# Framework Test
	add_executable(FrameworkTest Framework/test/main.cpp)
	set_property(TARGET FrameworkTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
	target_link_libraries(FrameworkTest Framework)
	add_subdirectory(Framework/test)

	# Library Test
	add_executable(LibraryTests Library/Tests/main.cpp)
	set_property(TARGET LibraryTests PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
	target_link_libraries(LibraryTests Library)
	add_subdirectory(Library/Tests)

	#UI Test
	add_executable(UITests UI/test/main.cpp)
	set_property(TARGET UITests PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/UITest)
	# Copy dependency files here
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/Library.dll" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/Library.pdb" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/UI.dll" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/UI.pdb" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/RenderEngine.dll" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/RenderEngine.pdb" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/glew32.dll" ${PROJECT_BINARY_DIR}/UITest)
	add_custom_command(TARGET UITests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" ${PROJECT_BINARY_DIR}/UITest/Assets)
	target_link_libraries(UITests UI)
	add_subdirectory(UI/test)

endif()

message(INFO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY} test")

option(BUILD_PACKAGE "Enable targets for distribution packaging")
if(BUILD_PACKAGE)
	add_custom_target(CreatePackageDirectories)
	add_custom_command(TARGET CreatePackageDirectories POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Dist
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Dist/lib
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Dist/include
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${CMAKE_CURRENT_SOURCE_DIR}/Assets"
				${CMAKE_BINARY_DIR}/Dist/Assets
			)

	#if(BUILD_FRAMEWORK_NETWORKING)
	#	if(BUILD_SERVER_FRAMEWORK)
	#		add_custom_target(PackageEngine DEPENDS PackageFramework PackageNetworking PackagePhysics PackageLibrary)
	#	else()
			add_custom_target(PackageEngine DEPENDS PackageFramework PackageUI PackageNetworking PackagePhysics PackageRenderEngine PackageLibrary)
		#endif()
	#else()
	#	add_custom_target(PackageEngine DEPENDS PackageFramework PackageUI PackagePhysics PackageRenderEngine PackageLibrary)
	#endif()
endif()
