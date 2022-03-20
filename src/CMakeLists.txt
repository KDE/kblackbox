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
    KF5KDEGames
    KF5::CoreAddons
    KF5::XmlGui
    KF5::I18n
    KF5::Archive
    KF5::TextWidgets
    KF5::DBusAddons
    KF5::Crash
    Qt${QT_MAJOR_VERSION}::Svg
    Qt${QT_MAJOR_VERSION}::Xml
)

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(kblackbox Qt6::SvgWidgets)
endif()

install(TARGETS kblackbox  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kblackbox.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblackbox.appdata.xml  DESTINATION  ${KDE_INSTALL_METAINFODIR})
