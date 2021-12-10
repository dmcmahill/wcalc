/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2009, 2021 Dan McMahill
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

/* #define DEBUG */

#include "config.h"

#include <gtk/gtk.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "about.h"
#include "files.h"
#include "newprint.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif


static void  window_close (GtkAction *action, gpointer data)
{
  GtkWidget *window;

  window = WC_WCALC (data)->window;

#ifdef DEBUG
  printf(_("%s():  Called with input pointer %p\n"),__FUNCTION__, (void *) data);
  printf(_("%s():  I will destroy window %p\n"), __FUNCTION__, window);
#endif

  /* blow away the window */
  gtk_widget_destroy(window);

}


static const char * ui_string =
  "<ui>"
  "  <menubar name='MainMenu'>"
  "    <menu name='File' action='FileMenu'>"
  "      <menu name='New' action='NewMenu'>"
  "        <placeholder name='ModelPlace'/>"
  "      </menu>"
  "      <menuitem action='Open'/>"
  "      <menuitem action='Save'/>"
  "      <menuitem action='SaveAs'/>"
  "      <separator/>"
  "      <menuitem action='Print'/>"
  "      <menuitem action='ExportToPDF'/>"
  "      <menuitem action='PageSetup'/>"
  "      <separator/>"
  "      <menuitem action='Close'/>"
  "      <separator/>"
  "      <menuitem  action='Exit'/>"
  "    </menu>"
  ""
  "    <menu name='Help' action='HelpMenu'>"
  "      <menuitem action='About'/>"
  "      <menuitem action='Copyright'/>"
  "      <menu name='Material Properties' action='MaterialsMenu'>"
  "        <menuitem action='Permeability'/>"
  "        <menuitem action='Permitivity'/>"
  "        <menuitem action='Resistivity'/>"
  "        <menuitem action='Units'/>"
  "        <menuitem action='WireSize'/>"
  "      </menu>"
  "    </menu>"
  "  </menubar>"
  "</ui>"
  ;

static const GtkActionEntry static_actions[]={
  { "FileMenu" , NULL, "_File"} ,
  { "NewMenu" , NULL, "_New"} ,
  { "Open" , GTK_STOCK_OPEN,
    "_Open" , "<control>O",
    "Open a file" , G_CALLBACK (wcalc_open)},
  { "Save" , GTK_STOCK_SAVE,
    "_Save" , "<control>S",
    "Save current file" , G_CALLBACK (wcalc_save)},
  { "SaveAs" , GTK_STOCK_SAVE_AS,
    "Save _As" , NULL,
    "Save to a new file" , G_CALLBACK (wcalc_save_as)},
  { "Print" , GTK_STOCK_PRINT,
    "Print" , "<control>P",
    "Save to a new file" , G_CALLBACK (newprint_popup)},
  { "ExportToPDF" , GTK_STOCK_PRINT,
    "Export to PDF" , "<control>E",
    "Export to a PDF file" , G_CALLBACK (newprint_pdf_popup)},
  { "PageSetup" , NULL,
    "Page Setup" , NULL,
    "Open the page setup dialog" , G_CALLBACK (page_setup_popup)},
  { "Close" , GTK_STOCK_CLOSE,
    "_Close" , "<control>W" ,
    "Close a file", G_CALLBACK (window_close)},
  { "Exit" , GTK_STOCK_QUIT,
    "E_xit", "<control>Q",
    "Exit the program", G_CALLBACK (gtk_main_quit)},

  { "HelpMenu" , NULL, "_Help"} ,
  { "About" , NULL,
    "About", NULL,
    "Show the about info", G_CALLBACK (about_popup)},
  { "Copyright" , NULL,
    "Copyright", NULL,
    "Show the copyright info", G_CALLBACK (copyright_popup)},
  { "MaterialsMenu" , NULL, "_Material Properties"} ,
  { "Permeability", NULL,
    "Per_meabilities", NULL,
    "Show a table of permeabilities of common materials.", G_CALLBACK (permeability_popup)},
  { "Permitivity", NULL,
    "P_ermitivities", NULL,
    "Show a table of permitivities of common materials.", G_CALLBACK (permitivity_popup)},
  { "Resistivity", NULL,
    "_Resistivities", NULL,
    "Show a table of resistivities of common materials.", G_CALLBACK (resistivity_popup)},
  { "Units", NULL,
    "_Units", NULL,
    "Show a units conversion table.", G_CALLBACK (units_popup)},
  { "WireSize", NULL,
    "_Wire Sizes", NULL,
    "Show a table of wire diameter vs AWG.", G_CALLBACK (AWG_popup)},
};

void get_main_menu( Wcalc *wcalc,
		    GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkActionGroup *action_group;
  GError *error = NULL;
  GtkUIManager *ui_manager;
  GtkAccelGroup *accel_group;
  guint merge_id;
  static GtkActionEntry *actions = NULL;
  gint nactions = sizeof (static_actions) / sizeof (static_actions[0]);
  static guint nmodels;
  int i;

  *menubar = NULL;

  if (actions == NULL) {
    nmodels = g_list_length(global_model_names);
    actions = (GtkActionEntry *)
      g_malloc((nactions + nmodels) * sizeof(GtkActionEntry));

    /* copy over the static actions */
    for (i=0; i<nactions; i++){
      actions[i] = static_actions[i];
    }

    /* add in the models */
    for (i=0; i<nmodels; i++){
      /*
       * struct GtkActionEntry {
       *   const gchar* name;
       *   const gchar* stock_id;
       *   const gchar* label;
       *   const gchar* accelerator;
       *   const gchar* tooltip;
       *   GCallback callback;
       * }
       */
      actions[nactions + i] = (GtkActionEntry)
        { g_list_nth_data(global_model_names, i) , NULL,
          g_list_nth_data(global_model_names, i) , NULL,
          "New model of the specified type", G_CALLBACK (wcalc_setup_cb)};
    }
  }

  nactions += nmodels;

  /* create actions and actions group */
  action_group = gtk_action_group_new ("Common_Actions");

  /* should this go before or after gtk_action_group_add_actions()? */
  gtk_action_group_set_translation_domain(action_group, NULL);

  gtk_action_group_add_actions (action_group,
                                actions,
                                nactions,
                                wcalc);
  //g_print("%s():  Created action group and added actions.  wcalc=%p\n", __FUNCTION__, wcalc);

  /* create UI manger */
  ui_manager = gtk_ui_manager_new();
  //g_print("%s():  Created UI manager: %p\n", __FUNCTION__, ui_manager);

  /* add the action group(s) to the UI manager */
  gtk_ui_manager_insert_action_group ( ui_manager,
                                       action_group,
                                       0);
  //g_print("%s():  Inserted action group\n", __FUNCTION__);

  /* extract accelerators from the UI manage and add to the top-level
     window */
  accel_group = gtk_ui_manager_get_accel_group (ui_manager);
  //g_print("%s():  Extracted accel_group = %p\n", __FUNCTION__, accel_group);

  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  //g_print("%s():  Added accel_group to window\n", __FUNCTION__);

  /* Load the UI */
  //merge_id = gtk_ui_manager_add_ui_from_file(ui_manager, "menus.xml", &error);
  //g_print("%s():  Loaded UI from file, merge_id = %u, error = %p\n", __FUNCTION__, merge_id, error);
  merge_id = gtk_ui_manager_add_ui_from_string(ui_manager, ui_string, -1, &error);
  //g_print("%s():  Loaded UI from string, merge_id = %u, error = %p\n", __FUNCTION__, merge_id, error);

  if( error != NULL) {
    g_critical("%s():  Failed to load UI: %s\n", __FUNCTION__, error->message);
    g_clear_error( &error );
    error = NULL;
    //exit(1);
    return;
  }  else if ( merge_id == 0) {
    g_critical("%s():  Failed to merge UI: %s\n", __FUNCTION__, error->message);
    g_clear_error( &error );
    error = NULL;
    //exit(1);
    return;
  }

  /* need to merge in the new model stuff */
  merge_id = gtk_ui_manager_new_merge_id(ui_manager);
  for (i=0; i<nmodels; i++){
    /*
     * struct GtkActionEntry {
     *   const gchar* name;
     *   const gchar* stock_id;
     *   const gchar* label;
     *   const gchar* accelerator;
     *   const gchar* tooltip;
     *   GCallback callback;
     * }
     */
    gtk_ui_manager_add_ui( ui_manager,
                           merge_id,
                           "/ui/MainMenu/File/New/ModelPlace",
                           g_list_nth_data(global_model_names, i),
                           g_list_nth_data(global_model_names, i),
                           GTK_UI_MANAGER_MENUITEM,
                           FALSE);
  }

  /* retrieve the widgets */
  *menubar = gtk_ui_manager_get_widget (ui_manager, "/MainMenu");
}
