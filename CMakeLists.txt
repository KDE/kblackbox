project(kblackbox)

add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames  )


########### next target ###############

set(kblackbox_SRCS
    kbbballsgraphicwidget.cpp
    kbbballsonboard.cpp
    kbbgamedoc.cpp
    kbbgraphicsitem.cpp
    kbbgraphicsitemball.cpp
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
    kbbinfowidget.cpp
    kbbitemwithposition.cpp
    kbblevelconfigurationpreview.cpp
    kbblevelconfigurationwidget.cpp
    kbbmainwindow.cpp
    kbbscalablegraphicwidget.cpp
    kbbthememanager.cpp
    kbbtutorial.cpp
    main.cpp
)

kde4_add_kcfg_files(kblackbox_SRCS kbbprefs.kcfgc )

kde4_add_executable(kblackbox ${kblackbox_SRCS})

target_link_libraries(kblackbox  ${KDE4_KDEUI_LIBS} ${QT_QT3SUPPORT_LIBRARY} kdegames )

install(TARGETS kblackbox  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kblackbox.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kblackboxui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kblackbox )

kde4_install_icons( ${ICON_INSTALL_DIR}   )




#original Makefile.am contents follow:

#SUBDIRS = pics
#
#INCLUDES = -I$(top_srcdir)/libkdegames $(all_includes)
#
#bin_PROGRAMS = kblackbox
#kblackbox_SOURCES = kbbgfx.cpp kbbgame.cpp util.cpp main.cpp
#kblackbox_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kblackbox_LDADD = $(LIB_KDEGAMES)
#kblackbox_DEPENDENCIES = $(LIB_KDEGAMES_DEP)
#
#METASOURCES = AUTO
#KDE_ICON = kblackbox
#
#xdg_apps_DATA = kblackbox.desktop
#
#EXTRA_DIST = VERSION CHANGES $(xdg_apps_DATA)
#
#rcdir = $(kde_datadir)/kblackbox
#rc_DATA = kblackboxui.rc
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kblackbox.pot
