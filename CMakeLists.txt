cmake_minimum_required(VERSION 3.0)

project(LearnOpengl VERSION 1.0)

# common includes and libs
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")
list(APPEND EXTRA_SOURCES "${PROJECT_SOURCE_DIR}/include/glad/glad.c")
list(APPEND EXTRA_LIBS glfw3)

# stb_image
list(APPEND EXTRA_SOURCES "${PROJECT_SOURCE_DIR}/include/stb/stb_image.cpp")

# ContextProvider
add_subdirectory(ContextProvider)
list(APPEND EXTRA_LIBS ContextProvider)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/ContextProvider")

# Shader class
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Shader")

# Camera class
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Camera")

# main.exe
add_executable(main main.cpp ${EXTRA_SOURCES})

target_include_directories(main PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(main PUBLIC ${EXTRA_LIBS})

# copy shader source files to build directory
add_custom_command(TARGET main POST_BUILD
	COMMAND ${PROJECT_SOURCE_DIR}/copyResource ARGS ${PROJECT_SOURCE_DIR}/resource ${PROJECT_BINARY_DIR}/resource
)

# test.exe
add_executable(test test.cpp)
target_include_directories(test PUBLIC ${EXTRAL_INCLUDES})