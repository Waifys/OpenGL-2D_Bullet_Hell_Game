cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Tell GLFW not to build examples/tests (faster build)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Add GLFW from your local folder
add_subdirectory(lib/glfw)


# GLAD (manual source)
add_library(glad lib/glad/src/glad.c)

target_include_directories(glad PUBLIC lib/glad/include)

add_executable(MyApp src/main.cpp)

target_include_directories(MyApp PRIVATE lib/glad/include)

target_link_libraries(MyApp PRIVATE
    glfw
    glad
    opengl32
)
