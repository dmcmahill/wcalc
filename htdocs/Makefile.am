## -*- makefile -*-
##

## Copyright (C) 2001, 2002, 2004, 2005, 2006, 2009 Dan McMahill
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

# search for the .asciitab files (for the resistivity table, etc)
vpath %.asciitab ../gtk-wcalc:$(top_srcdir)/gtk-wcalc

SUBDIRS=	sci-man  mex-man octave-man stdio-man

# Note, we let the user pick some arbitrary directory for the PNG files
# because we simply don't know how the user will organize their web server.
# For example, someone might install this in their personal public_html
# directory and need to look for images in ~randomuser/images, or perhaps
# this is installed by the webadmin under /wcalc.
pkgdatadir=	${datadir}/${PACKAGE}-${VERSION}
pkgimgdatadir= @HTMLDIR@
pkgimgdata_DATA= ${PNGS} ${HTML_OUT} favicon.ico

# the include= is a ':' seperated search path for SSI directives
SHTML_INCLUDE_PATH= $(srcdir)

SHTML_IN=	\
	about.shtml \
	air_coil.shtml \
	AWG.shtml \
	bars.shtml \
	bugs.shtml \
	cgi-wcalc.shtml \
	coax.shtml \
	coplanar.shtml \
	coupled_microstrip.shtml \
	coupled_stripline.shtml \
	faq.shtml \
	gtk-wcalc.shtml \
	ic_microstrip.shtml \
	index.shtml \
	mex-wcalc.shtml \
	microstrip.shtml \
	news.shtml \
	obtaining.shtml \
	octave-wcalc.shtml \
	parallel_rc.shtml \
	parallel_rl.shtml \
	permeability.shtml \
	permitivity.shtml \
	projects.shtml \
	resistivity.shtml \
	sci-wcalc.shtml \
	stdio-wcalc.shtml \
	stripline.shtml \
	tech.shtml \
	units.shtml

INCL_FILES=\
	disclaimer_properties.incl \
	left_column.incl \
	main_footer.incl \
	page_start.incl \
	${BUILT_INCL_FILES}

BUILT_INCL_FILES=\
	AWG.incl \
	permeability.incl \
	permitivity.incl \
	resistivity.incl \
	units.incl

HTML_OUT=	${SHTML_IN:.shtml=.html}

PNGS=\
	screen1-thumb.png \
	screen1.png \
	screen2-thumb.png \
	screen2.png

favicon.ico: $(srcdir)/favicon.xpm
if MISSING_XPMTOPPM
	@echo "****************************************************"
	@echo "WARNING:  xpmtoppm and/or ppmtowinicon were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to $<"
	@echo "          will be ignored."
	@echo "****************************************************"
else
	${XPMTOPPM} $(srcdir)/favicon.xpm | ${PPMTOWINICON} > $@
endif

clean-local:	
	-rm ${HTML_OUT}

distclean-local:
	-rm AWG.incl

BUILT_SOURCES=	${BUILT_INCL_FILES} 
EXTRA_DIST=	${SHTML_IN} ${INCL_FILES} ${PNGS} favicon.ico favicon.xpm shtml.mk common-man.mk

# add the dependency on the .incl files
${HTML_OUT} : ${INCL_FILES}

mex-wcalc : mex-man/whatis.incl
octave-wcalc : octave-man/whatis.incl
sci-wcalc : sci-man/whatis.incl
stdio-wcalc : stdio-man/whatis.incl

CGIPATH=	@CGIPATH@

if SOURCEFORGE
SHTML2HTML_SF= sourceforge=1 
endif

FORMAT_TAB  = ${AWK} -f $(top_srcdir)/utils/format_tab type=html html_no_table=yes

SUFFIXES=	.asciitab

.asciitab.incl:
	${FORMAT_TAB} $< > $@

include $(srcdir)/shtml.mk


