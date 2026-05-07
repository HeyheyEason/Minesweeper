cmake_minimum_required(VERSION 3.12)
project(Minesweeper VERSION 1.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 3.0.2
)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    tgui
    GIT_REPOSITORY https://github.com/texus/tgui
    GIT_TAG v1.12.0
)

set(TGUI_BUILD_GUI_BUILDER ON CACHE BOOL "" FORCE)
set(TGUI_BACKEND "SFML_GRAPHICS" CACHE STRING "" FORCE)
FetchContent_MakeAvailable(tgui)

set(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release)

if(MSVC)
    file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "rc/*.rc")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release)
    add_compile_options("/utf-8")
    add_compile_options("$<$<CONFIG:RELEASE>:/Zi>")
    add_link_options("$<$<CONFIG:RELEASE>:/DEBUG>")
    add_link_options("$<$<CONFIG:Release>:/OPT:REF>")
    add_link_options("$<$<CONFIG:Release>:/OPT:ICF>")
endif()

include_directories(include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.hpp" "include/*.h")
set(ALL_FILES ${SOURCES} ${HEADERS} ${RESOURCES})

if(SOURCES)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})
endif()

if(HEADERS)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADERS})
endif()

add_executable(${PROJECT_NAME} ${ALL_FILES})
target_link_libraries(${PROJECT_NAME} tgui SFML::Audio SFML::System SFML::Window SFML::Main)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>)
