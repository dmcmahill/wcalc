/* $Id: about.c,v 1.2 2002/01/03 03:54:46 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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

#include "about.h"

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
  gtk_window_set_title (GTK_WINDOW (window), "About WaveCalc");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
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
"WaveCalc\n" \
"\n" \
"Copyright (c) 1999, 2000, 2001 Dan McMahill\n" \
"All rights reserved.\n" \
"\n" \
"This code is derived from software written by Dan McMahill\n" \
"\n" \
"Redistribution and use in source and binary forms, with or without\n" \
"modification, are permitted provided that the following conditions\n" \
"are met:\n" \
"1. Redistributions of source code must retain the above copyright\n" \
"   notice, this list of conditions and the following disclaimer.\n" \
"2. Redistributions in binary form must reproduce the above copyright\n" \
"   notice, this list of conditions and the following disclaimer in the\n" \
"   documentation and/or other materials provided with the distribution.\n" \
"3. All advertising materials mentioning features or use of this software\n" \
"   must display the following acknowledgement:\n" \
"       This product includes software developed Dan McMahill\n" \
"4. The name of the author may not be used to endorse or promote products\n" \
"   derived from this software without specific prior written permission.\n" \
"\n" \
"THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR\n" \
"IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n" \
"OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n" \
"IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\n" \
"INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n" \
"BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n" \
"LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED\n" \
"AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n" \
"OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY\n" \
"OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF\n" \
"SUCH DAMAGE.\n"


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
  gtk_window_set_title (GTK_WINDOW (window), "WaveCalc Copyright");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
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

void permitivity_popup(void)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;
 
  /* create the "Permitivities" window */
  window = gtk_dialog_new();
  
  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), "Permitivities of Common Materials");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  

  /* add the text to the window */
  label = gtk_label_new (
			 "Silicon          (Si)   \n"
			 "Silicon Dioxide  (SiO2) \n"
			 );
  gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /* show it */
  gtk_widget_show (window);
}

void resistivity_popup(void)
{
  GtkWidget *button;
  GtkWidget *tab;
  GtkWidget *window;
  extern GtkWidget * resistivity_table(void);

  /* create the "Resistivities" window */
  window = gtk_dialog_new();
  
  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), "Resistivities of Common Materials");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  

  /* add the text to the window */
  tab = resistivity_table();
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      tab, TRUE, TRUE, 0);
  gtk_widget_show (tab);

  /* show it */
  gtk_widget_show (window);
}




