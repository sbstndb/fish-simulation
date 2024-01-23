cmake_minimum_required(VERSION 3.10)
project(sdl-imgui)

# Recherche de la bibliothèque SDL2
find_package(SDL2 REQUIRED)

# Ajout des sources de votre programme principal
set(SOURCES
	src/test_imgui.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_sdlrenderer2.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_tables.cpp  	
)


# Ajout des sources d'ImGui
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_sdlrenderer2.cpp
)


# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    external/imgui
    external/imgui/backends
)


# Définition de l'exécutable
add_executable(sdl-imgui ${SOURCES} ${IMGUI_SOURCES})

# Inclusion des en-têtes
target_include_directories(sdl-imgui PRIVATE external/imgui external/imgui/backends ${SDL2_INCLUDE_DIRS})

# Liaison des bibliothèques
target_link_libraries(sdl-imgui PRIVATE ${SDL2_LIBRARIES} GL)

