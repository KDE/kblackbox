kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kblackbox/pics FILES blue.xpm brown.xpm cyan.xpm gray.xpm green.xpm red.xpm white.xpm             green2.xpm giveup.xpm done.xpm )

kde4_footer()



#original Makefile.am contents follow:

#
## add here all files
#pics_DATA = blue.xpm brown.xpm cyan.xpm gray.xpm green.xpm red.xpm white.xpm\
#            green2.xpm giveup.xpm done.xpm
#
#picsdir = $(kde_datadir)/kblackbox/pics
#
## extra files for "make dist"
#EXTRA_DIST = $(pics_DATA)
#
