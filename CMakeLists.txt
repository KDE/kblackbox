project(kblackbox)

add_subdirectory(pics)

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

set(kblackbox_icons
	${CMAKE_CURRENT_SOURCE_DIR}/hi16-app-kblackbox.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi32-app-kblackbox.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi48-app-kblackbox.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi64-app-kblackbox.png
	${CMAKE_CURRENT_SOURCE_DIR}/hi128-app-kblackbox.png
)
kde4_add_win32_app_icon(kblackbox_SRCS ${kblackbox_icons})
kde4_add_executable(kblackbox ${kblackbox_SRCS})
target_link_libraries(kblackbox ${KDE4_KDEUI_LIBS} ${QT_QT3SUPPORT_LIBRARY} kdegames)


install(TARGETS kblackbox  DESTINATION ${BIN_INSTALL_DIR})
install(FILES kblackbox.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR})
install(FILES kblackboxui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblackbox)

kde4_install_icons(${ICON_INSTALL_DIR})
