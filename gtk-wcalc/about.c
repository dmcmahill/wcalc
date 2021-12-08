/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005,
 * 2006, 2007, 2012, 2020 Dan McMahill
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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "about.h"
#include "gettext.h"
#ifndef _
#define _(String) gettext (String)
#endif
#ifndef gettext_noop
#define gettext_noop(String) (String)
#endif
#ifndef N_
#define N_(String) gettext_noop (String)
#endif

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ASCII_POPUP_X 600
#define ASCII_POPUP_Y 200

#define ABOUT_TEXT \
"Wcalc\n" \
"\n" \
"Version " \
VER \
"\n"\
"A transmission line calculator.\n" \
"\n" \
"Written by,\n" \
"Dan McMahill, <mcmahill@alum.mit.edu>\n" \
"\n" \
"Wcalc is copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, \n" \
"2007, 2008, 2009, 2010, 2011, 2012, 2020 Dan McMahill."


static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);
}


void about_popup(void)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;

  /* create the "About" window */
  window = gtk_dialog_new();

  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), _("About Wcalc"));
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label (_("Ok"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);


  /* add the text to the window */
  label = gtk_label_new (ABOUT_TEXT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /* show it */
  gtk_widget_show (window);
}

#define COPYRIGHT_TEXT \
"Wcalc\n" \
"\n" \
"Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006,\n" \
"2007, 2008, 2009, 2010, 2011, 2012 Dan McMahill\n" \
"All rights reserved.\n" \
"\n" \
"This program is free software; you can redistribute it and/or modify\n" \
"it under the terms of the GNU General Public License as published by\n" \
"the Free Software Foundation; version 2 of the License.\n" \
"\n" \
"This program is distributed in the hope that it will be useful,\n" \
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
"GNU General Public License for more details.\n" \
"\n" \
"You should have received a copy of the GNU General Public License\n" \
"along with this program; if not, write to the Free Software\n" \
"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA\n"

void copyright_popup(void)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;

  /* create the "Copyright" window */
  window = gtk_dialog_new();

  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), _("Wcalc Copyright"));
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label (_("Ok"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);


  /* add the text to the window */
  label = gtk_label_new (COPYRIGHT_TEXT);
  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /* show it */
  gtk_widget_show (window);
}


void asciitab_popup(GtkWidget * (*table_fn)(void), const char *title)
{
  GtkWidget *button;
  GtkWidget *tab;
  GtkWidget *window;

  /* create the window */
  window = gtk_dialog_new();

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), title);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label (_("Ok"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);


  /* add the text to the window */
  tab = table_fn();
#if GTK_CHECK_VERSION(2,0,0)
  gtk_widget_set_size_request (GTK_WIDGET(tab), ASCII_POPUP_X, ASCII_POPUP_Y);
#else
  gtk_widget_set_usize (GTK_WIDGET(window), ASCII_POPUP_X, ASCII_POPUP_Y);
#endif
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      tab, TRUE, TRUE, 0);
  gtk_widget_show (tab);

  /* show it */
  gtk_widget_show (window);
}

void AWG_popup(void)
{
  extern GtkWidget * AWG_table(void);
  asciitab_popup(AWG_table,
		 _("Table of Wire Diameter vs. A.W.G."));
}

void permeability_popup(void)
{
  extern GtkWidget * permeability_table(void);
  asciitab_popup(permeability_table,
		 _("Permeabilities of Common Materials"));
}

void permitivity_popup(void)
{
  extern GtkWidget * permitivity_table(void);
  asciitab_popup(permitivity_table,
		 _("Permitivities of Common Materials"));
}

void resistivity_popup(void)
{
  extern GtkWidget * resistivity_table(void);
  asciitab_popup(resistivity_table,
		 _("Resistivities of Common Materials"));
}

void units_popup(void)
{
  extern GtkWidget * units_table(void);
  asciitab_popup(units_table,
		 _("Unit Conversion Factors"));
}



