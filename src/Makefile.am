#SUBDIRS= rmalloc

bin_PROGRAMS= roclick

roclick_SOURCES= roclick.c scm.c scm.h rmalloc/rmalloc.c rmalloc/rmalloc.h
roclick_CFLAGS= `guile-config compile`
roclick_LDFLAGS= `guile-config link` -lX11 -lXtst
roclick_LDADD= ../libXRecPlay/liblibXRecPlay.a
