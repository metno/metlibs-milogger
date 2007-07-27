# includefile contains platform-definitions

include  ../conf/${OSTYPE}.mk

LIBNAME=miLogger
LOCALIDIR = $(LOCALDIR)/include/$(LIBNAME)

INCLUDE=-I../include
DEFINES=

LOCALOPTIONS=
include ../conf/targets.mk

