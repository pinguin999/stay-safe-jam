cmake_minimum_required(VERSION 3.6)

project(ggj2020 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")

add_subdirectory(subprojects/jngl)
add_subdirectory(subprojects/Box2D)

file(GLOB SOURCES
	src/*.cpp
	src/engine/*.cpp
	src/engine/effects/*.cpp
	src/gui/*.cpp
)
if(ANDROID)
	# build native_app_glue as a static lib
	set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
	add_library(native_app_glue STATIC
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

	# now build app's shared lib
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

	# Export ANativeActivity_onCreate(),
	# Refer to: https://github.com/android-ndk/ndk/issues/381.
	set(CMAKE_SHARED_LINKER_FLAGS
		"${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

	add_library(ggj2020 SHARED ${SOURCES})

	target_include_directories(ggj2020 PRIVATE
		${ANDROID_NDK}/sources/android/native_app_glue
		../../../../../../src
		../../../../../include
	)

	# add lib dependencies
	target_link_libraries(ggj2020
		android
		native_app_glue
		EGL
		GLESv3
		OpenSLES
		log
		z
		jngl
	)


	add_library(bootstrap SHARED android/app/src/main/cpp/bootstrap.cpp)

	target_include_directories(bootstrap PRIVATE
		${ANDROID_NDK}/sources/android/native_app_glue)

	# add lib dependencies
	target_link_libraries(bootstrap
		android
		native_app_glue
		log)
else()
	add_executable(ggj2020 ${SOURCES})
	if(APPLE)
		find_package(Boost COMPONENTS filesystem)
		target_link_libraries(ggj2020 PRIVATE "-framework AppKit")
	elseif(NOT WIN32)
		target_link_libraries(ggj2020 PRIVATE
			stdc++fs
		)
	endif()
	target_link_libraries(ggj2020 PRIVATE
		${Boost_LIBRARIES} jngl box2d
	)
endif()

file(GLOB GFX_FILES RELATIVE ${CMAKE_SOURCE_DIR}/data data/gfx/*.webp data/gfx/*.png
                                                      data/gfx/*/*.webp data/gfx/*/*.png)
file(GLOB SFX_FILES RELATIVE ${CMAKE_SOURCE_DIR}/data data/sfx/*.ogg)
configure_file(src/engine/datafiles.hpp.in include/datafiles.hpp @ONLY)
target_include_directories(ggj2020 PRIVATE ${CMAKE_BINARY_DIR}/include subprojects/cereal/include)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(ggj2020 PROPERTIES WIN32_EXECUTABLE 1)
endif()
