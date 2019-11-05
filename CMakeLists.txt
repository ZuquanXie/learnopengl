cmake_minimum_required(VERSION 3.0)

project(LearnOpengl VERSION 1.0)

# common includes and libs
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")
list(APPEND EXTRA_SOURCES "${PROJECT_SOURCE_DIR}/include/glad/glad.c")
list(APPEND EXTRA_LIBS glfw3)

# ContextProvider
add_subdirectory(ContextProvider)
list(APPEND EXTRA_LIBS ContextProvider)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/ContextProvider")

# Shader class
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Shader")

add_executable(LearnOpengl main.cpp ${EXTRA_SOURCES})

target_include_directories(LearnOpengl PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(LearnOpengl PUBLIC ${EXTRA_LIBS})

# copy shader source files to build directory
add_custom_command(TARGET LearnOpengl POST_BUILD
	COMMAND ${PROJECT_SOURCE_DIR}/copyShaderSource ARGS ${PROJECT_SOURCE_DIR}/shaderSource ${PROJECT_BINARY_DIR}/shaderSource
)