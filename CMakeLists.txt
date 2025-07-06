# REF:
# https://medium.com/@atkinssamuel/cmake-the-devil-you-know-9be0e963e5d6
# https://dev.to/iblancasa/learning-cmake-2-43ga
# https://cmake.org/

cmake_minimum_required(VERSION 3.16)
project(MyGameEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# append to the already existing variable "CMAKE_MODULE_PATH" 
# the folder which contains cmake modules (it's empty but you never know later on)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ensure OpenGL is found "before" FetchContent for some cases, 
# though not strictly required here
find_package(OpenGL REQUIRED)

# just a quick usefull function
function(hide_target_if_exists name)
    if(TARGET ${name})
        set_target_properties(${name} PROPERTIES FOLDER "Hidden/Internal")
    endif()
endfunction()

# ------------ FetchContent setup ------------ #
# top-level CMakeLists.txt tells CMake to use FetchContent to download the source code 
# for all these GitHub repositories into the build directory 
# (typically _deps subfolder within your build folder)
include(FetchContent)

# ---- GLFW ----
FetchContent_Declare(
    glfw_repo
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
# fetchContent_MakeAvailable makes the target glfw_repo available in this build
# its include directories and libraries are automatically linked when 
# target_link_libraries(MyGameEngine glfw) is called

# this essentially tells CMake to process the CMakeLists.txt file within this downloaded repository
# and to start their own build instructions
FetchContent_MakeAvailable(glfw_repo)

# ---- GLAD ----
FetchContent_Declare(
    glad_repo 
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad_repo)

# ---- GLM (Header-only library) ----
FetchContent_Declare(
    glm_repo 
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm_repo)

# ---- stb_image ----
FetchContent_Declare(
    stb_repo
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb_repo)

# ---- ImGui (NO CMAKE file) ----
FetchContent_Declare(
    imgui_repo 
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui_repo)

# ---- Assimp ----
FetchContent_Declare(
    assimp_repo
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)         # don't build tests
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)  # don't build tools
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)       # don't build samples
FetchContent_MakeAvailable(assimp_repo)

# ---- Adds source files ----
file(GLOB_RECURSE ENGINE_SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Classes/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Classes/UI/*.cpp" 
    # header files are found via include directories
)

# ---- Adds main executable and assets ----
add_executable(MyGameEngine ${ENGINE_SRC_FILES})

# finds all files ending with .glsl in the shaders directory and its subdirectories
file(GLOB_RECURSE SHADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/default/*.glsl"
)
# the shader files as a dependency for the executable
# If any of these files change, CMake will consider 'MyGameEngine' out of date.
add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:MyGameEngine>/shaders
    DEPENDS ${SHADER_FILES}
    COMMENT "Copying shaders to build directory on shader change"
)

# copy shaders to the install prefix
# this command runs when you run 'cmake --install <build_directory>'
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/
    DESTINATION shaders
    # Group these files under a 'Runtime' component if you use components
    COMPONENT Runtime 
)
# ------------ Set up include directories ------------ #

# these are the directories where the 
# .cpp files will look for .h files
target_include_directories(MyGameEngine PRIVATE
    # for all your custom headers
    "${CMAKE_CURRENT_SOURCE_DIR}/includes" 
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/Defaults"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/ResourceClasses"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/UtilClasses"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/UIClasses"
)

# because ImGui doesn't have a CMakeList.txt file
# we have to add it to our included directories 
# STATIC links it directly into the executable
add_library(ImGuiLibrary STATIC 
    "${imgui_repo_SOURCE_DIR}/imgui.cpp"
    "${imgui_repo_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_repo_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_repo_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_repo_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_repo_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(ImGuiLibrary PUBLIC
    "${imgui_repo_SOURCE_DIR}"
    "${imgui_repo_SOURCE_DIR}/backends"
)

target_include_directories(MyGameEngine PRIVATE
    "${glm_repo_SOURCE_DIR}" # for GLM headers
    "${stb_repo_SOURCE_DIR}" # for stb_image headers
)

# ------------ Link dependencies to the executable ------------ #
target_link_libraries(ImGuiLibrary PRIVATE 
    glfw
)
target_link_libraries(MyGameEngine PRIVATE
    ImGuiLibrary
    glfw
    glad
    glm
    assimp
    OpenGL::GL
)

# ------------ Solution View setup ------------ #
set_target_properties(MyGameEngine PROPERTIES FOLDER "Engine")

set_target_properties(ImGuiLibrary PROPERTIES FOLDER "External Dependencies/ImGui")
set_target_properties(glfw PROPERTIES FOLDER "External Dependencies/GLFW3")
set_target_properties(glad PROPERTIES FOLDER "External Dependencies/GLAD")
set_target_properties(assimp PROPERTIES FOLDER "External Dependencies/ASSIMP")
set_target_properties(glm PROPERTIES FOLDER "External Dependencies/GLM")
# zlibstatic is a static version of the zlib compression library, and it comes from Assimp
# Assimp depends on zlib internally to handle compressed file formats like .dae, .gz, etc.
hide_target_if_exists(zlibstatic)
hide_target_if_exists(update_glfw)
hide_target_if_exists(update_glad)
hide_target_if_exists(update_assimp_zlib)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

target_compile_definitions(MyGameEngine PRIVATE 
    
    # STANDARD
    WINDOW_STD_WIDTH=800;
    WINDOW_STD_HEIGHT=600;
    WINDOW_STD_NAME="LearnOpenGL";
    STD_SCENE="main_scene"
    STD_SHADER="basic"

    # PATHS
    ASSETS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/";
    IMAGE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/images/";
    MESH_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/meshes/";
    JPG_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/images/jpg"; 
    PNG_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/images/png";
    OBJ_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/meshes/obj";
    SHADER_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/";  
)

