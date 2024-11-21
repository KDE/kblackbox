ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KBLACKBOX
    VERSION_HEADER kblackbox_version.h
)

add_executable(kblackbox)

target_sources(kblackbox PRIVATE
    kbbballsgraphicwidget.cpp
    kbbballsgraphicwidget.h
    kbbballsonboard.cpp
    kbbballsonboard.h
    kbbgamedoc.cpp
    kbbgamedoc.h
    kbbgraphicsitemball.cpp
    kbbgraphicsitemball.h
    kbbgraphicsitemballrepository.cpp
    kbbgraphicsitemballrepository.h
    kbbgraphicsitemblackbox.cpp
    kbbgraphicsitemblackbox.h
    kbbgraphicsitemborder.cpp
    kbbgraphicsitemborder.h
    kbbgraphicsitem.cpp
    kbbgraphicsitemcursor.cpp
    kbbgraphicsitemcursor.h
    kbbgraphicsitem.h
    kbbgraphicsiteminteractioninfo.cpp
    kbbgraphicsiteminteractioninfo.h
    kbbgraphicsitemlaser.cpp
    kbbgraphicsitemlaser.h
    kbbgraphicsitemonbox.cpp
    kbbgraphicsitemonbox.h
    kbbgraphicsitemray.cpp
    kbbgraphicsitemray.h
    kbbgraphicsitemrayresult.cpp
    kbbgraphicsitemrayresult.h
    kbbgraphicsitemset.cpp
    kbbgraphicsitemset.h
    kbbgraphicsitemtutorialmarker.cpp
    kbbgraphicsitemtutorialmarker.h
    kbbitemwithposition.cpp
    kbbitemwithposition.h
    kbblevelconfigurationpreview.cpp
    kbblevelconfigurationpreview.h
    kbblevelconfigurationwidget.cpp
    kbblevelconfigurationwidget.h
    kbbmainwindow.cpp
    kbbmainwindow.h
    kbbscalablegraphicwidget.cpp
    kbbscalablegraphicwidget.h
    kbbthememanager.cpp
    kbbthememanager.h
    kbbtutorial.cpp
    kbbtutorial.h
    main.cpp

    kblackbox.qrc
)

kconfig_add_kcfg_files(kblackbox kbbprefs.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-kblackbox.png")
ecm_add_app_icon(kblackbox ICONS ${ICONS_SRCS})

target_link_libraries(kblackbox
    KDEGames6
    KF6::CoreAddons
    KF6::XmlGui
    KF6::I18n
    KF6::Archive
    KF6::TextWidgets
    KF6::DBusAddons
    KF6::Crash
    KF6::IconThemes
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::Xml
)

install(TARGETS kblackbox  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kblackbox.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblackbox.appdata.xml  DESTINATION  ${KDE_INSTALL_METAINFODIR})
