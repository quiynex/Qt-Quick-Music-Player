cmake_minimum_required(VERSION 3.16)

project(MusicPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup()

qt_add_executable(appMusicPlayer
    main.cpp
    playercontroller.h playercontroller.cpp
    audiosearchmodel.h audiosearchmodel.cpp
)

qt_add_qml_module(appMusicPlayer
    URI MusicPlayer
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES AudioInfoBox.qml
    QML_FILES ImageButton.qml

    RESOURCES
    assets/images/quin_dog_11.jpg
    assets/images/quin_moyu.png
    assets/images/quin_kawaii.png
    assets/images/strawberry.png

    assets/icons/app_icon.ico
    assets/icons/next_icon.png
    assets/icons/pause_icon.png
    assets/icons/play_icon.png
    assets/icons/previous_icon.png
    assets/icons/add_icon.png
    assets/icons/menu_icon.png
    assets/icons/trash_icon.png
    assets/icons/close_icon.png
    assets/icons/search_icon.png

    assets/audio/air_on_the_g_string.mp3
    assets/audio/eine_kleine_nachtmusik.mp3
    assets/audio/symphony_no_5.mp3
    assets/audio/heartaches.mp3

    SOURCES audioinfo.h audioinfo.cpp
    QML_FILES PlaylistPanel.qml
    QML_FILES SearchField.qml
    QML_FILES SearchPanel.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMusicPlayer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMusicPlayer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMusicPlayer
    PRIVATE Qt6::Quick Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appMusicPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
