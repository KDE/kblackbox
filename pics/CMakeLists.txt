



########### install files ###############

install( FILES balls.svg blue.xpm brown.xpm cyan.xpm gray.xpm green.xpm red.xpm white.xpm             green2.xpm giveup.xpm done.xpm  DESTINATION  ${DATA_INSTALL_DIR}/kblackbox/pics )




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
