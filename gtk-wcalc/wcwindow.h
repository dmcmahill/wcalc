/* $Id: wcwindow.h,v 1.2 2002/06/12 11:30:21 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001 Dan McMahill
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

#ifndef __WC_WINDOW_H__
#define __WC_WINDOW_H__


#include <gdk/gdk.h>
#include <gtk/gtkwindow.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define WC_TYPE_WINDOW                  (wc_window_get_type ())
#define WC_WINDOW(obj)                  (GTK_CHECK_CAST ((obj), WC_TYPE_WINDOW, WcWindow))
#define WC_WINDOW_CLASS(klass)          (GTK_CHECK_CLASS_CAST ((klass), WC_TYPE_WINDOW, WcWindowClass))
#define WC_IS_WINDOW(obj)               (GTK_CHECK_TYPE ((obj), WC_TYPE_WINDOW))
#define WC_IS_WINDOW_CLASS(klass)       (GTK_CHECK_CLASS_TYPE ((klass), WC_TYPE_WINDOW))


typedef struct _WcWindow        WcWindow;
typedef struct _WcWindowClass   WcWindowClass;
typedef struct _WcWindowButton  WcWindowButton;


struct _WcWindow
{
  GtkWindow window;

  GtkWidget *vbox;
  GtkWidget *action_area;
};

struct _WcWindowClass
{
  GtkWindowClass parent_class;
};


GtkType    wc_window_get_type (void);
GtkWidget* wc_window_new      (void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __WC_WINDOW_H__ */
