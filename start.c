/* $Id: start.c,v 1.1 2001/09/15 14:43:56 dan Exp $ */

/*
 * Copyright (c) 2001 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "pixmaps/splash.xpm"
#include "start.h"

#define ABOUT_TEXT \
"WaveCalc\n" \
"\n" \
"Version " \
VER \
"\n"\
"A transmission line calculator.\n" \
"\n" \
"Written by,\n" \
"Dan McMahill, <mcmahill@alum.mit.edu>\n" \
"\n" \
"WaveCalc is copyright (C) 1999, 2000, 2001\n" \
"Dan McMahill."


extern void wcalc_setup(void);

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

  wcalc_setup ();

}

 
void start_popup(void)
{
  GtkWidget *window;
  GtkWidget *main_vbox;

  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *combo_model;

  GtkWidget *separator;
  GtkWidget *action_area;

  /* stuff for the picture */
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    
 
  GList *glist=NULL;

  /* create the initial window */
  //  window = gtk_window_new(GTK_WINDOW_DIALOG);
  window = gtk_window_new(GTK_WINDOW_DIALOG);
  
  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW(window), "WaveCalc");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  gtk_widget_realize(window);


  main_vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show(main_vbox);


  /*
   * The Action Area
   */

  action_area = gtk_hbox_new (TRUE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (action_area), 10);
  gtk_box_pack_end (GTK_BOX (main_vbox), action_area, FALSE, TRUE, 0);
  gtk_widget_show(action_area);

  separator = gtk_hseparator_new ();
  gtk_box_pack_end (GTK_BOX (main_vbox), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_window_set_focus(GTK_WINDOW(window),button);
  gtk_widget_show (button);


  glist = g_list_append(glist,"Air Core Inductor");
  glist = g_list_append(glist,"Microstrip");

  combo_model =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo_model), glist);
  gtk_combo_set_use_arrows( GTK_COMBO(combo_model), 1);
  gtk_box_pack_start (GTK_BOX (action_area), combo_model, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo_model)->entry), FALSE);
  //  gtk_widget_set_usize(GTK_WIDGET(combo_model),60,0);
  gtk_widget_show( combo_model );

  /*
   * The info Area
   */

  //  my_vbox = gtk_vbox_new (FALSE, 1);
  //  gtk_box_pack_start (GTK_BOX (main_vbox), my_vbox, FALSE, TRUE, 0);

  /* now for the pixmap from gdk */

  style = gtk_widget_get_style( window );
  
  pixmap = gdk_pixmap_create_from_xpm_d( window->window, 
					 &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) splash);
    					
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (main_vbox),
		      pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    


  /* add the text to the window */
  label = gtk_label_new (ABOUT_TEXT);
  gtk_box_pack_start (GTK_BOX (main_vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /* show it */
   gtk_widget_show (window);
}






