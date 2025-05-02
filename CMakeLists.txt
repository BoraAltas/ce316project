cmake_minimum_required(VERSION 3.30)
project(ce316project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")
find_package(Qt6 6.9 REQUIRED COMPONENTS Quick Gui Core)
find_package(Qt6 6.9 REQUIRED COMPONENTS Quick Gui Core Sql)

qt_standard_project_setup(REQUIRES 6.9)
qt_policy(set QTP0001 NEW)
qt_policy(set QTP0004 NEW)

set(PROJECT_SOURCES
        src/main.cpp
        src/iae.cpp
        src/iae.h
        src/sourceCodeHandler.cpp
        src/sourceCodeHandler.h
        src/project.cpp
        src/project.h
        src/student.cpp
        src/student.h
        src/sqlite3.c
        src/SQLite.cpp
        src/SQLite.h
)

qt_add_qml_module(
        iaeQml
        URI iae
        VERSION 1.0
        QML_FILES
        src/qml/mainWindow.qml
        src/qml/CreateNewConfiguration.qml
)

qt_add_executable(iae
        ${PROJECT_SOURCES}
        resources.qrc
)

target_link_libraries(iae
        PRIVATE
        iaeQml
        Qt6::Quick
        Qt6::Gui
        Qt6::Core
        Qt6::Sql
)
target_include_directories(iae
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
