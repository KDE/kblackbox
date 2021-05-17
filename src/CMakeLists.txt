ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KBLACKBOX
    VERSION_HEADER kblackbox_version.h
)

add_executable(kblackbox)

target_sources(kblackbox PRIVATE
    kbbballsonboard.cpp
    kbbgamedoc.cpp
    kbbgraphicsitem.cpp
    kbbgraphicsitemball.cpp
    kbbgraphicsitemballrepository.cpp
    kbbgraphicsitemblackbox.cpp
    kbbgraphicsitemborder.cpp
    kbbgraphicsitemcursor.cpp
    kbbgraphicsiteminteractioninfo.cpp
    kbbgraphicsitemlaser.cpp
    kbbgraphicsitemonbox.cpp
    kbbgraphicsitemray.cpp
    kbbgraphicsitemrayresult.cpp
    kbbgraphicsitemset.cpp
    kbbgraphicsitemtutorialmarker.cpp
    kbbitemwithposition.cpp
    kbblevelconfigurationpreview.cpp
    kbblevelconfigurationwidget.cpp
    kbbmainwindow.cpp
    kbbscalablegraphicwidget.cpp
    kbbthememanager.cpp
    kbbtutorial.cpp
    main.cpp

    kblackbox.qrc
)

kconfig_add_kcfg_files(kblackbox kbbprefs.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-kblackbox.png")
ecm_add_app_icon(kblackbox ICONS ${ICONS_SRCS})

target_link_libraries(kblackbox
    KF5KDEGames
    KF5::XmlGui
    KF5::I18n
    KF5::Archive
    KF5::TextWidgets
    KF5::DBusAddons
    KF5::Crash
    Qt5::Svg
    Qt5::Xml
)

install(TARGETS kblackbox  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kblackbox.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblackbox.appdata.xml  DESTINATION  ${KDE_INSTALL_METAINFODIR})
