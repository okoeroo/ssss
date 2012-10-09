#
#  Copyright (c) Members of the EGEE Collaboration. 2005.
#  See http://eu-egee.org/partners/ for details on the copyright holders.
#  For license conditions see the license file or http://eu-egee.org/license.html
#
#  Main Makefile.am file for the gLite security ssss component
#
#  Authors: Akos Frohner <Akos.Frohner@cern.ch>
#  Version info: $Id: Makefile.am,v 1.2 2011/03/11 12:30:09 jwhite Exp $
#  Release: $Name:  $

# Location of the macro files
ACLOCAL_AMFLAGS = -I project

SUBDIRS	= src interface
#SUBDIRS	= src interface doc

docdir = $(datadir)/doc/$(PACKAGE)
dist_doc_DATA = RELEASE-NOTES LICENSE

stage: install

.PHONY: stage

AUTOMAKE_OPTIONS = foreign

MAINTAINERCLEANFILES = Makefile.in
