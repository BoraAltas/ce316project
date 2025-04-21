cmake_minimum_required(VERSION 3.30)
project(ce316project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")

find_package(Qt6 6.9 REQUIRED COMPONENTS Quick Gui Core)

qt_standard_project_setup(REQUIRES 6.9)

set(PROJECT_SOURCES src/main.cpp)

qt_add_qml_module(MainWindowQml
        URI MainWindow
        VERSION 1.0
        QML_FILES
            src/qml/mainWindow.qml
)

qt_add_executable(MainWindow
        ${PROJECT_SOURCES}
        resources.qrc
)

target_link_libraries(MainWindow
        PRIVATE
        MainWindowQml
        Qt6::Quick
        Qt6::Gui
        Qt6::Core
)
