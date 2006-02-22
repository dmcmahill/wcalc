/*      $Id: wcalc.h,v 1.20 2006/02/22 16:14:12 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004, 2005, 2006 Dan McMahill
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
 *        This product includes software developed by Dan McMahill
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

#ifndef __WCALC_H__
#define __WCALC_H__

#include "config.h"

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "gettext.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifndef _
#define _(String) gettext (String)
#endif
#ifndef gettext_noop
#define gettext_noop(String) (String)
#endif
#ifndef N_
#define N_(String) gettext_noop (String)
#endif

#include "misc.h"
#include "print.h"

/* Global list of the names of the various models */
GList *global_model_names;

/* Global list of pointers to the _new() functions for each model */
GList *global_model_new;

/* Global list of menu paths for the models */
GList *global_model_menus;

/* Global list of default file names for the models */
GList *global_model_defaults;

/* Global printer page setup */
print_config *global_print_config;

/* Format string used by many forms and postscript output */
#define WC_FMT_G "%.4g"

/* Width of entries used in many forms */
/* with WC_GMT_G = %.4g, we might get "1.234e-11" which is 9 characters */
#define WC_ENTRYLENGTH  9
#define WC_WIDTH 70

/* Padding used in all the forms */
#define WC_XPAD 2
#define WC_YPAD 0

/* place holder text */
#define WC_OUTPUT_TEXT "     "

typedef struct WCALC
{
  /* 
   * Flag that says all initialization is done.  This is used to mask the effects
   * of certain callbacks which get triggered during initialization.
   */
  int init_done;

  /*
   * The main window
   */
  GtkWidget *window;

  /*
   * The main vbox
   */
  GtkWidget main_vbox;


  /*
   * Required public methods 
   */
  void (*init) (struct WCALC *, GtkWidget *, FILE *);
  void (*print_ps) (struct WCALC *, FILE *);
  int (*load) (struct WCALC *, FILE *);
  void (*save) (struct WCALC *, FILE *, char *);
  int (*analyze) (void);
  int (*synthesize) (void);
  int (*display) (void);

  /*
   * Required data
   */

  /* complete file name (path too) */
  char *file_name;

  /* base filename (no path) */
  char *file_basename;

  /* name of the model.  "Microstrip Analysis/Synthesis" for example */
  char *model_name;
  
  /* version of the model.  "v1.0" for example */
  char *model_version;

  /*
   * name of window (typically "Wcalc:model_name:filename") 
   * Note, this will be set to have room for one extra character which
   * is either \0 or '*'.  The '*' indicates that a file->save is
   * needed. 
   */
  char *window_title;

  /*
   * pointer to the last character in model_name so we can easily set
   * to ' ' or '*' 
   */
  char *save_needed;

  /*
   * TRUE if the values are in sync because an analysis has been
   * performed.  FALSE because an entry has been changed
   */
  int values_in_sync;

  /*
   * a list which keeps track of all units menus in the GUI.  This
   * will be used to tickle the callbacks of all the units menus as
   * part of the GUI initialization.
   */
  GList *units_menu_list;
  
  /* a text widget used for calculation status (values in/out of sync) */
  GtkWidget *text_status;

  /*  
  model_new;
  gui_init;
  gui_print;
  gui_destroy;
  */

} Wcalc;

void wcalc_setup(gpointer data,
		 guint action,
		 GtkWidget *widget);

void wcalc_setup_cb(gpointer data,
		    guint action,
		    GtkWidget *widget);

Wcalc *Wcalc_new(void);

void wcalc_save_needed(GtkWidget *widget, gpointer data );
void wcalc_set_title(Wcalc * wcalc);
void vals_changedCB(GtkWidget *widget, gpointer data );

/* report how many open wcalc windows we have */
int wcalc_num_windows(void);

#define WC_WCALC(x)      ((Wcalc *) (x))

#endif /* __WCALC_H__ */

