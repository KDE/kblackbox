project(kblackbox)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.3.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Svg)
find_package(KF5 REQUIRED COMPONENTS 
    CoreAddons 
    Config 
    Config 
    I18n 
    GuiAddons 
    IconThemes 
    XmlGui)

find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="kblackbox")


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

kde4_add_app_icon(kblackbox_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-apps-kblackbox.png")
add_executable(kblackbox ${kblackbox_SRCS})
target_link_libraries(kblackbox Qt5::Xml  KF5KDEGames KF5::XmlGui Qt5::Svg KF5::I18n KF5::Archive KF5::TextWidgets KF5::DBusAddons)


install(TARGETS kblackbox  ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kblackbox.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR})
install(FILES kblackbox.appdata.xml  DESTINATION  share/appdata)
install(FILES kblackboxui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/kblackbox)

ecm_install_icons(ICONS hi128-apps-kblackbox.png  hi16-apps-kblackbox.png  hi22-apps-kblackbox.png  hi32-apps-kblackbox.png  hi48-apps-kblackbox.png  hi64-apps-kblackbox.png DESTINATION ${ICON_INSTALL_DIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
