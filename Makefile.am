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

docdir = $(datadir)/doc/$(PACKAGE)
dist_doc_DATA = RELEASE-NOTES LICENSE

stage: install

distsrc: dist
	mv $(distdir).tar.gz $(DISTTAR)/$(distdir)_src.tar.gz

distbin: all
	mkdir -p $(top_builddir)/dist_bin/$(prefix)
	$(MAKE) $(AM_MAKEFLAGS) install DESTDIR=`cd $(top_builddir); pwd`/dist_bin
	(cd $(top_builddir)/dist_bin/$(prefix); $(AMTAR) -zcf $(DISTTAR)/$(distdir)_bin.tar.gz .)
	rm  -rf $(top_builddir)/dist_bin

.PHONY: stage distsrc distbin

AUTOMAKE_OPTIONS = foreign

MAINTAINERCLEANFILES = Makefile.in
