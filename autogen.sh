#! /bin/sh
#
# $Id: autogen.sh,v 1.1.1.1 2003/02/20 00:22:38 danmc Exp $
#
# Run the various GNU autotools to bootstrap the build
# system.  Should only need to be done once.

# for now avoid using bash as not everyone has that installed
CONFIG_SHELL=/bin/sh
export CONFIG_SHELL

aclocal
autoheader
automake -a -c --foreign
autoconf
./configure $@
