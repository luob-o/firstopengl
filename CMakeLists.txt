
cmake_minimum_required(VERSION 3.20)

project(terrain_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 兼容旧版 SOIL 的 CMake 配置
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)

# =========================
# SOIL
# =========================
FetchContent_Declare(
    soil
    GIT_REPOSITORY https://github.com/kbranigan/Simple-OpenGL-Image-Library.git
    GIT_TAG master
)

set(SOIL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(soil)

# miniaudio


FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.25
)

FetchContent_MakeAvailable(miniaudio)



# =========================
# GLM
# =========================
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# =========================
# GLFW
# =========================
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# =========================
# GLAD
# =========================
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)

set(GLAD_SPEC "gl" CACHE STRING "" FORCE)
set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
set(GLAD_API "gl=3.3" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c" CACHE STRING "" FORCE)
set(GLAD_REPRODUCIBLE ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glad)

# =========================
# OpenGL
# =========================
find_package(OpenGL REQUIRED)

# =========================
# Demo common function
# =========================
function(add_opengl_demo target_name source_file)

    add_executable(${target_name}
        ${source_file}
    )

    target_include_directories(${target_name} PRIVATE
        ${soil_SOURCE_DIR}/src
    )

    target_link_libraries(${target_name} PRIVATE
        glm::glm
        glad
        glfw
        SOIL
        miniaudio
        OpenGL::GL
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /utf-8)
    endif()

endfunction()

# =========================
# Main Project
# =========================
add_opengl_demo(terrain_demo src/main.cpp)

# =========================
# Learning Demos
# =========================
add_opengl_demo(demo_clear demos/demo_clear.cpp)

add_opengl_demo(box demos/box.cpp)
add_opengl_demo(logoTV demos/logoTV.cpp)
add_opengl_demo(firstTD demos/firstTD.cpp)
add_opengl_demo(secondTD demos/secondTD.cpp)
add_opengl_demo(td3 demos/td3.cpp)
add_opengl_demo(onlym demos/onlym.cpp)
add_opengl_demo(camera demos/camera.cpp)
