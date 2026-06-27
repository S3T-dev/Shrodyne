cmake_minimum_required(VERSION 3.20)
project(shrodyne LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Vulkan SDK ---
# Tracks the user's installed driver/runtime, so this stays a system-level
# find_package rather than something we vendor via FetchContent.
find_program(GLSLC glslc REQUIRED)
find_package(Vulkan REQUIRED)

include(FetchContent)

# --- GLFW: windowing + Vulkan surface creation ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# --- volk: lightweight Vulkan function loader ---
# Platform defines below control which surface-creation entry points volk
# loads. Revisit once swapchain/surface code is actually written (v0.1) --
# GLFW 3.4 added glfwInitVulkanLoader(), which is the hook for feeding it
# volk's vkGetInstanceProcAddr instead of GLFW's own internal loader.
if(WIN32)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
elseif(APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
else()
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XCB_KHR)
endif()

FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG        vulkan-sdk-1.4.350.0
)
FetchContent_MakeAvailable(volk)

# --- GLM: header-only math (vectors/matrices/quaternions) ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# --- VMA: Vulkan Memory Allocator ---
# Not optional -- pulled in now even though v0.1 barely allocates anything,
# because retrofitting this once compute buffers multiply (nucleon state,
# wavefunction grids, decay amplitudes, density fields in v0.2+) is far
# more painful than starting with it.
FetchContent_Declare(
    vma
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG        v3.3.0
)
FetchContent_MakeAvailable(vma)

# --- Dear ImGui: debug overlay ---
# TEMPORARY. Useful right now for a live stats overlay while debugging early
# physics (frame time, particle counts, force/energy values), but this is
# explicitly expected to be removed once the real UI work in roadmap v0.6
# replaces it. Don't build anything load-bearing on top of this without
# revisiting that assumption first.
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.6
)
FetchContent_MakeAvailable(imgui)

# ImGui doesn't ship its own CMakeLists.txt -- build it as a static lib here.
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)

# --- Project sources ---
# Only collect source files from the actual project source tree, not the build
# directory. This avoids accidentally compiling external dependency test files
# that are created inside the build tree by FetchContent.
file(GLOB_RECURSE SHRODYNE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(shrodyne ${SHRODYNE_SOURCES})

target_include_directories(shrodyne PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(shrodyne PRIVATE
    glfw
    volk
    glm::glm
    GPUOpen::VulkanMemoryAllocator
    imgui
    Vulkan::Vulkan
)

target_include_directories(shrodyne PRIVATE ${vma_SOURCE_DIR}/include)

# Required when mixing volk with any code path that still includes
# vulkan/vulkan.h directly, so the static Vulkan loader's prototypes don't
# collide with volk's dynamically loaded function pointers.
target_compile_definitions(shrodyne PRIVATE VK_NO_PROTOTYPES)

# --- Shader compilation ---
# (Existing block, now wired into the real shrodyne target instead of
# assuming one already exists.)
file(GLOB_RECURSE SHADER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/shaders/*.vert
    ${CMAKE_SOURCE_DIR}/shaders/*.frag
    ${CMAKE_SOURCE_DIR}/shaders/*.comp
)

set(SHADER_BINARY_DIR ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})

set(SPIRV_BINARY_FILES)
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV ${SHADER_BINARY_DIR}/${FILE_NAME}.spv)
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSLC} ${SHADER} -o ${SPIRV}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()

add_custom_target(shaders DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(shrodyne shaders)

# Doxygen documentation target (optional)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif()
