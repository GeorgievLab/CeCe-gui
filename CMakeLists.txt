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

cmake_minimum_required(VERSION 3.2)

# ######################################################################### #

# Project name
project(CeCe-gui VERSION 0.6.0 LANGUAGES CXX)

# C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_POSITION_INDEPENDENT_CODE On)

include_directories(
    CeCe
    ${CMAKE_CURRENT_BINARY_DIR}/CeCe
)

# ######################################################################### #

add_subdirectory(CeCe)
add_subdirectory(plugins)

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

if (DEV_PLUGINS)
    # Directory is in build directory
    add_definitions(
        -DDIR_PLUGINS="${CMAKE_BINARY_DIR}/${DIR_PLUGINS}"
    )
endif ()

# Configure version file
configure_file(version.hpp.in version.hpp @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# ######################################################################### #

# Sources
set(SRCS
    main.cpp
    MainWindow.hpp
    MainWindow.cpp
    MainWindow.ui
    AboutDialog.hpp
    AboutDialog.cpp
    AboutDialog.ui
    PlotCreateDialog.hpp
    PlotCreateDialog.cpp
    PlotCreateDialog.ui
    Simulator.hpp
    Simulator.cpp
    VisualizationWidget.hpp
    VisualizationWidget.cpp
    LogStream.hpp
    LogStream.cpp
    XmlHighlighter.hpp
    XmlHighlighter.cpp
    icons.qrc
    PlotWidget.hpp
    PlotWidget.cpp
    PlotCustomizationDialog.hpp
    PlotCustomizationDialog.cpp

    DataExportPlot.hpp
    DataExportPlot.cpp
    DataExportPlotFactory.hpp
    DataExportPlotFactory.cpp

    qcustomplot.h
    qcustomplot.cpp
)

if (APPLE)
    set(SRCS ${SRCS} icons/${APP_NAME}.icns)
    set_source_files_properties(icons/${APP_NAME}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    configure_file(version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}-gui")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015 Georgiev Lab")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "cz.zcu.ccy.${APP_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "${APP_NAME}.icns")

# ######################################################################### #

# Create program
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SRCS}
)

# ######################################################################### #

# Hide symbols
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist.in
    )
endif ()

# ######################################################################### #

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME}
    CeCe
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
