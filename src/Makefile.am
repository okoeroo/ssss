####
#   This file is edited to remove the gLite specific build requirements by Oscar Koeroo
####
#
#  Copyright (c) Members of the EGEE Collaboration. 2005.
#  See http://eu-egee.org/partners/ for details on the copyright holders.
#  For license conditions see the license file or http://eu-egee.org/license.html
#
# Authors: Akos Frohner <Akos.Frohner@cern.ch>
# Version info: $Id: Makefile.am,v 1.5 2006/08/16 16:07:31 taspelie Exp $
# Release: $Name:  $


CPPFLAGS = -I$(top_srcdir)/interface $(GLITE_CFLAGS)
LDFLAGS = -L/opt/local/lib

MAINTAINERCLEAN = Makefile.in

lib_LTLIBRARIES = libssss.la 

libssss_la_SOURCES = shamir.c shamir-funcs.c log4c.c

libssss_la_LIBADD = \
	-L/opt/local/lib -lm -lcrypto


bin_PROGRAMS = \
        ssss-split-key \
        ssss-join-key \
        ssss-split-passwd \
        ssss-join-passwd

noinst_PROGRAMS = \
        test-shamir \
        test-shamir-ascii \
        ssss-generate-key

ssss_split_key_SOURCES = splitKey.c
ssss_split_key_LDADD = libssss.la

ssss_split_passwd_SOURCES = splitPasswd.c
ssss_split_passwd_LDADD = libssss.la

ssss_join_key_SOURCES = joinKey.c
ssss_join_key_LDADD = libssss.la

ssss_join_passwd_SOURCES = joinPasswd.c
ssss_join_passwd_LDADD = libssss.la

ssss_generate_key_SOURCES = genKey.c
ssss_generate_key_LDADD = libssss.la

test_shamir_SOURCES = test-shamir.c
test_shamir_LDADD = libssss.la

test_shamir_ascii_SOURCES = test-shamir-ascii.c
test_shamir_ascii_LDADD = libssss.la

noinst_SCRIPTS = \
          test-shamir.sh \
          test-shamir-ascii.sh
TESTS = \
    test-shamir.sh \
    test-shamir-ascii.sh

