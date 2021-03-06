####
#   This file is edited to remove the gLite specific build requirements by Oscar Koeroo
####
#
#  Copyright (c) Members of the EGEE Collaboration. 2004.
#  See http://eu-egee.org/partners/ for details on the copyright holders.
#  For license conditions see the license file or http://eu-egee.org/license.html
#
#  Authors: Akos Frohner <Akos.Frohner@cern.ch>
#  Version info: $Id: configure.ac,v 1.7 2011/06/14 13:35:31 jwhite Exp $
#  Release: $Name:  $
# 
#
AC_PREREQ(2.57)
AC_INIT([ssss], [1.0.1])
AC_CONFIG_MACRO_DIR([project])
AM_INIT_AUTOMAKE([1.6.3 subdir-objects foreign])
AC_CONFIG_SRCDIR([src/Makefile.am])

# Notices.
AC_COPYRIGHT([Copyright (c) 2004 The EU EGEE Project
See LICENCE file for details
])
AC_REVISION([$Revision: 1.7 $])

#Environment.
WORKDIR=`pwd`
AC_SUBST(WORKDIR)

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

LT_INIT

# Turn on warnings
if test "$ac_cv_c_compiler_gnu" = yes; then
	CFLAGS="$CFLAGS -Wall -W -Wmissing-prototypes"
fi

AC_PATH_PROG([XSLTPROC], [xsltproc], [no])
if test "$XSLTPROC" != no; then
    if test -z "$XLSTPROCFLAGS"; then
        XSLTPROCFLAGS="--nonet"
    fi
    AC_CACHE_CHECK([for DocBook XML manpage stylesheets], [glite_cv_docbook_man],
    [
        cat >conftest.xml <<"EOF"
<?xml version="1.0"?>
<!-- "http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd" @<:@ -->
<?xml-stylesheet href="http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl" type="text/xsl"?>
<!DOCTYPE refentry PUBLIC "-//OASIS//DTD DocBook XML V4.1.2//EN"
"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd" @<:@
@:>@>
<refentry id="test">
<refmeta>
<refentrytitle>TEST</refentrytitle>
<manvolnum>test</manvolnum>
</refmeta>
</refentry>
EOF
        $XSLTPROC $XSLTPROCFLAGS http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl conftest.xml >/dev/null 2>/dev/null
        result=$?
        if test $result = 0; then
            glite_cv_docbook_man=yes
        else
            glite_cv_docbook_man=no
        fi
    ])
    AC_SUBST([XSLTPROCFLAGS])
fi
AM_CONDITIONAL([HAVE_DOCBOOK_MAN], [test "$glite_cv_docbook_man" = yes])

# Location of the HTML documentation
AC_ARG_WITH([htmldir],
	AC_HELP_STRING([--with-htmldir],
		[Where to put the HTML documentation @<:@${datadir}/doc/${PACKAGE}/html@:>@]),
	[htmldir=$withval],
	[htmldir='${datadir}/doc/${PACKAGE}/html'])
AC_SUBST([htmldir])

# Configuration items
AM_CONFIG_HEADER([src/autogen/config.h])
AC_CONFIG_FILES([\
	Makefile \
	src/Makefile \
    interface/Makefile
])
#AC_CONFIG_FILES([\
#	Makefile \
#	src/Makefile \
#    interface/Makefile \
#    doc/Makefile \
#    doc/apidoc/Makefile \
#    doc/man/Makefile
#])

AC_OUTPUT
