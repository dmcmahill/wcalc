# $Id: Makefile,v 1.7 2001/09/14 00:01:47 dan Exp $
#

# Copyright (c) 2001 Dan McMahill
# All rights reserved.
#
# This code is derived from software written by Dan McMahill
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#        This product includes software developed by Dan McMahill
#  4. The name of the author may not be used to endorse or promote products
#     derived from this software without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#  SUCH DAMAGE.
#

TGIF_SRCS=	\
		air_coil.obj \
		coupled_microstrip.obj \
		ic_microstrip.obj \
		ic_microstrip_model.obj \
		microstrip.obj \
		microstrip_model.obj \
		stripline.obj \
		stripline_model.obj

XPMS=	${TGIF_SRCS:.obj=.xpm}
GIFS=	${TGIF_SRCS:.obj=.gif}
PNGS=	${TGIF_SRCS:.obj=.png}

TGIF=		tgif
TGIF_FLAGS=	-print -color

all:	${GIFS} ${PNGS} ${XPMS}

clean:
	rm -f *.xpm *.xbm *.gif *.png *~

.SUFFIXES : .obj .xpm .xbm .gif .png

.obj.gif :
	$(TGIF) $(TGIF_FLAGS) -gif $*.obj

.obj.png :
	$(TGIF) $(TGIF_FLAGS) -png $*.obj

.obj.xbm :
	$(TGIF) $(TGIF_FLAGS) -xbm $*.obj

.xbm.xpm:
	convert $*.xbm xpm:- | sed "s/magick/$*/g" > $*.xpm

