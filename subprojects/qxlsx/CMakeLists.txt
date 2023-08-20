cmake_minimum_required(VERSION 3.16)
project(qxlsx)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(QT NAMES Qt5 REQUIRED)
if (${QT_VERSION_MAJOR} LESS 5)
  message(FATAL_ERROR "Needs Qt5")
endif ()
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/sources/*.cpp)
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/headers/*.h)

add_library(
    qxlsx STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

set(QT_PRIVATE_INCLUDE
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/qt5/QtGui/${QT_VERSION}"
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/qt5/QtGui/${QT_VERSION}/QtGui"
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/qt5/QtCore/${QT_VERSION}"
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/qt5/QtCore/${QT_VERSION}/QtCore"
)

target_include_directories(qxlsx PUBLIC headers)
target_include_directories(qxlsx PUBLIC ${QT_PRIVATE_INCLUDE})
target_link_libraries(qxlsx PUBLIC Qt5::Core Qt5::Gui)
