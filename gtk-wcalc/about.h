/* $Id: about.h,v 1.8 2004/08/05 12:12:25 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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

#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <gtk/gtk.h>

void about_popup(void);
void copyright_popup(void);

/* XXX should move this to a seperate help .c file */
void asciitab_popup(GtkWidget * (*table_fn)(void), const char *title);

void AWG_popup(void);
void permeability_popup(void);
void permitivity_popup(void);
void resistivity_popup(void);
void units_popup(void);

#endif /*__ABOUT_H__*/
