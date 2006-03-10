kde4_header()

add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kblackbox_SRCS 
   kbbgfx.cpp 
   kbbgame.cpp 
   util.cpp 
   main.cpp )

kde4_automoc(${kblackbox_SRCS})

kde4_add_executable(kblackbox ${kblackbox_SRCS})

target_link_libraries(kblackbox  ${QT_AND_KDECORE_LIBS} )

install_targets(/bin kblackbox )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kblackbox.desktop )
install_files( ${DATA_INSTALL_DIR}/kblackbox FILES kblackboxui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



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
