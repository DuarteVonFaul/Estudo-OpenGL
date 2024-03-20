cmake_minimum_required(VERSION 3.28)
project(OPENGLTT VERSION 0.1.0 LANGUAGES CXX)

# Validando Plataforma
if (UNIX)
    set(PLATFORM linux CACHE STRING "Target platform" FORCE)
elseif (WIN32)
    set(PLATFORM win64 CACHE STRING "Target platform" FORCE)
else ()
    message(FATAL_ERROR "Plataforma não suportada")
endif()

#Definindo dependencias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

#caminho raiz do projeto
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)


#IMGUI CONFIGURACAO----------------------------------------------------
# Defina o diretório onde estão localizados os arquivos imgui*.cpp
set(IMGUI_SOURCE_DIR ${SOURCE_PATH}/lib/imgui)
# Adicione todos os arquivos imgui*.cpp ao seu projeto
file(GLOB IMGUI_SOURCES
    ${IMGUI_SOURCE_DIR}/*.cpp
)
#CONFIGURACAO DO PROJETO------------------------------------------------
# Adicione seus diretórios de inclusão
include_directories(
    ${SOURCE_PATH}/include
)


#Gerar Executavel
add_executable(executavel WIN32 ${SOURCE_PATH}/main.cpp  ${IMGUI_SOURCES})
target_include_directories(executavel PRIVATE ${IMGUI_SOURCE_DIR})
target_link_libraries(executavel PRIVATE OpenGL::GL glfw gdi32 glu32.lib )