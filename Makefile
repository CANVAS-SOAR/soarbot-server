##############################################################################
#
#    file                 : Makefile
#    created              : Fri Mar 24 15:55:57 PDT 2017
#    copyright            : (C) 2002 DJ Seeds
#
##############################################################################
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
##############################################################################

ROBOT       = soarbot
MODULE      = ${ROBOT}.so
MODULEDIR   = drivers/${ROBOT}
SOURCES     = ${ROBOT}.cpp TCPServer.cpp

SHIPDIR     = drivers/${ROBOT}
SHIP        = ${ROBOT}.xml kc-corvette-ttop.rgb logo.rgb
SHIPSUBDIRS = 
COMPILFLAGS = --std=c++11

PKGSUBDIRS  = ${SHIPSUBDIRS}
src-robots-soarbot_PKGFILES = $(shell find * -maxdepth 0 -type f -print)
src-robots-soarbot_PKGDIR   = ${PACKAGE}-${VERSION}/$(subst ${TORCS_BASE},,$(shell pwd))

include ${MAKE_DEFAULT}
