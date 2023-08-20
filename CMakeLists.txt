cmake_minimum_required(VERSION 3.16)
project(sen-script-decompiler)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED)
if (${QT_VERSION_MAJOR} LESS 5)
  message(FATAL_ERROR "Needs Qt5")
endif ()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  add_definitions("/bigobj")
endif()

add_executable(
    sen-script-decompiler
    sources/Builder.cpp
    sources/decompiler.cpp
    sources/functions.cpp
    sources/instruction.cpp
    sources/main.cpp
    sources/translationfile.cpp
    sources/utilities.cpp
)

set(HEADER_FILES
    headers/Builder.h
    headers/CS1InstructionsSet.h
    headers/CS2InstructionsSet.h
    headers/CS3InstructionsSet.h
    headers/CS4InstructionsSet.h
    headers/ReverieInstructionsSet.h
    headers/TXInstructionsSet.h
    headers/decompiler.h
    headers/exceptions.h
    headers/functions.h
    headers/global_vars.h
    headers/instruction.h
    headers/operande.h
    headers/translationfile.h
    headers/utilities.h
)

target_compile_definitions(sen-script-decompiler PUBLIC
    -DQT_DISABLE_DEPRECATED_BEFORE=0x060000
    -DQT_DEPRECATED_WARNINGS
)

target_include_directories(sen-script-decompiler PUBLIC .)
target_include_directories(sen-script-decompiler PUBLIC subprojects)

add_subdirectory(subprojects/qxlsx)

target_link_libraries(sen-script-decompiler PUBLIC qxlsx)
target_link_libraries(sen-script-decompiler PUBLIC Qt5::Core Qt5::Gui)
