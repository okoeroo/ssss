#
#  Copyright (c) Members of the EGEE Collaboration. 2004.
#  See http://eu-egee.org/partners/ for details on the copyright holders.
#  For license conditions see the license file or http://eu-egee.org/license.html
#
#  Common doc/html/Makefile.am file for the gLite SSSS component
#
#  Authors: Akos Frohner <Akos.Frohner@cern.ch>
#  Version info: $Id: Makefile.am,v 1.1 2006/08/07 14:58:37 szamsu Exp $
#  Release: $Name:  $
#
#
apidocdir = $(datadir)/doc/$(PACKAGE)/apidoc
dist_apidoc_DATA = index.html \
	$(wildcard *.html) \
	$(wildcard *.gif) \
	$(wildcard *.png) \
	$(wildcard *.css)

Doxyfile: $(srcdir)/Doxyfile.template
	sed -e "s#\@top_srcdir\@#$(top_srcdir)#; s#\@srcdir\@#$(srcdir)#" $(srcdir)/Doxyfile.template >$@

index.html: Doxyfile
	doxygen Doxyfile

MAINTAINERCLEANFILES = Makefile.in
