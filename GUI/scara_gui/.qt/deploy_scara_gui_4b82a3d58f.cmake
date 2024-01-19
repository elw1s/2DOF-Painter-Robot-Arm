include(/home/ardakilic/Desktop/CSE396/GUI/scara_gui/.qt/QtDeploySupport.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/scara_gui-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_ALL_MODULES_FOUND_VIA_FIND_PACKAGE "Core;DBus;Gui;Widgets;Network;PrintSupport;QmlIntegration;Qml;QmlModels;OpenGL;Quick;WebChannel;Positioning;WebEngineCore;QuickWidgets;WebEngineWidgets")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /home/ardakilic/Desktop/CSE396/GUI/scara_gui/scara_gui
    GENERATE_QT_CONF
)
