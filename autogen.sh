#! /bin/sh
#
# $Id: autogen.sh,v 1.3 2005/09/06 04:31:10 danmc Exp $
#
# Run the various GNU autotools to bootstrap the build
# system.  Should only need to be done once.

# for now avoid using bash as not everyone has that installed
CONFIG_SHELL=/bin/sh
export CONFIG_SHELL

echo "Running aclocal..."
aclocal $ACLOCAL_FLAGS || exit 1
echo "Done with aclocal"

echo "Running autoheader..."
autoheader || exit 1
echo "Done with autoheader"

echo "Running automake..."
automake -a -c --foreign || exit 1
echo "Done with automake"

echo "Running autoconf..."
autoconf || exit 1
echo "Done with autoconf"

echo "Running ./configure $@"
./configure $@ || exit 1
echo "Done with configure"

echo "All done with autogen.sh"

