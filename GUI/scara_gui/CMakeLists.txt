cmake_minimum_required(VERSION 3.5)

project(scara_gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets PrintSupport REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WebEngineWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(scara_gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        DrawingArea.h
        DrawingArea.cpp
        DrawingApp.h
        DrawingApp.cpp
        ImageUploader.h
        ImageUploader.cpp
        ImagePathsConfig.h
        ImagePathsConfig.cpp
        CropFeature.h
        CropFeature.cpp
        resources.qrc
        DragDropScene.h
        DragDropScene.cpp
        RobotProjectionWidget.h
        RobotProjectionWidget.cpp
        RobotMainMenu.h
        RobotMainMenu.cpp
        ServerListenerThread.h
        ServerListenerThread.cpp
        Settings.h
        Settings.cpp
        qcustomplot.h
        qcustomplot.cpp
        ExamplesWidget.h
        ExamplesWidget.cpp
        ExamplesApp.h
        ExamplesApp.cpp
        XOXApp.cpp
        XOXWidget.cpp
        XOXWidget.h
        XOXApp.h
        SudokuWidget.h
        SudokuWidget.cpp
        NumberSelectionDialog.h
        NumberSelectionDialog.cpp
        SudokuApp.h
        SudokuApp.cpp
        StatsApp.h
        StatsApp.cpp
        ShortestPathWidget.h ShortestPath.h CSVPlotter.h
        ShortestPathWidget.cpp ShortestPath.cpp CSVPlotter.cpp
        ColorDialog.cpp
        ColorDialog.h
        Gazebo.cpp
        Gazebo.h
        GazeboApp.h
        GazeboApp.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET scara_gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(scara_gui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(scara_gui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(scara_gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network Qt6::Core Qt6::Widgets Qt6::PrintSupport Qt6::WebEngineWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.scara_gui)
endif()
set_target_properties(scara_gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS scara_gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#Generate the deployment script for the target scara_gui
qt_generate_deploy_app_script(
	TARGET scara_gui
	FILENAME_VARIABLE deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(scara_gui)
endif()

set(CPACK_PACKAGE_NAME scara_gui)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Painter and Gamer Robot")
set(CPACK_PACKAGE_VENDOR "DrawnPlayBuddy")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/scaragui")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Arda Kilic k.kilic2019@gtu.edu.tr")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libgcc-s1")
include(CPack)
