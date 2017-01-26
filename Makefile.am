## $Id: Makefile.am,v 1.29 2008/11/30 00:21:41 dan Exp $

##
## Copyright (C) 2001, 2002, 2003, 2004, 2017 Dan McMahill
## All rights reserved.
##
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
## 

SUBDIRS= m4 include intl po @LIB_WCALC@ @PIXMAPS@ @STDIO_WCALC@ \
		@GTK_WCALC@ @CGI_WCALC@ \
		@OCT_WCALC@ @SCI_WCALC@ @MEX_WCALC@ @HTDOCS@ utils doc win32

DIST_SUBDIRS= m4 include intl po libwcalc pixmaps stdio-wcalc gtk-wcalc \
		cgi-wcalc octave-wcalc sci-wcalc mex-wcalc htdocs utils doc win32

ACLOCAL_AMFLAGS=	-I m4

# XXX need to move the ps stuff
EXTRA_DIST= config.rpath \
	gettext.h \
	ltcf-c.sh \
	ps_header.c \
	ps_footer.c \
	ps_microstrip.c \
	AUTHORS \
	README.newmodel \
	README.win32 \
	TODO

