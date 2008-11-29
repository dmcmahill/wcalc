/* $Id: print.h,v 1.4 2002/06/12 11:30:17 dan Exp $ */

/*
 * Copyright (C) 2001, 2002 Dan McMahill
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

#ifndef __PRINT_H__
#define __PRINT_H__

/* data should be a pointer to the current wcalc */
void print_popup(gpointer data,
		 guint action,
		 GtkWidget *widget);

void global_printer_init(void);

typedef struct PRINT_CONFIG {
  /* format options */
  double fontsize;
  double leftmargin;
  double topmargin;
  double bottommargin;
  double paperheight;
  double paperwidth;
  double tab1;
  double tab2;
  double tab3;
  double tab4;

  /* directory options */
  char *eps_dir;
  char dir_sep;

} print_config;

#endif /*__PRINT_H__*/




