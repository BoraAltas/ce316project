cmake_minimum_required(VERSION 3.30)
project(ce316project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")

find_package(Qt6 6.9 REQUIRED COMPONENTS Quick Gui Core QuickDialogs2)
find_package(Qt6 6.9 REQUIRED COMPONENTS Quick Gui Core QuickDialogs2 Widgets)

qt_standard_project_setup(REQUIRES 6.9)
qt_policy(set QTP0001 NEW)
qt_policy(set QTP0004 NEW)

add_subdirectory(src/zlib-1.3.1)

set(PROJECT_SOURCES
        src/main.cpp
        src/iae.cpp
        src/iae.h
        src/project.cpp
        src/project.h
        src/student.cpp
        src/student.h
        src/zipHandler.cpp
        src/zipHandler.h
)

set(MINIZIP_SOURCES
        src/zlib-1.3.1/contrib/minizip/unzip.c
        src/zlib-1.3.1/contrib/minizip/ioapi.c
)

qt_add_qml_module(
        iaeQml
        URI iae
        VERSION 1.0
        QML_FILES
        src/qml/mainWindow.qml
        src/qml/CreateNewConfiguration.qml
        src/qml/NewProject.qml
        src/qml/EditExistingConfig.qml
        src/qml/RemoveConfig.qml
        src/qml/ImportConfig.qml
        src/qml/ExportConfig.qml

)

qt_add_executable(iae
        ${PROJECT_SOURCES}
        ${MINIZIP_SOURCES}
        resources.qrc
)

target_include_directories(iae
        PRIVATE
        src/zlib-1.3.1/contrib/minizip
)

target_link_libraries(iae
        PRIVATE
        iaeQml
        Qt6::Quick
        Qt6::Gui
        Qt6::Core
        Qt6::QuickDialogs2
        Qt6::Widgets
        zlibstatic
)
