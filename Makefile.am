SUBDIRS = pics

INCLUDES = -I$(top_srcdir)/libkdegames $(all_includes)

bin_PROGRAMS = kblackbox
kblackbox_SOURCES = kbbgfx.cpp kbbgame.cpp util.cpp main.cpp
kblackbox_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kblackbox_LDADD = $(LIB_KDEGAMES)
kblackbox_la_DEPENDENCIES = $(LIB_KDEGAMES_DEP)

METASOURCES = AUTO
KDE_ICON = kblackbox

xdg_apps_DATA = kblackbox.desktop

EXTRA_DIST = VERSION CHANGES $(xdg_apps_DATA)

rcdir = $(kde_datadir)/kblackbox
rc_DATA = kblackboxui.rc

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kblackbox.pot
