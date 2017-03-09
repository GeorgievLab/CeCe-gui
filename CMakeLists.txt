# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

cmake_minimum_required(VERSION 3.1)

# ######################################################################### #

# Project name
project(cece-gui
    VERSION 0.6.0
    LANGUAGES CXX
)

# ######################################################################### #

if (NOT TARGET cece)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CeCe-core/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init CeCe-core
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

    add_subdirectory(CeCe-core)

    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        set_property(DIRECTORY CeCe-core PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(DIRECTORY CeCe-core PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif ()

if (NOT TARGET cece-plugins)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CeCe-plugins/CMakeLists.txt")
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init CeCe-plugins
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()

    add_subdirectory(CeCe-plugins)

    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        set_property(DIRECTORY CeCe-plugins PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(DIRECTORY CeCe-plugins PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

# ######################################################################### #

set(CMAKE_INCLUDE_CURRENT_DIR On)
set(CMAKE_AUTOMOC On)
set(CMAKE_AUTOUIC On)
set(CMAKE_AUTORCC On)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5PrintSupport)

# ######################################################################### #

# Configure version file
configure_file(src/version.hpp.in version.hpp @ONLY)

# ######################################################################### #

# Sources
set(SRCS
    src/main.cpp
    src/MainWindow.hpp
    src/MainWindow.cpp
    src/AboutDialog.hpp
    src/AboutDialog.cpp
    src/PlotCreateDialog.hpp
    src/PlotCreateDialog.cpp
    src/Simulator.hpp
    src/Simulator.cpp
    src/VisualizationWidget.hpp
    src/VisualizationWidget.cpp
    src/LogStream.hpp
    src/LogStream.cpp
    src/XmlHighlighter.hpp
    src/XmlHighlighter.cpp
    src/PlotWidget.hpp
    src/PlotWidget.cpp
    src/PlotCustomizationDialog.hpp
    src/PlotCustomizationDialog.cpp
    src/DataExportPlot.hpp
    src/DataExportPlot.cpp
    src/DataExportPlotFactory.hpp
    src/DataExportPlotFactory.cpp
    src/StdOutStream.hpp
    src/StdOutStream.cpp
    src/qcustomplot.h
    src/qcustomplot.cpp

    src/MainWindow.ui
    src/AboutDialog.ui
    src/PlotCreateDialog.ui

    resources/icons.qrc
)

if (APPLE)
    set(SRCS ${SRCS} resources/icons.icns)
    set_source_files_properties(resources/icons.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    configure_file(resources/version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

set(MACOSX_BUNDLE_BUNDLE_NAME "CeCe-gui")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2016 Georgiev Lab")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "cz.zcu.ccy.cece")
set(MACOSX_BUNDLE_ICON_FILE "icons.icns")

# ######################################################################### #

# Create program
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SRCS}
)

# ######################################################################### #

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_BINARY_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS Off
    CXX_STANDARD_REQUIRED On
)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in
        MACOSX_BUNDLE_BUNDLE_NAME "CeCe"
    )
endif ()

if (CECE_PLUGINS_DEV)
    get_target_property(DIR cece-plugins OUTPUT_DIRECTORY)

    # Directory is in build directory
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -DDIR_PLUGINS="${DIR}"
    )
endif ()

# ######################################################################### #

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME}
    cece
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::PrintSupport
)

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

# ######################################################################### #

if (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "\$ORIGIN/../lib")
elseif (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib")
endif ()

# ######################################################################### #

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    BUNDLE DESTINATION .
)

install(FILES LICENSE.txt Copyright.txt README.md
    DESTINATION .
)

if (WIN32)
    find_file(QT5_CORE_DLL Qt5Core.dll HINTS ${CMAKE_PREFIX_PATH}\\bin)
    find_file(QT5_GUI_DLL Qt5Gui.dll HINTS ${CMAKE_PREFIX_PATH}\\bin)
    find_file(QT5_WIDGETS_DLL Qt5Widgets.dll HINTS ${CMAKE_PREFIX_PATH}\\bin)
    find_file(QT5_OPENGL_DLL Qt5OpenGL.dll HINTS ${CMAKE_PREFIX_PATH}\\bin)
    find_file(QT5_PRINTSUPPORT_DLL Qt5PrintSupport.dll HINTS ${CMAKE_PREFIX_PATH}\\bin)
    find_file(QT5_PLATFORM_DLL qwindows.dll HINTS ${CMAKE_PREFIX_PATH}\\plugins\\platforms)

    if (QT5_CORE_DLL AND QT5_GUI_DLL AND QT5_WIDGETS_DLL AND QT5_OPENGL_DLL AND QT5_PRINTSUPPORT_DLL AND QT5_PLATFORM_DLL)
        install(FILES
            ${QT5_CORE_DLL}
            ${QT5_GUI_DLL}
            ${QT5_WIDGETS_DLL}
            ${QT5_OPENGL_DLL}
            ${QT5_PRINTSUPPORT_DLL}
            DESTINATION bin
        )

        install(FILES
            ${QT5_PLATFORM_DLL}
            DESTINATION bin/platforms
        )

    else ()
        message(WARNING "Unable to find Qt5 DLL libraries")
    endif ()
endif ()

# ######################################################################### #

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CeCe GUI application")
set(CPACK_PACKAGE_VENDOR "Georgiev Lab")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_NAME CeCe-gui)
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Jiří Fatka <fatkaj@ntis.zcu.cz>")

find_package(Git)
if (GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --exact-match --tags
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

message("TAG: ${GIT_TAG}")
message("COMMIT: ${GIT_COMMIT}")
message("BRANCH: ${GIT_BRANCH}")

if (NOT GIT_TAG AND GIT_COMMIT AND GIT_BRANCH)
    set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
    if (${SYSTEM_NAME} MATCHES "Windows")
        if (CMAKE_CL_64)
            set(SYSTEM_NAME "Win64")
        else ()
            set(SYSTEM_NAME "Win32")
        endif ()
    endif ()

    string(SUBSTRING "${GIT_COMMIT}" 0 7 COMMIT)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${SYSTEM_NAME}-${GIT_BRANCH}-${COMMIT}")
endif ()

# Configure file for travis-ci -> bintray deployment
if (UNIX AND NOT APPLE)
    set(BINTRAY_PLATFORM "linux-x64")
elseif (APPLE)
    set(BINTRAY_PLATFORM "macOS")
endif ()

if (NOT GIT_TAG AND GIT_COMMIT AND GIT_BRANCH)
    set(BINTRAY_VERSION ${PROJECT_VERSION}-${COMMIT})
else ()
    set(BINTRAY_VERSION ${PROJECT_VERSION})
endif ()

configure_file(resources/descriptor.json.in descriptor.json @ONLY)

# ######################################################################### #

set(CPACK_STRIP_FILES "${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}")

# ######################################################################### #

include(CPack)

# ######################################################################### #
