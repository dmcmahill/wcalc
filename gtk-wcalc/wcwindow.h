/* $Id: wcwindow.h,v 1.1 2001/10/25 00:49:54 dan Exp $ */

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
