project(kblackbox)

cmake_minimum_required (VERSION 3.5 FATAL_ERROR)
set (QT_MIN_VERSION "5.7.0")
set (KF5_MIN_VERSION "5.30.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    CoreAddons
    Config
    Crash
    DocTools
    I18n
    DBusAddons
    TextWidgets
    XmlGui
)

find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMAddAppIcon)

add_definitions(
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

add_subdirectory(pics)
add_subdirectory(doc)

set(kblackbox_SRCS
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
)

kconfig_add_kcfg_files(kblackbox_SRCS kbbprefs.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kblackbox.png")
ecm_add_app_icon(kblackbox_SRCS ICONS ${ICONS_SRCS})
add_executable(kblackbox ${kblackbox_SRCS})
target_link_libraries(kblackbox
    Qt5::Xml
    KF5KDEGames
    KF5::XmlGui
    Qt5::Svg
    KF5::I18n
    KF5::Archive
    KF5::TextWidgets
    KF5::DBusAddons
    KF5::Crash
)


install(TARGETS kblackbox  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kblackbox.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kblackbox.appdata.xml  DESTINATION  ${KDE_INSTALL_METAINFODIR})
install(FILES kblackboxui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kblackbox)

ecm_install_icons(ICONS 128-apps-kblackbox.png  16-apps-kblackbox.png  22-apps-kblackbox.png  32-apps-kblackbox.png  48-apps-kblackbox.png  64-apps-kblackbox.png DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
