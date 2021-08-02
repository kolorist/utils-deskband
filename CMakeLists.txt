# 1. required version
cmake_minimum_required(VERSION 3.4.1)

# 2. initial setup
set (PROJECT_NAME "utils-deskband")
project (${PROJECT_NAME})
include ("${PROJECT_SOURCE_DIR}/project_configs.cmake")
message (STATUS "Project: ${PROJECT_NAME}")
message (STATUS "Executable: ${EXECUTABLE_FILE_NAME}.dll")
message (STATUS "Project source directory: ${PROJECT_SOURCE_DIR}")
message (STATUS "Project binary directory: ${PROJECT_BINARY_DIR}")

# 4. build commands output
if (${USE_MSVC_PROJECT})
	message (STATUS "MSVC Solution / Project structure will be generated")
endif ()

# 5.1 file listing
if (${USE_MSVC_PROJECT})
	file (GLOB_RECURSE file_list
		LIST_DIRECTORIES false
		"${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/src/*.def"
		"${PROJECT_SOURCE_DIR}/src/*.c"
		"${PROJECT_SOURCE_DIR}/src/*.h")
else ()
	file (GLOB_RECURSE file_list
		LIST_DIRECTORIES false
		"${PROJECT_SOURCE_DIR}/src/*.c"
		"${PROJECT_SOURCE_DIR}/src/*.def"
		"${PROJECT_SOURCE_DIR}/src/*.cpp")
endif ()

# 6. platform specific compiling
add_compile_definitions(_CRT_SECURE_NO_WARNINGS UNICODE _UNICODE WIN32_LEAN_AND_MEAN)

# 7. C and CXX global compile options

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -frtti -fno-exceptions -fcolor-diagnostics -fansi-escape-codes")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -fcolor-diagnostics -fansi-escape-codes")

# 8. subdirectories
add_subdirectory (externals/floral)
add_subdirectory (externals/helich)
add_subdirectory (externals/clover)

# 9. platform specific linking
add_library (${PROJECT_NAME} SHARED ${file_list})
target_link_libraries (${PROJECT_NAME} 
	floral
	helich
	clover
	comctl32
	UxTheme)
set (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Xlinker /subsystem:console")
set_target_properties (${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_FILE_NAME})

# 11. misc
if (${USE_MSVC_PROJECT})
	# organize filters
	foreach(_source IN ITEMS ${file_list})
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${PROJECT_SOURCE_DIR}" "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
	# TODO: startup project and working directory
endif ()
