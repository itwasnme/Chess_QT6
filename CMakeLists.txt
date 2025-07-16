cmake_minimum_required(VERSION 3.16)

project(Chess_QT6 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(appChess_QT6
    main.cpp
)

qt_add_qml_module(appChess_QT6
    URI Chess_QT6
    VERSION 1.0
    QML_FILES
        mainMenu.qml
        hostMultiplayer.qml
        joinMultiplayer.qml
        board.qml
        aiboard.qml
    SOURCES
        main.cpp
        board.cpp board.h
        figures.cpp figures.h
        piece.cpp piece.h
        AI.cpp AI.h
        NetworkManager.cpp NetworkManager.h
        ChessWrappers.cpp ChessWrappers.h
    RESOURCES
        assets/logo.png
        assets/blackbishop.png
        assets/blackking.png
        assets/blackknight.png
        assets/blackpawn.png
        assets/blackqueen.png
        assets/blackrook.png
        assets/whitebishop.png
        assets/whiteking.png
        assets/whiteknight.png
        assets/whitepawn.png
        assets/whitequeen.png
        assets/whiterook.png
)

set_target_properties(appChess_QT6 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appChess_QT6
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appChess_QT6
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

