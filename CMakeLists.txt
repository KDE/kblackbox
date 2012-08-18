project(kblackbox)

if(NOT COMPILING_KDEGAMES)
	find_package(KDE4 REQUIRED)
	include(KDE4Defaults)
	include(MacroLibrary)
	find_package(KDEGames REQUIRED)

	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
	include_directories(${QDBUS_INCLUDE_DIRS} ${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIRS})
endif(NOT COMPILING_KDEGAMES)

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

kde4_add_kcfg_files(kblackbox_SRCS kbbprefs.kcfgc)

kde4_add_app_icon(kblackbox_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-kblackbox.png")
kde4_add_executable(kblackbox ${kblackbox_SRCS})
target_link_libraries(kblackbox ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} kdegames)


install(TARGETS kblackbox  ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS kblackbox.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR})
install(FILES kblackboxui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblackbox)

kde4_install_icons(${ICON_INSTALL_DIR})
