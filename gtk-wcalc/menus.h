/* $Id: menus.h,v 1.3 2002/06/12 11:30:15 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#ifndef __MENUS_H__
#define __MENUS_H__

#include "wcalc.h"

void get_main_menu( Wcalc *wcalc,
		    GtkWidget  *window,
                    GtkWidget **menubar );

#endif /*__MENUS_H__*/
