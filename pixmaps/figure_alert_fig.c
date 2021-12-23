/* ***** Generated from pstoedit ***** */
#include <cairo.h>
#include <stdio.h>

/*
 * Original bounding box = for page # 1 is
 * LL: x: 135 y: 643 UR: x: 191 y: 690
 * The figure has been offset by (-135, 690)
 * to move LL to (0,0).  The width and height
 * can be read from the following two variables:
 */
static int figure_alert_fig_page_1_width = 56;
static int figure_alert_fig_page_1_height = 47;

static cairo_t * figure_alert_fig_page_1_render(cairo_surface_t *cs, cairo_t *cr)
{

  if (cr == NULL && cs == NULL) {
    return NULL;
  } else if(cr == NULL && cs != NULL) {
    cr = cairo_create (cs);
  } else if(cr != NULL && cs == NULL) {
  } else if(cr != NULL && cs != NULL) {
  }

  cairo_save (cr);

  /* set an initial font */
  cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

  cairo_rectangle (cr, 135,643.5, 56.25,47.25);

  /*
   * Path # 1 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0, -0.75);
  cairo_line_to (cr, 56.25, -0.75);
  cairo_line_to (cr, 56.25, 46.5);
  cairo_line_to (cr, 0, 46.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 135,643.5, 56.25,47.25);

  /*
   * Path # 2 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0, -0.75);
  cairo_line_to (cr, 56.25, -0.75);
  cairo_line_to (cr, 56.25, 46.5);
  cairo_line_to (cr, 0, 46.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 3 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 12 */
  cairo_move_to (cr, 20.25, 12.75);
  cairo_line_to (cr, 20.25, 12.75);
  cairo_curve_to (cr, 25.219, 12.75, 29.25, 16.781, 29.25, 21.75);

  cairo_line_to (cr, 29.25, 33);
  cairo_line_to (cr, 29.25, 33);
  cairo_curve_to (cr, 29.25, 37.969, 25.219, 42, 20.25, 42);

  cairo_line_to (cr, 18, 42);
  cairo_line_to (cr, 18, 42);
  cairo_curve_to (cr, 13.031, 42, 9, 37.969, 9, 33);

  cairo_line_to (cr, 9, 21.75);
  cairo_line_to (cr, 9, 21.75);
  cairo_curve_to (cr, 9, 16.781, 13.031, 12.75, 18, 12.75);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 4 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 31 */
  cairo_move_to (cr, 10.688, 16.125);
  cairo_line_to (cr, 10.688, 15.562);
  cairo_line_to (cr, 10.125, 15.562);
  cairo_line_to (cr, 10.125, 15.562);
  cairo_line_to (cr, 9.563, 15);
  cairo_line_to (cr, 9.563, 15);
  cairo_line_to (cr, 9, 14.437);
  cairo_line_to (cr, 9, 14.437);
  cairo_line_to (cr, 8.438, 14.437);
  cairo_line_to (cr, 8.438, 13.875);
  cairo_line_to (cr, 7.875, 13.312);
  cairo_line_to (cr, 7.875, 13.312);
  cairo_line_to (cr, 7.313, 12.75);
  cairo_line_to (cr, 6.75, 12.187);
  cairo_line_to (cr, 6.75, 12.187);
  cairo_line_to (cr, 6.188, 11.625);
  cairo_line_to (cr, 6.188, 11.625);
  cairo_line_to (cr, 5.625, 11.062);
  cairo_line_to (cr, 5.625, 11.062);
  cairo_line_to (cr, 5.625, 10.5);
  cairo_line_to (cr, 5.625, 10.5);
  cairo_line_to (cr, 5.063, 10.5);
  cairo_line_to (cr, 5.063, 10.5);
  cairo_line_to (cr, 4.5, 9.93701);
  cairo_line_to (cr, 4.5, 9.93701);
  cairo_line_to (cr, 3.938, 9.93701);
  cairo_line_to (cr, 3.938, 9.93701);
  cairo_line_to (cr, 3.375, 9.93701);
  cairo_line_to (cr, 3.375, 9.93701);
  cairo_line_to (cr, 2.813, 9.93701);
  cairo_line_to (cr, 2.813, 9.93701);
  cairo_line_to (cr, 2.25, 9.93701);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 5 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 23 */
  cairo_move_to (cr, 14.063, 13.312);
  cairo_line_to (cr, 14.063, 13.312);
  cairo_line_to (cr, 14.063, 12.75);
  cairo_line_to (cr, 14.063, 12.75);
  cairo_line_to (cr, 14.063, 12.187);
  cairo_line_to (cr, 14.063, 11.625);
  cairo_line_to (cr, 14.063, 11.062);
  cairo_line_to (cr, 14.063, 11.062);
  cairo_line_to (cr, 14.063, 10.5);
  cairo_line_to (cr, 13.5, 9.93701);
  cairo_line_to (cr, 13.5, 9.93701);
  cairo_line_to (cr, 13.5, 9.375);
  cairo_line_to (cr, 12.938, 8.81201);
  cairo_line_to (cr, 12.375, 8.25);
  cairo_line_to (cr, 12.375, 7.68701);
  cairo_line_to (cr, 11.813, 7.125);
  cairo_line_to (cr, 11.25, 6.56201);
  cairo_line_to (cr, 11.25, 6.56201);
  cairo_line_to (cr, 10.688, 6);
  cairo_line_to (cr, 10.125, 5.43701);
  cairo_line_to (cr, 10.125, 5.43701);
  cairo_line_to (cr, 10.125, 5.43701);
  cairo_line_to (cr, 10.125, 4.875);
  cairo_line_to (cr, 9.563, 4.875);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 6 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 26 */
  cairo_move_to (cr, 18, 12.75);
  cairo_line_to (cr, 18, 12.187);
  cairo_line_to (cr, 18, 12.187);
  cairo_line_to (cr, 18, 11.625);
  cairo_line_to (cr, 18, 11.062);
  cairo_line_to (cr, 18, 9.93701);
  cairo_line_to (cr, 18, 8.81201);
  cairo_line_to (cr, 18, 8.25);
  cairo_line_to (cr, 18, 7.68701);
  cairo_line_to (cr, 18, 7.125);
  cairo_line_to (cr, 18, 6.56201);
  cairo_line_to (cr, 18, 6);
  cairo_line_to (cr, 18, 6);
  cairo_line_to (cr, 18, 5.43701);
  cairo_line_to (cr, 18, 5.43701);
  cairo_line_to (cr, 18, 4.875);
  cairo_line_to (cr, 18, 4.875);
  cairo_line_to (cr, 18, 4.31201);
  cairo_line_to (cr, 18, 4.31201);
  cairo_line_to (cr, 18, 3.75);
  cairo_line_to (cr, 18, 3.75);
  cairo_line_to (cr, 18, 3.18701);
  cairo_line_to (cr, 18, 3.18701);
  cairo_line_to (cr, 18, 2.625);
  cairo_line_to (cr, 18, 2.625);
  cairo_line_to (cr, 18.563, 2.625);
  cairo_line_to (cr, 18, 2.625);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 7 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 17 */
  cairo_move_to (cr, 21.375, 12.75);
  cairo_line_to (cr, 21.375, 12.75);
  cairo_line_to (cr, 21.938, 12.187);
  cairo_line_to (cr, 21.938, 11.625);
  cairo_line_to (cr, 22.5, 10.5);
  cairo_line_to (cr, 23.063, 8.81201);
  cairo_line_to (cr, 23.063, 8.25);
  cairo_line_to (cr, 23.625, 7.68701);
  cairo_line_to (cr, 23.625, 7.125);
  cairo_line_to (cr, 23.625, 6.56201);
  cairo_line_to (cr, 23.625, 6);
  cairo_line_to (cr, 23.625, 6);
  cairo_line_to (cr, 23.625, 5.43701);
  cairo_line_to (cr, 23.625, 5.43701);
  cairo_line_to (cr, 24.188, 4.875);
  cairo_line_to (cr, 24.188, 4.875);
  cairo_line_to (cr, 24.75, 4.31201);
  cairo_line_to (cr, 25.313, 3.75);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 8 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 20 */
  cairo_move_to (cr, 25.875, 14.437);
  cairo_line_to (cr, 25.875, 14.437);
  cairo_line_to (cr, 25.875, 13.875);
  cairo_line_to (cr, 26.438, 13.312);
  cairo_line_to (cr, 26.438, 12.187);
  cairo_line_to (cr, 26.438, 11.625);
  cairo_line_to (cr, 27, 10.5);
  cairo_line_to (cr, 27.563, 9.93701);
  cairo_line_to (cr, 27.563, 9.375);
  cairo_line_to (cr, 28.125, 8.25);
  cairo_line_to (cr, 28.688, 7.68701);
  cairo_line_to (cr, 28.688, 7.125);
  cairo_line_to (cr, 28.688, 7.125);
  cairo_line_to (cr, 29.25, 7.125);
  cairo_line_to (cr, 29.25, 7.125);
  cairo_line_to (cr, 29.25, 7.125);
  cairo_line_to (cr, 29.25, 6.56201);
  cairo_line_to (cr, 29.813, 6.56201);
  cairo_line_to (cr, 29.813, 6.56201);
  cairo_line_to (cr, 30.375, 6.56201);
  cairo_line_to (cr, 30.375, 6.56201);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 9 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 12 */
  cairo_move_to (cr, 13.5, 22.312);
  cairo_line_to (cr, 13.5, 22.312);
  cairo_line_to (cr, 14.063, 22.312);
  cairo_line_to (cr, 14.063, 22.875);
  cairo_line_to (cr, 14.625, 22.875);
  cairo_line_to (cr, 15.188, 23.437);
  cairo_line_to (cr, 15.188, 23.437);
  cairo_line_to (cr, 15.188, 24);
  cairo_line_to (cr, 15.75, 24);
  cairo_line_to (cr, 15.75, 24.562);
  cairo_line_to (cr, 15.75, 24.562);
  cairo_line_to (cr, 16.313, 24.562);
  cairo_line_to (cr, 16.313, 24.562);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 10 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 17 */
  cairo_move_to (cr, 16.313, 21.75);
  cairo_line_to (cr, 16.313, 22.312);
  cairo_line_to (cr, 15.75, 22.312);
  cairo_line_to (cr, 15.188, 22.875);
  cairo_line_to (cr, 15.188, 22.875);
  cairo_line_to (cr, 15.188, 22.875);
  cairo_line_to (cr, 15.188, 23.437);
  cairo_line_to (cr, 14.625, 23.437);
  cairo_line_to (cr, 14.625, 23.437);
  cairo_line_to (cr, 14.625, 23.437);
  cairo_line_to (cr, 14.625, 24);
  cairo_line_to (cr, 14.625, 24);
  cairo_line_to (cr, 14.063, 24.562);
  cairo_line_to (cr, 14.063, 24.562);
  cairo_line_to (cr, 14.063, 24.562);
  cairo_line_to (cr, 14.063, 25.125);
  cairo_line_to (cr, 13.5, 25.125);
  cairo_line_to (cr, 13.5, 25.125);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 11 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 13 */
  cairo_move_to (cr, 21.375, 22.312);
  cairo_line_to (cr, 21.375, 22.312);
  cairo_line_to (cr, 21.375, 22.312);
  cairo_line_to (cr, 21.375, 22.312);
  cairo_line_to (cr, 21.938, 22.875);
  cairo_line_to (cr, 22.5, 22.875);
  cairo_line_to (cr, 22.5, 22.875);
  cairo_line_to (cr, 23.063, 23.437);
  cairo_line_to (cr, 23.063, 23.437);
  cairo_line_to (cr, 23.625, 24);
  cairo_line_to (cr, 23.625, 24);
  cairo_line_to (cr, 23.625, 24);
  cairo_line_to (cr, 24.188, 24.562);
  cairo_line_to (cr, 24.188, 24.562);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 12 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 14 */
  cairo_move_to (cr, 24.188, 22.312);
  cairo_line_to (cr, 23.625, 22.312);
  cairo_line_to (cr, 23.625, 22.312);
  cairo_line_to (cr, 23.063, 22.875);
  cairo_line_to (cr, 23.063, 22.875);
  cairo_line_to (cr, 22.5, 22.875);
  cairo_line_to (cr, 22.5, 23.437);
  cairo_line_to (cr, 22.5, 23.437);
  cairo_line_to (cr, 22.5, 23.437);
  cairo_line_to (cr, 22.5, 24);
  cairo_line_to (cr, 21.938, 24);
  cairo_line_to (cr, 21.938, 24);
  cairo_line_to (cr, 21.938, 24.562);
  cairo_line_to (cr, 21.938, 24.562);
  cairo_line_to (cr, 21.938, 25.125);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 13 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 73 */
  cairo_move_to (cr, 23.625, 34.125);
  cairo_line_to (cr, 23.063, 34.125);
  cairo_line_to (cr, 23.063, 34.125);
  cairo_line_to (cr, 22.5, 33.562);
  cairo_line_to (cr, 22.5, 33.562);
  cairo_line_to (cr, 21.938, 33.562);
  cairo_line_to (cr, 21.938, 33);
  cairo_line_to (cr, 21.375, 33);
  cairo_line_to (cr, 21.375, 33);
  cairo_line_to (cr, 20.813, 33);
  cairo_line_to (cr, 20.813, 32.437);
  cairo_line_to (cr, 20.25, 32.437);
  cairo_line_to (cr, 19.688, 32.437);
  cairo_line_to (cr, 19.688, 32.437);
  cairo_line_to (cr, 19.125, 31.875);
  cairo_line_to (cr, 19.125, 31.875);
  cairo_line_to (cr, 18.563, 31.875);
  cairo_line_to (cr, 18, 31.875);
  cairo_line_to (cr, 18, 31.312);
  cairo_line_to (cr, 17.438, 31.312);
  cairo_line_to (cr, 16.875, 31.312);
  cairo_line_to (cr, 16.875, 31.312);
  cairo_line_to (cr, 16.313, 31.312);
  cairo_line_to (cr, 15.75, 31.312);
  cairo_line_to (cr, 15.75, 31.312);
  cairo_line_to (cr, 15.188, 31.312);
  cairo_line_to (cr, 15.188, 31.312);
  cairo_line_to (cr, 14.625, 31.312);
  cairo_line_to (cr, 14.063, 31.312);
  cairo_line_to (cr, 14.063, 31.312);
  cairo_line_to (cr, 14.063, 31.312);
  cairo_line_to (cr, 13.5, 31.312);
  cairo_line_to (cr, 13.5, 31.312);
  cairo_line_to (cr, 13.5, 31.875);
  cairo_line_to (cr, 13.5, 31.875);
  cairo_line_to (cr, 13.5, 32.437);
  cairo_line_to (cr, 12.938, 32.437);
  cairo_line_to (cr, 12.938, 32.437);
  cairo_line_to (cr, 13.5, 33);
  cairo_line_to (cr, 13.5, 33);
  cairo_line_to (cr, 13.5, 33);
  cairo_line_to (cr, 14.063, 33.562);
  cairo_line_to (cr, 14.063, 33.562);
  cairo_line_to (cr, 14.625, 33.562);
  cairo_line_to (cr, 15.188, 34.125);
  cairo_line_to (cr, 15.75, 34.125);
  cairo_line_to (cr, 16.313, 34.125);
  cairo_line_to (cr, 16.875, 34.125);
  cairo_line_to (cr, 16.875, 34.125);
  cairo_line_to (cr, 17.438, 34.125);
  cairo_line_to (cr, 18, 34.125);
  cairo_line_to (cr, 18, 34.125);
  cairo_line_to (cr, 18.563, 34.125);
  cairo_line_to (cr, 18.563, 34.125);
  cairo_line_to (cr, 19.125, 34.125);
  cairo_line_to (cr, 19.125, 34.125);
  cairo_line_to (cr, 19.688, 34.687);
  cairo_line_to (cr, 19.688, 34.687);
  cairo_line_to (cr, 20.25, 34.687);
  cairo_line_to (cr, 20.25, 34.687);
  cairo_line_to (cr, 20.813, 34.687);
  cairo_line_to (cr, 21.375, 35.25);
  cairo_line_to (cr, 21.375, 35.25);
  cairo_line_to (cr, 21.938, 35.25);
  cairo_line_to (cr, 21.938, 35.25);
  cairo_line_to (cr, 22.5, 35.25);
  cairo_line_to (cr, 22.5, 35.25);
  cairo_line_to (cr, 23.063, 35.25);
  cairo_line_to (cr, 23.063, 35.25);
  cairo_line_to (cr, 23.063, 35.25);
  cairo_line_to (cr, 23.063, 34.687);
  cairo_line_to (cr, 23.063, 34.687);
  cairo_line_to (cr, 23.063, 34.125);
  cairo_line_to (cr, 23.063, 34.125);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 14 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 11 */
  cairo_move_to (cr, 18.563, 26.812);
  cairo_line_to (cr, 18.563, 26.812);
  cairo_line_to (cr, 18.563, 27.375);
  cairo_line_to (cr, 19.125, 27.375);
  cairo_line_to (cr, 19.125, 27.937);
  cairo_line_to (cr, 19.688, 27.937);
  cairo_line_to (cr, 19.688, 28.5);
  cairo_line_to (cr, 20.25, 28.5);
  cairo_line_to (cr, 20.25, 29.062);
  cairo_line_to (cr, 20.813, 29.625);
  cairo_line_to (cr, 20.813, 29.625);
  cairo_line_to (cr, 20.813, 30.187);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 15 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 50 */
  cairo_move_to (cr, 33.328, 18.375);
  cairo_line_to (cr, 35.563, 18.375);
  cairo_curve_to (cr, 37.031, 18.375, 37.922, 17.297, 37.922, 15.531);

  cairo_curve_to (cr, 37.922, 13.766, 37.031, 12.687, 35.563, 12.687);

  cairo_line_to (cr, 33.328, 12.687);
  cairo_close_path (cr);
  cairo_move_to (cr, 34.078, 17.734);
  cairo_line_to (cr, 34.078, 13.328);
  cairo_line_to (cr, 35.438, 13.328);
  cairo_curve_to (cr, 36.578, 13.328, 37.188, 14.094, 37.188, 15.531);

  cairo_curve_to (cr, 37.188, 16.984, 36.578, 17.734, 35.438, 17.734);

  cairo_close_path (cr);
  cairo_move_to (cr, 41.402, 12.594);
  cairo_curve_to (cr, 39.746, 12.594, 38.605, 13.797, 38.605, 15.578);

  cairo_curve_to (cr, 38.605, 17.359, 39.73, 18.547, 41.402, 18.547);

  cairo_curve_to (cr, 42.121, 18.547, 42.73, 18.344, 43.199, 17.953);

  cairo_curve_to (cr, 43.824, 17.437, 44.199, 16.547, 44.199, 15.625);

  cairo_curve_to (cr, 44.199, 13.797, 43.105, 12.594, 41.402, 12.594);

  cairo_close_path (cr);
  cairo_move_to (cr, 41.402, 13.234);
  cairo_curve_to (cr, 42.652, 13.234, 43.465, 14.172, 43.465, 15.609);

  cairo_curve_to (cr, 43.465, 16.984, 42.637, 17.922, 41.402, 17.922);

  cairo_curve_to (cr, 40.168, 17.922, 39.355, 16.984, 39.355, 15.578);

  cairo_curve_to (cr, 39.355, 14.172, 40.168, 13.234, 41.402, 13.234);

  cairo_close_path (cr);
  cairo_move_to (cr, 48.809, 15.781);
  cairo_line_to (cr, 48.809, 18.375);
  cairo_line_to (cr, 49.559, 18.375);
  cairo_line_to (cr, 49.559, 12.687);
  cairo_line_to (cr, 48.809, 12.687);
  cairo_line_to (cr, 48.809, 15.141);
  cairo_line_to (cr, 45.84, 15.141);
  cairo_line_to (cr, 45.84, 12.687);
  cairo_line_to (cr, 45.09, 12.687);
  cairo_line_to (cr, 45.09, 18.375);
  cairo_line_to (cr, 45.84, 18.375);
  cairo_line_to (cr, 45.84, 15.781);
  cairo_close_path (cr);
  cairo_move_to (cr, 51.773, 12.687);
  cairo_line_to (cr, 51.117, 12.687);
  cairo_line_to (cr, 51.117, 15.328);
  cairo_line_to (cr, 51.289, 17.062);
  cairo_line_to (cr, 51.602, 17.062);
  cairo_line_to (cr, 51.773, 15.328);
  cairo_close_path (cr);
  cairo_move_to (cr, 51.773, 17.562);
  cairo_line_to (cr, 51.102, 17.562);
  cairo_line_to (cr, 51.102, 18.375);
  cairo_line_to (cr, 51.773, 18.375);
  cairo_close_path (cr);
  cairo_move_to (cr, 52.305, 18.375);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 16 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.125);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 12 */
  cairo_move_to (cr, 31.5, 12.75);
  cairo_line_to (cr, 31.5, 17.25);
  cairo_curve_to (cr, 31.5, 20.25, 33, 21.75, 36, 21.75);

  cairo_line_to (cr, 38.25, 21.75);
  cairo_line_to (cr, 38.25, 26.25);
  cairo_line_to (cr, 42.75, 21.75);
  cairo_line_to (cr, 49.5, 21.75);
  cairo_curve_to (cr, 52.5, 21.75, 54, 20.25, 54, 17.25);

  cairo_line_to (cr, 54, 12.75);
  cairo_curve_to (cr, 54, 9.75, 52.5, 8.25, 49.5, 8.25);

  cairo_line_to (cr, 36, 8.25);
  cairo_curve_to (cr, 33, 8.25, 31.5, 9.75, 31.5, 12.75);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_restore (cr);

  return cr;
} /* end of figure_alert_fig_page_1_render() */

/* Total number of pages */
int figure_alert_fig_total_pages;

/* Array of the individual page render functions */
cairo_t * (*figure_alert_fig_render[1])(cairo_surface_t *, cairo_t *);

/* array of pointers to the widths and heights */
int figure_alert_fig_width[1];
int figure_alert_fig_height[1];

/* This function should be called at the beginning of the user program */
void figure_alert_fig_init(void)
{

  figure_alert_fig_total_pages = 1;

  figure_alert_fig_render[0] = figure_alert_fig_page_1_render;

  figure_alert_fig_width[0] = figure_alert_fig_page_1_width;
  figure_alert_fig_height[0] = figure_alert_fig_page_1_height;
}

float figure_alert_fig_width_max = 56;
float figure_alert_fig_height_max = 47;
