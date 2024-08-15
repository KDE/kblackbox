cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "24")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "0")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kblackbox VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMSetupVersion)
include(ECMDeprecationSettings)
include(FeatureSummary)

include(InternalMacros)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Svg
    SvgWidgets
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    CoreAddons
    Config
    Crash
    DocTools
    I18n
    DBusAddons
    TextWidgets
    WidgetsAddons
    XmlGui
)

find_package(KDEGames6 6.0.0 REQUIRED)

ecm_set_disabled_deprecation_versions(
    QT 6.5
    KF 6.0
    KDEGAMES 6.0
)

add_subdirectory(themes)
add_subdirectory(doc)
add_subdirectory(icons)
add_subdirectory(src)

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
