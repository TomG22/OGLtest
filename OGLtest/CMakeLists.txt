cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(OGLtest)

# add_compile_options(-Wall)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED OFF CACHE BOOL "" FORCE) # Ensure static linking
set(GLEW_STATIC ON)  # Tell GLEW that we want to use static linking
add_definitions(-DGLEW_STATIC)

link_directories(Dependencies/GLFW/lib)
link_directories(Dependencies/GLEW/lib/Release/x64)
link_directories(C:/mingw64/x86_64-w64-mingw32/lib)

add_executable(${PROJECT_NAME}
    OGLtest/src/app.cpp
    OGLtest/src/IndexBuffer.cpp
    OGLtest/src/Renderer.cpp
    OGLtest/src/Shader.cpp
    OGLtest/src/Texture.cpp
    OGLtest/src/VertexArray.cpp
    OGLtest/src/VertexBuffer.cpp

    OGLtest/src/tests/Test.cpp
    OGLtest/src/tests/TestClearColor.cpp
    OGLtest/src/tests/TestTexture2D.cpp

    OGLtest/src/vendor/imgui/imgui.cpp
    OGLtest/src/vendor/imgui/imgui_draw.cpp
    OGLtest/src/vendor/imgui/imgui_tables.cpp
    OGLtest/src/vendor/imgui/imgui_widgets.cpp
    OGLtest/src/vendor/imgui/imgui_impl_glfw.cpp
    OGLtest/src/vendor/imgui/imgui_impl_opengl3.cpp

    OGLtest/src/vendor/stb_image/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    OGLtest/src
    OGLtest/src/tests
    OGLtest/src/vendor
    Dependencies/GLFW/include
    Dependencies/GLEW/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw3
    ${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib
    opengl32
)