SUBDIRS = pics

INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD = $(LIB_KDEGAMES)

bin_PROGRAMS = kblackbox
kblackbox_SOURCES = kbbgfx.cpp kbbgame.cpp util.cpp main.cpp
METASOURCES = AUTO
KDE_ICON = kblackbox

appsdir = $(kde_appsdir)/Games/Board
apps_DATA = kblackbox.desktop

EXTRA_DIST = VERSION CHANGES $(apps_DATA)

rcdir = $(kde_datadir)/kblackbox
rc_DATA = kblackboxui.rc

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kblackbox.pot

install-data-local:
	rm -rf $(DESTDIR)$(kde_appsdir)/Games/kblackbox.desktop
