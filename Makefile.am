# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# just set the variable
APPSDIR = $(kde_appsdir)/Games
# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = doc pics

####### This part is very kblackbox specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	kblackbox

# Which sources should be compiled for kblackbox.
kblackbox_SOURCES = kbbgfx.cpp kbbgame.cpp util.cpp main.cpp

kblackbox_METASOURCES = USE_AUTOMOC

# the library search path. 
kblackbox_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
kblackbox_LDADD   = -lkdeui -lkdecore -lqt -lXext -lX11

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = kbbgfx.h kbbgame.h util.h version.h

# extra files for "make dist"
EXTRA_DIST += VERSION CHANGES kblackbox.kdelnk kblackbox.xpm mini-kblackbox.xpm

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
CLEANFILES = $(kblackbox_METASOURCES)


# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
#messages:
#	$(XGETTEXT) -C -ktranslate -x ../../../kdelibs/po/kde.pot $(kblackbox_SOURCES) && mv messages.po ../po/kblackbox.pot

# make messages.po. Move this one to ../po/ and "make merge" in po
messages:
	$(XGETTEXT) -C -ktranslate $(kblackbox_SOURCES) && mv messages.po ../po/kblackbox.pot

kde_icon_DATA = kblackbox.xpm

install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) $(srcdir)/kblackbox.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) $(srcdir)/mini-kblackbox.xpm $(kde_minidir)/kblackbox.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/kblackbox.kdelnk
	-rm -f $(kde_minidir)/kblackbox.xpm
