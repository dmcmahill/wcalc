/* ***** Generated from pstoedit ***** */
#include <cairo.h>
#include <stdio.h>

/*
 * Original bounding box = for page # 1 is
 * LL: x: 59 y: 614 UR: x: 369 y: 708
 * The figure has been offset by (-59, 708)
 * to move LL to (0,0).  The width and height
 * can be read from the following two variables:
 */
static int figure_rc_page_1_width = 310;
static int figure_rc_page_1_height = 94;

static cairo_t * figure_rc_page_1_render(cairo_surface_t *cs, cairo_t *cr)
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

  cairo_rectangle (cr, 159.75,614.25, 112.5,31.5);

  /*
   * Path # 1 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 100.75, 62.25);
  cairo_line_to (cr, 213.25, 62.25);
  cairo_line_to (cr, 213.25, 93.75);
  cairo_line_to (cr, 100.75, 93.75);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 159.75,614.25, 112.5,31.5);

  /*
   * Path # 2 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 100.75, 62.25);
  cairo_line_to (cr, 213.25, 62.25);
  cairo_line_to (cr, 213.25, 93.75);
  cairo_line_to (cr, 100.75, 93.75);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 157.5,616.5, 112.5,31.5);

  /*
   * Path # 3 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 98.5, 60);
  cairo_line_to (cr, 211, 60);
  cairo_line_to (cr, 211, 91.5);
  cairo_line_to (cr, 98.5, 91.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 157.5,616.5, 112.5,31.5);

  /*
   * Path # 4 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 98.5, 60);
  cairo_line_to (cr, 211, 60);
  cairo_line_to (cr, 211, 91.5);
  cairo_line_to (cr, 98.5, 91.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 5 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 33);
  cairo_line_to (cr, 76, 42);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 6 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 44.25);
  cairo_line_to (cr, 76, 51);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 7 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 71.5, 42);
  cairo_line_to (cr, 80.5, 42);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 8 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 71.5, 44.25);
  cairo_line_to (cr, 80.5, 44.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 9 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 94, 28.5);
  cairo_line_to (cr, 94, 35.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 10 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 94, 48.75);
  cairo_line_to (cr, 94, 55.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 11 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 8 */
  cairo_move_to (cr, 94, 35.25);
  cairo_line_to (cr, 89.5, 36.375);
  cairo_line_to (cr, 98.5, 38.625);
  cairo_line_to (cr, 89.5, 40.875);
  cairo_line_to (cr, 98.5, 43.125);
  cairo_line_to (cr, 89.5, 45.375);
  cairo_line_to (cr, 98.5, 47.625);
  cairo_line_to (cr, 94, 48.75);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 12 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 7 */
  cairo_move_to (cr, 94, 35.25);
  cairo_line_to (cr, 89.5, 36.375);
  cairo_line_to (cr, 98.5, 38.625);
  cairo_line_to (cr, 89.5, 40.875);
  cairo_line_to (cr, 98.5, 43.125);
  cairo_line_to (cr, 89.5, 45.375);
  cairo_line_to (cr, 98.5, 47.625);
  cairo_line_to (cr, 94, 48.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 13 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 33);
  cairo_line_to (cr, 76, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 14 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 51);
  cairo_line_to (cr, 76, 64.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 15 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 94, 55.5);
  cairo_line_to (cr, 94, 64.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 16 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 94, 28.5);
  cairo_line_to (cr, 94, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 17 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 64.5);
  cairo_curve_to (cr, 86.688, 65.43, 85.93, 66.187, 85, 66.187);

  cairo_curve_to (cr, 84.07, 66.187, 83.313, 65.43, 83.313, 64.5);

  cairo_curve_to (cr, 83.313, 63.57, 84.07, 62.812, 85, 62.812);

  cairo_curve_to (cr, 85.93, 62.812, 86.688, 63.57, 86.688, 64.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 18 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 64.5);
  cairo_curve_to (cr, 86.688, 65.43, 85.93, 66.187, 85, 66.187);

  cairo_curve_to (cr, 84.07, 66.187, 83.313, 65.43, 83.313, 64.5);

  cairo_curve_to (cr, 83.313, 63.57, 84.07, 62.812, 85, 62.812);

  cairo_curve_to (cr, 85.93, 62.812, 86.688, 63.57, 86.688, 64.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 19 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 19.5);
  cairo_curve_to (cr, 86.688, 20.43, 85.93, 21.187, 85, 21.187);

  cairo_curve_to (cr, 84.07, 21.187, 83.313, 20.43, 83.313, 19.5);

  cairo_curve_to (cr, 83.313, 18.57, 84.07, 17.812, 85, 17.812);

  cairo_curve_to (cr, 85.93, 17.812, 86.688, 18.57, 86.688, 19.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 20 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 19.5);
  cairo_curve_to (cr, 86.688, 20.43, 85.93, 21.187, 85, 21.187);

  cairo_curve_to (cr, 84.07, 21.187, 83.313, 20.43, 83.313, 19.5);

  cairo_curve_to (cr, 83.313, 18.57, 84.07, 17.812, 85, 17.812);

  cairo_curve_to (cr, 85.93, 17.812, 86.688, 18.57, 86.688, 19.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 21 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 64.5);
  cairo_line_to (cr, 94, 64.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 22 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 85, 64.5);
  cairo_line_to (cr, 85, 82.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 23 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 5 */
  cairo_move_to (cr, 86.688, 82.5);
  cairo_curve_to (cr, 86.688, 83.43, 85.93, 84.187, 85, 84.187);

  cairo_curve_to (cr, 84.07, 84.187, 83.313, 83.43, 83.313, 82.5);

  cairo_curve_to (cr, 83.313, 81.57, 84.07, 80.812, 85, 80.812);

  cairo_curve_to (cr, 85.93, 80.812, 86.688, 81.57, 86.688, 82.5);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 24 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 82.5);
  cairo_curve_to (cr, 86.688, 83.43, 85.93, 84.187, 85, 84.187);

  cairo_curve_to (cr, 84.07, 84.187, 83.313, 83.43, 83.313, 82.5);

  cairo_curve_to (cr, 83.313, 81.57, 84.07, 80.812, 85, 80.812);

  cairo_curve_to (cr, 85.93, 80.812, 86.688, 81.57, 86.688, 82.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 25 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 76, 19.5);
  cairo_line_to (cr, 94, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 26 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 85, 1.5);
  cairo_line_to (cr, 85, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 27 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 5 */
  cairo_move_to (cr, 86.688, 1.5);
  cairo_curve_to (cr, 86.688, 2.42999, 85.93, 3.18701, 85, 3.18701);

  cairo_curve_to (cr, 84.07, 3.18701, 83.313, 2.42999, 83.313, 1.5);

  cairo_curve_to (cr, 83.313, 0.570007, 84.07, -0.187988, 85, -0.187988);

  cairo_curve_to (cr, 85.93, -0.187988, 86.688, 0.570007, 86.688, 1.5);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 28 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86.688, 1.5);
  cairo_curve_to (cr, 86.688, 2.42999, 85.93, 3.18701, 85, 3.18701);

  cairo_curve_to (cr, 84.07, 3.18701, 83.313, 2.42999, 83.313, 1.5);

  cairo_curve_to (cr, 83.313, 0.570007, 84.07, -0.187988, 85, -0.187988);

  cairo_curve_to (cr, 85.93, -0.187988, 86.688, 0.570007, 86.688, 1.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 29 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 19.5);
  cairo_line_to (cr, 229, 28.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 30 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 30.75);
  cairo_line_to (cr, 229, 37.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 31 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 224.5, 28.5);
  cairo_line_to (cr, 233.5, 28.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 32 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 224.5, 30.75);
  cairo_line_to (cr, 233.5, 30.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 33 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 37.5);
  cairo_line_to (cr, 229, 44.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 34 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 57.75);
  cairo_line_to (cr, 229, 64.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 35 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 8 */
  cairo_move_to (cr, 229, 44.25);
  cairo_line_to (cr, 224.5, 45.375);
  cairo_line_to (cr, 233.5, 47.625);
  cairo_line_to (cr, 224.5, 49.875);
  cairo_line_to (cr, 233.5, 52.125);
  cairo_line_to (cr, 224.5, 54.375);
  cairo_line_to (cr, 233.5, 56.625);
  cairo_line_to (cr, 229, 57.75);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 36 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 7 */
  cairo_move_to (cr, 229, 44.25);
  cairo_line_to (cr, 224.5, 45.375);
  cairo_line_to (cr, 233.5, 47.625);
  cairo_line_to (cr, 224.5, 49.875);
  cairo_line_to (cr, 233.5, 52.125);
  cairo_line_to (cr, 224.5, 54.375);
  cairo_line_to (cr, 233.5, 56.625);
  cairo_line_to (cr, 229, 57.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 37 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 64.5);
  cairo_line_to (cr, 229, 82.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 38 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 229, 1.5);
  cairo_line_to (cr, 229, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 39 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 5 */
  cairo_move_to (cr, 230.688, 1.5);
  cairo_curve_to (cr, 230.688, 2.42999, 229.93, 3.18701, 229, 3.18701);

  cairo_curve_to (cr, 228.07, 3.18701, 227.313, 2.42999, 227.313, 1.5);

  cairo_curve_to (cr, 227.313, 0.570007, 228.07, -0.187988, 229, -0.187988);

  cairo_curve_to (cr, 229.93, -0.187988, 230.688, 0.570007, 230.688, 1.5);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 40 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 230.688, 1.5);
  cairo_curve_to (cr, 230.688, 2.42999, 229.93, 3.18701, 229, 3.18701);

  cairo_curve_to (cr, 228.07, 3.18701, 227.313, 2.42999, 227.313, 1.5);

  cairo_curve_to (cr, 227.313, 0.570007, 228.07, -0.187988, 229, -0.187988);

  cairo_curve_to (cr, 229.93, -0.187988, 230.688, 0.570007, 230.688, 1.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 41 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 5 */
  cairo_move_to (cr, 230.688, 82.5);
  cairo_curve_to (cr, 230.688, 83.43, 229.93, 84.187, 229, 84.187);

  cairo_curve_to (cr, 228.07, 84.187, 227.313, 83.43, 227.313, 82.5);

  cairo_curve_to (cr, 227.313, 81.57, 228.07, 80.812, 229, 80.812);

  cairo_curve_to (cr, 229.93, 80.812, 230.688, 81.57, 230.688, 82.5);

  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 42 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 230.688, 82.5);
  cairo_curve_to (cr, 230.688, 83.43, 229.93, 84.187, 229, 84.187);

  cairo_curve_to (cr, 228.07, 84.187, 227.313, 83.43, 227.313, 82.5);

  cairo_curve_to (cr, 227.313, 81.57, 228.07, 80.812, 229, 80.812);

  cairo_curve_to (cr, 229.93, 80.812, 230.688, 81.57, 230.688, 82.5);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 43 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 190.75, 39.75);
  cairo_line_to (cr, 213.25, 39.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 44 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 190.75, 44.25);
  cairo_line_to (cr, 213.25, 44.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 45 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 2 */
  cairo_move_to (cr, 208.75, 35.25);
  cairo_line_to (cr, 215.5, 42);
  cairo_line_to (cr, 208.75, 48.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 46 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 2 */
  cairo_move_to (cr, 195.25, 35.25);
  cairo_line_to (cr, 188.5, 42);
  cairo_line_to (cr, 195.25, 48.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 47 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 240.109, 61);
  cairo_line_to (cr, 242.813, 61);
  cairo_curve_to (cr, 243.75, 61, 244.172, 61.437, 244.172, 62.453);

  cairo_line_to (cr, 244.156, 63.172);
  cairo_curve_to (cr, 244.156, 63.672, 244.25, 64.172, 244.406, 64.5);

  cairo_line_to (cr, 245.672, 64.5);
  cairo_line_to (cr, 245.672, 64.25);
  cairo_curve_to (cr, 245.281, 63.969, 245.203, 63.687, 245.188, 62.609);

  cairo_curve_to (cr, 245.172, 61.266, 244.953, 60.859, 244.063, 60.484);

  cairo_curve_to (cr, 244.984, 60.031, 245.359, 59.484, 245.359, 58.547);

  cairo_curve_to (cr, 245.359, 57.141, 244.484, 56.359, 242.859, 56.359);

  cairo_line_to (cr, 239.047, 56.359);
  cairo_line_to (cr, 239.047, 64.5);
  cairo_line_to (cr, 240.109, 64.5);
  cairo_close_path (cr);
  cairo_move_to (cr, 240.109, 60.078);
  cairo_line_to (cr, 240.109, 57.281);
  cairo_line_to (cr, 242.641, 57.281);
  cairo_curve_to (cr, 243.234, 57.281, 243.578, 57.375, 243.828, 57.594);

  cairo_curve_to (cr, 244.125, 57.828, 244.266, 58.203, 244.266, 58.672);

  cairo_curve_to (cr, 244.266, 59.641, 243.766, 60.078, 242.641, 60.078);

  cairo_close_path (cr);
  cairo_move_to (cr, 246.121, 64.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 48 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 250.121, 63.281);
  cairo_curve_to (cr, 250.121, 62.953, 250.105, 62.859, 249.996, 62.625);

  cairo_curve_to (cr, 249.73, 62.062, 249.168, 61.766, 248.34, 61.766);

  cairo_curve_to (cr, 247.262, 61.766, 246.59, 62.312, 246.59, 63.203);

  cairo_curve_to (cr, 246.59, 63.812, 246.902, 64.187, 247.559, 64.344);

  cairo_line_to (cr, 248.777, 64.672);
  cairo_curve_to (cr, 249.402, 64.828, 249.668, 65.078, 249.668, 65.469);

  cairo_curve_to (cr, 249.668, 65.734, 249.527, 66, 249.324, 66.141);

  cairo_curve_to (cr, 249.121, 66.281, 248.824, 66.359, 248.418, 66.359);

  cairo_curve_to (cr, 247.887, 66.359, 247.527, 66.219, 247.293, 65.937);

  cairo_curve_to (cr, 247.105, 65.734, 247.027, 65.5, 247.043, 65.187);

  cairo_line_to (cr, 246.449, 65.187);
  cairo_curve_to (cr, 246.449, 65.641, 246.543, 65.937, 246.73, 66.203);

  cairo_curve_to (cr, 247.074, 66.672, 247.637, 66.906, 248.387, 66.906);

  cairo_curve_to (cr, 248.965, 66.906, 249.449, 66.766, 249.762, 66.531);

  cairo_curve_to (cr, 250.09, 66.266, 250.293, 65.828, 250.293, 65.406);

  cairo_curve_to (cr, 250.293, 64.797, 249.918, 64.359, 249.262, 64.172);

  cairo_line_to (cr, 248.027, 63.844);
  cairo_curve_to (cr, 247.434, 63.687, 247.215, 63.5, 247.215, 63.125);

  cairo_curve_to (cr, 247.215, 62.625, 247.652, 62.297, 248.309, 62.297);

  cairo_curve_to (cr, 249.09, 62.297, 249.527, 62.641, 249.543, 63.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 250.621, 66.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 49 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 262.609, 61);
  cairo_line_to (cr, 265.313, 61);
  cairo_curve_to (cr, 266.25, 61, 266.672, 61.437, 266.672, 62.453);

  cairo_line_to (cr, 266.656, 63.172);
  cairo_curve_to (cr, 266.656, 63.672, 266.75, 64.172, 266.906, 64.5);

  cairo_line_to (cr, 268.172, 64.5);
  cairo_line_to (cr, 268.172, 64.25);
  cairo_curve_to (cr, 267.781, 63.969, 267.703, 63.687, 267.688, 62.609);

  cairo_curve_to (cr, 267.672, 61.266, 267.453, 60.859, 266.563, 60.484);

  cairo_curve_to (cr, 267.484, 60.031, 267.859, 59.484, 267.859, 58.547);

  cairo_curve_to (cr, 267.859, 57.141, 266.984, 56.359, 265.359, 56.359);

  cairo_line_to (cr, 261.547, 56.359);
  cairo_line_to (cr, 261.547, 64.5);
  cairo_line_to (cr, 262.609, 64.5);
  cairo_close_path (cr);
  cairo_move_to (cr, 262.609, 60.078);
  cairo_line_to (cr, 262.609, 57.281);
  cairo_line_to (cr, 265.141, 57.281);
  cairo_curve_to (cr, 265.734, 57.281, 266.078, 57.375, 266.328, 57.594);

  cairo_curve_to (cr, 266.625, 57.828, 266.766, 58.203, 266.766, 58.672);

  cairo_curve_to (cr, 266.766, 59.641, 266.266, 60.078, 265.141, 60.078);

  cairo_close_path (cr);
  cairo_move_to (cr, 268.621, 64.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 50 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 269.855, 64.672);
  cairo_line_to (cr, 271.402, 64.672);
  cairo_curve_to (cr, 271.777, 64.672, 272.09, 64.562, 272.34, 64.328);

  cairo_curve_to (cr, 272.637, 64.047, 272.777, 63.734, 272.777, 63.281);

  cairo_curve_to (cr, 272.777, 62.359, 272.23, 61.844, 271.262, 61.844);

  cairo_line_to (cr, 269.23, 61.844);
  cairo_line_to (cr, 269.23, 66.75);
  cairo_line_to (cr, 269.855, 66.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 269.855, 64.125);
  cairo_line_to (cr, 269.855, 62.406);
  cairo_line_to (cr, 271.168, 62.406);
  cairo_curve_to (cr, 271.762, 62.406, 272.121, 62.719, 272.121, 63.266);

  cairo_curve_to (cr, 272.121, 63.797, 271.762, 64.125, 271.168, 64.125);

  cairo_close_path (cr);
  cairo_move_to (cr, 273.121, 66.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 51 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 39 */
  cairo_move_to (cr, 281.422, 67.75);
  cairo_line_to (cr, 281.422, 73.5);
  cairo_line_to (cr, 282.422, 73.5);
  cairo_line_to (cr, 282.422, 65.437);
  cairo_line_to (cr, 281.766, 65.437);
  cairo_curve_to (cr, 281.422, 66.672, 281.188, 66.844, 279.656, 67.047);

  cairo_line_to (cr, 279.656, 67.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 293.926, 70.516);
  cairo_line_to (cr, 291.582, 70.516);
  cairo_line_to (cr, 291.582, 68.219);
  cairo_line_to (cr, 290.785, 68.219);
  cairo_line_to (cr, 290.785, 70.516);
  cairo_line_to (cr, 288.441, 70.516);
  cairo_line_to (cr, 288.441, 71.297);
  cairo_line_to (cr, 290.785, 71.297);
  cairo_line_to (cr, 290.785, 73.609);
  cairo_line_to (cr, 291.582, 73.609);
  cairo_line_to (cr, 291.582, 71.297);
  cairo_line_to (cr, 293.926, 71.297);
  cairo_close_path (cr);
  cairo_move_to (cr, 305.867, 73.516);
  cairo_line_to (cr, 304.789, 72.656);
  cairo_curve_to (cr, 305.57, 71.797, 305.961, 70.766, 305.961, 69.469);

  cairo_curve_to (cr, 305.961, 66.937, 304.352, 65.234, 301.977, 65.234);

  cairo_curve_to (cr, 299.602, 65.234, 298.008, 66.937, 298.008, 69.5);

  cairo_curve_to (cr, 298.008, 72.031, 299.602, 73.75, 301.977, 73.75);

  cairo_curve_to (cr, 302.805, 73.75, 303.477, 73.578, 304.148, 73.187);

  cairo_line_to (cr, 305.336, 74.156);
  cairo_close_path (cr);
  cairo_move_to (cr, 303.008, 71.219);
  cairo_line_to (cr, 302.492, 71.844);
  cairo_line_to (cr, 303.32, 72.531);
  cairo_curve_to (cr, 302.836, 72.766, 302.461, 72.844, 301.977, 72.844);

  cairo_curve_to (cr, 300.211, 72.844, 299.055, 71.5, 299.055, 69.5);

  cairo_curve_to (cr, 299.055, 67.484, 300.211, 66.141, 301.977, 66.141);

  cairo_curve_to (cr, 303.727, 66.141, 304.914, 67.484, 304.914, 69.484);

  cairo_curve_to (cr, 304.914, 70.547, 304.633, 71.344, 304.023, 72.031);

  cairo_close_path (cr);
  cairo_move_to (cr, 306.32, 73.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 52 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 309.727, 67.297);
  cairo_line_to (cr, 307.211, 67.297);
  cairo_curve_to (cr, 307.273, 66.891, 307.492, 66.625, 308.07, 66.281);

  cairo_line_to (cr, 308.742, 65.891);
  cairo_curve_to (cr, 309.414, 65.531, 309.758, 65.031, 309.758, 64.437);

  cairo_curve_to (cr, 309.758, 64.031, 309.602, 63.656, 309.32, 63.391);

  cairo_curve_to (cr, 309.023, 63.141, 308.68, 63.016, 308.227, 63.016);

  cairo_curve_to (cr, 307.617, 63.016, 307.18, 63.234, 306.914, 63.641);

  cairo_curve_to (cr, 306.742, 63.906, 306.664, 64.203, 306.664, 64.687);

  cairo_line_to (cr, 307.242, 64.687);
  cairo_curve_to (cr, 307.273, 64.359, 307.305, 64.172, 307.383, 64.016);

  cairo_curve_to (cr, 307.539, 63.719, 307.852, 63.531, 308.211, 63.531);

  cairo_curve_to (cr, 308.742, 63.531, 309.148, 63.922, 309.148, 64.453);

  cairo_curve_to (cr, 309.148, 64.828, 308.93, 65.172, 308.508, 65.406);

  cairo_line_to (cr, 307.883, 65.781);
  cairo_curve_to (cr, 306.898, 66.344, 306.602, 66.812, 306.555, 67.875);

  cairo_line_to (cr, 309.727, 67.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 310.07, 67.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 53 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 8 */
  cairo_move_to (cr, 290.422, 49.75);
  cairo_line_to (cr, 290.422, 55.5);
  cairo_line_to (cr, 291.422, 55.5);
  cairo_line_to (cr, 291.422, 47.437);
  cairo_line_to (cr, 290.766, 47.437);
  cairo_curve_to (cr, 290.422, 48.672, 290.188, 48.844, 288.656, 49.047);

  cairo_line_to (cr, 288.656, 49.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 293.754, 55.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 54 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.6875);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 278.5, 60);
  cairo_line_to (cr, 305.5, 60);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 55 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 257.547, 60.562);
  cairo_line_to (cr, 252.063, 60.562);
  cairo_line_to (cr, 252.063, 61.344);
  cairo_line_to (cr, 257.547, 61.344);
  cairo_close_path (cr);
  cairo_move_to (cr, 257.547, 62.484);
  cairo_line_to (cr, 252.063, 62.484);
  cairo_line_to (cr, 252.063, 63.266);
  cairo_line_to (cr, 257.547, 63.266);
  cairo_close_path (cr);
  cairo_move_to (cr, 258.066, 64.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 56 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 245.484, 18.391);
  cairo_curve_to (cr, 245.156, 16.609, 244.125, 15.734, 242.313, 15.734);

  cairo_curve_to (cr, 241.203, 15.734, 240.313, 16.078, 239.703, 16.75);

  cairo_curve_to (cr, 238.953, 17.547, 238.547, 18.719, 238.547, 20.031);

  cairo_curve_to (cr, 238.547, 21.359, 238.969, 22.516, 239.734, 23.312);

  cairo_curve_to (cr, 240.375, 23.953, 241.188, 24.25, 242.266, 24.25);

  cairo_curve_to (cr, 244.281, 24.25, 245.406, 23.187, 245.656, 21.031);

  cairo_line_to (cr, 244.578, 21.031);
  cairo_curve_to (cr, 244.484, 21.594, 244.359, 21.969, 244.203, 22.297);

  cairo_curve_to (cr, 243.859, 22.969, 243.156, 23.344, 242.281, 23.344);

  cairo_curve_to (cr, 240.641, 23.344, 239.594, 22.047, 239.594, 20.016);

  cairo_curve_to (cr, 239.594, 17.937, 240.578, 16.641, 242.188, 16.641);

  cairo_curve_to (cr, 242.859, 16.641, 243.469, 16.844, 243.813, 17.156);

  cairo_curve_to (cr, 244.125, 17.437, 244.281, 17.781, 244.406, 18.391);

  cairo_close_path (cr);
  cairo_move_to (cr, 246.121, 24);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 57 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 250.121, 22.781);
  cairo_curve_to (cr, 250.121, 22.453, 250.105, 22.359, 249.996, 22.125);

  cairo_curve_to (cr, 249.73, 21.562, 249.168, 21.266, 248.34, 21.266);

  cairo_curve_to (cr, 247.262, 21.266, 246.59, 21.812, 246.59, 22.703);

  cairo_curve_to (cr, 246.59, 23.312, 246.902, 23.687, 247.559, 23.844);

  cairo_line_to (cr, 248.777, 24.172);
  cairo_curve_to (cr, 249.402, 24.328, 249.668, 24.578, 249.668, 24.969);

  cairo_curve_to (cr, 249.668, 25.234, 249.527, 25.5, 249.324, 25.641);

  cairo_curve_to (cr, 249.121, 25.781, 248.824, 25.859, 248.418, 25.859);

  cairo_curve_to (cr, 247.887, 25.859, 247.527, 25.719, 247.293, 25.437);

  cairo_curve_to (cr, 247.105, 25.234, 247.027, 25, 247.043, 24.687);

  cairo_line_to (cr, 246.449, 24.687);
  cairo_curve_to (cr, 246.449, 25.141, 246.543, 25.437, 246.73, 25.703);

  cairo_curve_to (cr, 247.074, 26.172, 247.637, 26.406, 248.387, 26.406);

  cairo_curve_to (cr, 248.965, 26.406, 249.449, 26.266, 249.762, 26.031);

  cairo_curve_to (cr, 250.09, 25.766, 250.293, 25.328, 250.293, 24.906);

  cairo_curve_to (cr, 250.293, 24.297, 249.918, 23.859, 249.262, 23.672);

  cairo_line_to (cr, 248.027, 23.344);
  cairo_curve_to (cr, 247.434, 23.187, 247.215, 23, 247.215, 22.625);

  cairo_curve_to (cr, 247.215, 22.125, 247.652, 21.797, 248.309, 21.797);

  cairo_curve_to (cr, 249.09, 21.797, 249.527, 22.141, 249.543, 22.781);

  cairo_close_path (cr);
  cairo_move_to (cr, 250.621, 26.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 58 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 267.984, 18.391);
  cairo_curve_to (cr, 267.656, 16.609, 266.625, 15.734, 264.813, 15.734);

  cairo_curve_to (cr, 263.703, 15.734, 262.813, 16.078, 262.203, 16.75);

  cairo_curve_to (cr, 261.453, 17.547, 261.047, 18.719, 261.047, 20.031);

  cairo_curve_to (cr, 261.047, 21.359, 261.469, 22.516, 262.234, 23.312);

  cairo_curve_to (cr, 262.875, 23.953, 263.688, 24.25, 264.766, 24.25);

  cairo_curve_to (cr, 266.781, 24.25, 267.906, 23.187, 268.156, 21.031);

  cairo_line_to (cr, 267.078, 21.031);
  cairo_curve_to (cr, 266.984, 21.594, 266.859, 21.969, 266.703, 22.297);

  cairo_curve_to (cr, 266.359, 22.969, 265.656, 23.344, 264.781, 23.344);

  cairo_curve_to (cr, 263.141, 23.344, 262.094, 22.047, 262.094, 20.016);

  cairo_curve_to (cr, 262.094, 17.937, 263.078, 16.641, 264.688, 16.641);

  cairo_curve_to (cr, 265.359, 16.641, 265.969, 16.844, 266.313, 17.156);

  cairo_curve_to (cr, 266.625, 17.437, 266.781, 17.781, 266.906, 18.391);

  cairo_close_path (cr);
  cairo_move_to (cr, 268.621, 24);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 59 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 269.855, 24.172);
  cairo_line_to (cr, 271.402, 24.172);
  cairo_curve_to (cr, 271.777, 24.172, 272.09, 24.062, 272.34, 23.828);

  cairo_curve_to (cr, 272.637, 23.547, 272.777, 23.234, 272.777, 22.781);

  cairo_curve_to (cr, 272.777, 21.859, 272.23, 21.344, 271.262, 21.344);

  cairo_line_to (cr, 269.23, 21.344);
  cairo_line_to (cr, 269.23, 26.25);
  cairo_line_to (cr, 269.855, 26.25);
  cairo_close_path (cr);
  cairo_move_to (cr, 269.855, 23.625);
  cairo_line_to (cr, 269.855, 21.906);
  cairo_line_to (cr, 271.168, 21.906);
  cairo_curve_to (cr, 271.762, 21.906, 272.121, 22.219, 272.121, 22.766);

  cairo_curve_to (cr, 272.121, 23.297, 271.762, 23.625, 271.168, 23.625);

  cairo_close_path (cr);
  cairo_move_to (cr, 273.121, 26.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 60 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 39 */
  cairo_move_to (cr, 281.422, 9.25);
  cairo_line_to (cr, 281.422, 15);
  cairo_line_to (cr, 282.422, 15);
  cairo_line_to (cr, 282.422, 6.93701);
  cairo_line_to (cr, 281.766, 6.93701);
  cairo_curve_to (cr, 281.422, 8.172, 281.188, 8.34399, 279.656, 8.547);

  cairo_line_to (cr, 279.656, 9.25);
  cairo_close_path (cr);
  cairo_move_to (cr, 293.926, 12.016);
  cairo_line_to (cr, 291.582, 12.016);
  cairo_line_to (cr, 291.582, 9.71899);
  cairo_line_to (cr, 290.785, 9.71899);
  cairo_line_to (cr, 290.785, 12.016);
  cairo_line_to (cr, 288.441, 12.016);
  cairo_line_to (cr, 288.441, 12.797);
  cairo_line_to (cr, 290.785, 12.797);
  cairo_line_to (cr, 290.785, 15.109);
  cairo_line_to (cr, 291.582, 15.109);
  cairo_line_to (cr, 291.582, 12.797);
  cairo_line_to (cr, 293.926, 12.797);
  cairo_close_path (cr);
  cairo_move_to (cr, 305.867, 15.016);
  cairo_line_to (cr, 304.789, 14.156);
  cairo_curve_to (cr, 305.57, 13.297, 305.961, 12.266, 305.961, 10.969);

  cairo_curve_to (cr, 305.961, 8.43701, 304.352, 6.73401, 301.977, 6.73401);

  cairo_curve_to (cr, 299.602, 6.73401, 298.008, 8.43701, 298.008, 11);

  cairo_curve_to (cr, 298.008, 13.531, 299.602, 15.25, 301.977, 15.25);

  cairo_curve_to (cr, 302.805, 15.25, 303.477, 15.078, 304.148, 14.687);

  cairo_line_to (cr, 305.336, 15.656);
  cairo_close_path (cr);
  cairo_move_to (cr, 303.008, 12.719);
  cairo_line_to (cr, 302.492, 13.344);
  cairo_line_to (cr, 303.32, 14.031);
  cairo_curve_to (cr, 302.836, 14.266, 302.461, 14.344, 301.977, 14.344);

  cairo_curve_to (cr, 300.211, 14.344, 299.055, 13, 299.055, 11);

  cairo_curve_to (cr, 299.055, 8.98401, 300.211, 7.64099, 301.977, 7.64099);

  cairo_curve_to (cr, 303.727, 7.64099, 304.914, 8.98401, 304.914, 10.984);

  cairo_curve_to (cr, 304.914, 12.047, 304.633, 12.844, 304.023, 13.531);

  cairo_close_path (cr);
  cairo_move_to (cr, 306.32, 15);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 61 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 309.727, 8.797);
  cairo_line_to (cr, 307.211, 8.797);
  cairo_curve_to (cr, 307.273, 8.39099, 307.492, 8.125, 308.07, 7.78101);

  cairo_line_to (cr, 308.742, 7.39099);
  cairo_curve_to (cr, 309.414, 7.03101, 309.758, 6.53101, 309.758, 5.93701);

  cairo_curve_to (cr, 309.758, 5.53101, 309.602, 5.15601, 309.32, 4.89099);

  cairo_curve_to (cr, 309.023, 4.64099, 308.68, 4.51599, 308.227, 4.51599);

  cairo_curve_to (cr, 307.617, 4.51599, 307.18, 4.73401, 306.914, 5.14099);

  cairo_curve_to (cr, 306.742, 5.40601, 306.664, 5.703, 306.664, 6.18701);

  cairo_line_to (cr, 307.242, 6.18701);
  cairo_curve_to (cr, 307.273, 5.85901, 307.305, 5.672, 307.383, 5.51599);

  cairo_curve_to (cr, 307.539, 5.21899, 307.852, 5.03101, 308.211, 5.03101);

  cairo_curve_to (cr, 308.742, 5.03101, 309.148, 5.422, 309.148, 5.953);

  cairo_curve_to (cr, 309.148, 6.328, 308.93, 6.672, 308.508, 6.90601);

  cairo_line_to (cr, 307.883, 7.28101);
  cairo_curve_to (cr, 306.898, 7.84399, 306.602, 8.31201, 306.555, 9.375);

  cairo_line_to (cr, 309.727, 9.375);
  cairo_close_path (cr);
  cairo_move_to (cr, 310.07, 9.375);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 62 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 295.797, 33.016);
  cairo_line_to (cr, 294.719, 32.156);
  cairo_curve_to (cr, 295.5, 31.297, 295.891, 30.266, 295.891, 28.969);

  cairo_curve_to (cr, 295.891, 26.437, 294.281, 24.734, 291.906, 24.734);

  cairo_curve_to (cr, 289.531, 24.734, 287.938, 26.437, 287.938, 29);

  cairo_curve_to (cr, 287.938, 31.531, 289.531, 33.25, 291.906, 33.25);

  cairo_curve_to (cr, 292.734, 33.25, 293.406, 33.078, 294.078, 32.687);

  cairo_line_to (cr, 295.266, 33.656);
  cairo_close_path (cr);
  cairo_move_to (cr, 292.938, 30.719);
  cairo_line_to (cr, 292.422, 31.344);
  cairo_line_to (cr, 293.25, 32.031);
  cairo_curve_to (cr, 292.766, 32.266, 292.391, 32.344, 291.906, 32.344);

  cairo_curve_to (cr, 290.141, 32.344, 288.984, 31, 288.984, 29);

  cairo_curve_to (cr, 288.984, 26.984, 290.141, 25.641, 291.906, 25.641);

  cairo_curve_to (cr, 293.656, 25.641, 294.844, 26.984, 294.844, 28.984);

  cairo_curve_to (cr, 294.844, 30.047, 294.563, 30.844, 293.953, 31.531);

  cairo_close_path (cr);
  cairo_move_to (cr, 296.25, 33);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 63 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 299.656, 26.797);
  cairo_line_to (cr, 297.141, 26.797);
  cairo_curve_to (cr, 297.203, 26.391, 297.422, 26.125, 298, 25.781);

  cairo_line_to (cr, 298.672, 25.391);
  cairo_curve_to (cr, 299.344, 25.031, 299.688, 24.531, 299.688, 23.937);

  cairo_curve_to (cr, 299.688, 23.531, 299.531, 23.156, 299.25, 22.891);

  cairo_curve_to (cr, 298.953, 22.641, 298.609, 22.516, 298.156, 22.516);

  cairo_curve_to (cr, 297.547, 22.516, 297.109, 22.734, 296.844, 23.141);

  cairo_curve_to (cr, 296.672, 23.406, 296.594, 23.703, 296.594, 24.187);

  cairo_line_to (cr, 297.172, 24.187);
  cairo_curve_to (cr, 297.203, 23.859, 297.234, 23.672, 297.313, 23.516);

  cairo_curve_to (cr, 297.469, 23.219, 297.781, 23.031, 298.141, 23.031);

  cairo_curve_to (cr, 298.672, 23.031, 299.078, 23.422, 299.078, 23.953);

  cairo_curve_to (cr, 299.078, 24.328, 298.859, 24.672, 298.438, 24.906);

  cairo_line_to (cr, 297.813, 25.281);
  cairo_curve_to (cr, 296.828, 25.844, 296.531, 26.312, 296.484, 27.375);

  cairo_line_to (cr, 299.656, 27.375);
  cairo_close_path (cr);
  cairo_move_to (cr, 300, 27.375);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 64 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.6875);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 278.5, 19.5);
  cairo_line_to (cr, 305.5, 19.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 65 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 257.547, 20.062);
  cairo_line_to (cr, 252.063, 20.062);
  cairo_line_to (cr, 252.063, 20.844);
  cairo_line_to (cr, 257.547, 20.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 257.547, 21.984);
  cairo_line_to (cr, 252.063, 21.984);
  cairo_line_to (cr, 252.063, 22.766);
  cairo_line_to (cr, 257.547, 22.766);
  cairo_close_path (cr);
  cairo_move_to (cr, 258.066, 24);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 66 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 6.9844, 40.891);
  cairo_curve_to (cr, 6.6563, 39.109, 5.625, 38.234, 3.8125, 38.234);

  cairo_curve_to (cr, 2.7031, 38.234, 1.8125, 38.578, 1.2031, 39.25);

  cairo_curve_to (cr, 0.453098, 40.047, 0.0469017, 41.219, 0.0469017, 42.531);

  cairo_curve_to (cr, 0.0469017, 43.859, 0.4688, 45.016, 1.2344, 45.812);

  cairo_curve_to (cr, 1.875, 46.453, 2.6875, 46.75, 3.7656, 46.75);

  cairo_curve_to (cr, 5.7813, 46.75, 6.9063, 45.687, 7.1563, 43.531);

  cairo_line_to (cr, 6.0781, 43.531);
  cairo_curve_to (cr, 5.9844, 44.094, 5.8594, 44.469, 5.7031, 44.797);

  cairo_curve_to (cr, 5.3594, 45.469, 4.6563, 45.844, 3.7813, 45.844);

  cairo_curve_to (cr, 2.1406, 45.844, 1.0938, 44.547, 1.0938, 42.516);

  cairo_curve_to (cr, 1.0938, 40.437, 2.0781, 39.141, 3.6875, 39.141);

  cairo_curve_to (cr, 4.3594, 39.141, 4.9688, 39.344, 5.3125, 39.656);

  cairo_curve_to (cr, 5.625, 39.937, 5.7813, 40.281, 5.9063, 40.891);

  cairo_close_path (cr);
  cairo_move_to (cr, 7.6211, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 67 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 8.8555, 46.672);
  cairo_line_to (cr, 10.4023, 46.672);
  cairo_curve_to (cr, 10.7773, 46.672, 11.0898, 46.562, 11.3398, 46.328);

  cairo_curve_to (cr, 11.6367, 46.047, 11.7773, 45.734, 11.7773, 45.281);

  cairo_curve_to (cr, 11.7773, 44.359, 11.2305, 43.844, 10.2617, 43.844);

  cairo_line_to (cr, 8.2305, 43.844);
  cairo_line_to (cr, 8.2305, 48.75);
  cairo_line_to (cr, 8.8555, 48.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 8.8555, 46.125);
  cairo_line_to (cr, 8.8555, 44.406);
  cairo_line_to (cr, 10.168, 44.406);
  cairo_curve_to (cr, 10.7617, 44.406, 11.1211, 44.719, 11.1211, 45.266);

  cairo_curve_to (cr, 11.1211, 45.797, 10.7617, 46.125, 10.168, 46.125);

  cairo_close_path (cr);
  cairo_move_to (cr, 12.1211, 48.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 68 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 29.4844, 40.891);
  cairo_curve_to (cr, 29.1563, 39.109, 28.125, 38.234, 26.3125, 38.234);

  cairo_curve_to (cr, 25.2031, 38.234, 24.3125, 38.578, 23.7031, 39.25);

  cairo_curve_to (cr, 22.9531, 40.047, 22.5469, 41.219, 22.5469, 42.531);

  cairo_curve_to (cr, 22.5469, 43.859, 22.9688, 45.016, 23.7344, 45.812);

  cairo_curve_to (cr, 24.375, 46.453, 25.1875, 46.75, 26.2656, 46.75);

  cairo_curve_to (cr, 28.2813, 46.75, 29.4063, 45.687, 29.6563, 43.531);

  cairo_line_to (cr, 28.5781, 43.531);
  cairo_curve_to (cr, 28.4844, 44.094, 28.3594, 44.469, 28.2031, 44.797);

  cairo_curve_to (cr, 27.8594, 45.469, 27.1563, 45.844, 26.2813, 45.844);

  cairo_curve_to (cr, 24.6406, 45.844, 23.5938, 44.547, 23.5938, 42.516);

  cairo_curve_to (cr, 23.5938, 40.437, 24.5781, 39.141, 26.1875, 39.141);

  cairo_curve_to (cr, 26.8594, 39.141, 27.4688, 39.344, 27.8125, 39.656);

  cairo_curve_to (cr, 28.125, 39.937, 28.2813, 40.281, 28.4063, 40.891);

  cairo_close_path (cr);
  cairo_move_to (cr, 30.1211, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 69 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 34.1211, 45.281);
  cairo_curve_to (cr, 34.1211, 44.953, 34.1055, 44.859, 33.9961, 44.625);

  cairo_curve_to (cr, 33.7305, 44.062, 33.168, 43.766, 32.3398, 43.766);

  cairo_curve_to (cr, 31.2617, 43.766, 30.5898, 44.312, 30.5898, 45.203);

  cairo_curve_to (cr, 30.5898, 45.812, 30.9023, 46.187, 31.5586, 46.344);

  cairo_line_to (cr, 32.7773, 46.672);
  cairo_curve_to (cr, 33.4023, 46.828, 33.668, 47.078, 33.668, 47.469);

  cairo_curve_to (cr, 33.668, 47.734, 33.5273, 48, 33.3242, 48.141);

  cairo_curve_to (cr, 33.1211, 48.281, 32.8242, 48.359, 32.418, 48.359);

  cairo_curve_to (cr, 31.8867, 48.359, 31.5273, 48.219, 31.293, 47.937);

  cairo_curve_to (cr, 31.1055, 47.734, 31.0273, 47.5, 31.043, 47.187);

  cairo_line_to (cr, 30.4492, 47.187);
  cairo_curve_to (cr, 30.4492, 47.641, 30.543, 47.937, 30.7305, 48.203);

  cairo_curve_to (cr, 31.0742, 48.672, 31.6367, 48.906, 32.3867, 48.906);

  cairo_curve_to (cr, 32.9648, 48.906, 33.4492, 48.766, 33.7617, 48.531);

  cairo_curve_to (cr, 34.0898, 48.266, 34.293, 47.828, 34.293, 47.406);

  cairo_curve_to (cr, 34.293, 46.797, 33.918, 46.359, 33.2617, 46.172);

  cairo_line_to (cr, 32.0273, 45.844);
  cairo_curve_to (cr, 31.4336, 45.687, 31.2148, 45.5, 31.2148, 45.125);

  cairo_curve_to (cr, 31.2148, 44.625, 31.6523, 44.297, 32.3086, 44.297);

  cairo_curve_to (cr, 33.0898, 44.297, 33.5273, 44.641, 33.543, 45.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 34.6211, 48.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 70 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 39 */
  cairo_move_to (cr, 42.922, 49.75);
  cairo_line_to (cr, 42.922, 55.5);
  cairo_line_to (cr, 43.922, 55.5);
  cairo_line_to (cr, 43.922, 47.437);
  cairo_line_to (cr, 43.266, 47.437);
  cairo_curve_to (cr, 42.922, 48.672, 42.688, 48.844, 41.156, 49.047);

  cairo_line_to (cr, 41.156, 49.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 55.426, 52.516);
  cairo_line_to (cr, 53.082, 52.516);
  cairo_line_to (cr, 53.082, 50.219);
  cairo_line_to (cr, 52.285, 50.219);
  cairo_line_to (cr, 52.285, 52.516);
  cairo_line_to (cr, 49.941, 52.516);
  cairo_line_to (cr, 49.941, 53.297);
  cairo_line_to (cr, 52.285, 53.297);
  cairo_line_to (cr, 52.285, 55.609);
  cairo_line_to (cr, 53.082, 55.609);
  cairo_line_to (cr, 53.082, 53.297);
  cairo_line_to (cr, 55.426, 53.297);
  cairo_close_path (cr);
  cairo_move_to (cr, 67.367, 55.516);
  cairo_line_to (cr, 66.289, 54.656);
  cairo_curve_to (cr, 67.07, 53.797, 67.461, 52.766, 67.461, 51.469);

  cairo_curve_to (cr, 67.461, 48.937, 65.852, 47.234, 63.477, 47.234);

  cairo_curve_to (cr, 61.102, 47.234, 59.508, 48.937, 59.508, 51.5);

  cairo_curve_to (cr, 59.508, 54.031, 61.102, 55.75, 63.477, 55.75);

  cairo_curve_to (cr, 64.305, 55.75, 64.977, 55.578, 65.648, 55.187);

  cairo_line_to (cr, 66.836, 56.156);
  cairo_close_path (cr);
  cairo_move_to (cr, 64.508, 53.219);
  cairo_line_to (cr, 63.992, 53.844);
  cairo_line_to (cr, 64.82, 54.531);
  cairo_curve_to (cr, 64.336, 54.766, 63.961, 54.844, 63.477, 54.844);

  cairo_curve_to (cr, 61.711, 54.844, 60.555, 53.5, 60.555, 51.5);

  cairo_curve_to (cr, 60.555, 49.484, 61.711, 48.141, 63.477, 48.141);

  cairo_curve_to (cr, 65.227, 48.141, 66.414, 49.484, 66.414, 51.484);

  cairo_curve_to (cr, 66.414, 52.547, 66.133, 53.344, 65.523, 54.031);

  cairo_close_path (cr);
  cairo_move_to (cr, 67.82, 55.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 71 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 71.227, 49.297);
  cairo_line_to (cr, 68.711, 49.297);
  cairo_curve_to (cr, 68.773, 48.891, 68.992, 48.625, 69.57, 48.281);

  cairo_line_to (cr, 70.242, 47.891);
  cairo_curve_to (cr, 70.914, 47.531, 71.258, 47.031, 71.258, 46.437);

  cairo_curve_to (cr, 71.258, 46.031, 71.102, 45.656, 70.82, 45.391);

  cairo_curve_to (cr, 70.523, 45.141, 70.18, 45.016, 69.727, 45.016);

  cairo_curve_to (cr, 69.117, 45.016, 68.68, 45.234, 68.414, 45.641);

  cairo_curve_to (cr, 68.242, 45.906, 68.164, 46.203, 68.164, 46.687);

  cairo_line_to (cr, 68.742, 46.687);
  cairo_curve_to (cr, 68.773, 46.359, 68.805, 46.172, 68.883, 46.016);

  cairo_curve_to (cr, 69.039, 45.719, 69.352, 45.531, 69.711, 45.531);

  cairo_curve_to (cr, 70.242, 45.531, 70.648, 45.922, 70.648, 46.453);

  cairo_curve_to (cr, 70.648, 46.828, 70.43, 47.172, 70.008, 47.406);

  cairo_line_to (cr, 69.383, 47.781);
  cairo_curve_to (cr, 68.398, 48.344, 68.102, 48.812, 68.055, 49.875);

  cairo_line_to (cr, 71.227, 49.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 71.57, 49.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 72 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 57.297, 37.516);
  cairo_line_to (cr, 56.219, 36.656);
  cairo_curve_to (cr, 57, 35.797, 57.391, 34.766, 57.391, 33.469);

  cairo_curve_to (cr, 57.391, 30.937, 55.781, 29.234, 53.406, 29.234);

  cairo_curve_to (cr, 51.031, 29.234, 49.438, 30.937, 49.438, 33.5);

  cairo_curve_to (cr, 49.438, 36.031, 51.031, 37.75, 53.406, 37.75);

  cairo_curve_to (cr, 54.234, 37.75, 54.906, 37.578, 55.578, 37.187);

  cairo_line_to (cr, 56.766, 38.156);
  cairo_close_path (cr);
  cairo_move_to (cr, 54.438, 35.219);
  cairo_line_to (cr, 53.922, 35.844);
  cairo_line_to (cr, 54.75, 36.531);
  cairo_curve_to (cr, 54.266, 36.766, 53.891, 36.844, 53.406, 36.844);

  cairo_curve_to (cr, 51.641, 36.844, 50.484, 35.5, 50.484, 33.5);

  cairo_curve_to (cr, 50.484, 31.484, 51.641, 30.141, 53.406, 30.141);

  cairo_curve_to (cr, 55.156, 30.141, 56.344, 31.484, 56.344, 33.484);

  cairo_curve_to (cr, 56.344, 34.547, 56.063, 35.344, 55.453, 36.031);

  cairo_close_path (cr);
  cairo_move_to (cr, 57.75, 37.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 73 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 61.156, 31.297);
  cairo_line_to (cr, 58.641, 31.297);
  cairo_curve_to (cr, 58.703, 30.891, 58.922, 30.625, 59.5, 30.281);

  cairo_line_to (cr, 60.172, 29.891);
  cairo_curve_to (cr, 60.844, 29.531, 61.188, 29.031, 61.188, 28.437);

  cairo_curve_to (cr, 61.188, 28.031, 61.031, 27.656, 60.75, 27.391);

  cairo_curve_to (cr, 60.453, 27.141, 60.109, 27.016, 59.656, 27.016);

  cairo_curve_to (cr, 59.047, 27.016, 58.609, 27.234, 58.344, 27.641);

  cairo_curve_to (cr, 58.172, 27.906, 58.094, 28.203, 58.094, 28.687);

  cairo_line_to (cr, 58.672, 28.687);
  cairo_curve_to (cr, 58.703, 28.359, 58.734, 28.172, 58.813, 28.016);

  cairo_curve_to (cr, 58.969, 27.719, 59.281, 27.531, 59.641, 27.531);

  cairo_curve_to (cr, 60.172, 27.531, 60.578, 27.922, 60.578, 28.453);

  cairo_curve_to (cr, 60.578, 28.828, 60.359, 29.172, 59.938, 29.406);

  cairo_line_to (cr, 59.313, 29.781);
  cairo_curve_to (cr, 58.328, 30.344, 58.031, 30.812, 57.984, 31.875);

  cairo_line_to (cr, 61.156, 31.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 61.5, 31.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 74 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.6875);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 40, 42);
  cairo_line_to (cr, 67, 42);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 75 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 19.0469, 42.562);
  cairo_line_to (cr, 13.5625, 42.562);
  cairo_line_to (cr, 13.5625, 43.344);
  cairo_line_to (cr, 19.0469, 43.344);
  cairo_close_path (cr);
  cairo_move_to (cr, 19.0469, 44.484);
  cairo_line_to (cr, 13.5625, 44.484);
  cairo_line_to (cr, 13.5625, 45.266);
  cairo_line_to (cr, 19.0469, 45.266);
  cairo_close_path (cr);
  cairo_move_to (cr, 19.5664, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 76 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 105.109, 43);
  cairo_line_to (cr, 107.813, 43);
  cairo_curve_to (cr, 108.75, 43, 109.172, 43.437, 109.172, 44.453);

  cairo_line_to (cr, 109.156, 45.172);
  cairo_curve_to (cr, 109.156, 45.672, 109.25, 46.172, 109.406, 46.5);

  cairo_line_to (cr, 110.672, 46.5);
  cairo_line_to (cr, 110.672, 46.25);
  cairo_curve_to (cr, 110.281, 45.969, 110.203, 45.687, 110.188, 44.609);

  cairo_curve_to (cr, 110.172, 43.266, 109.953, 42.859, 109.063, 42.484);

  cairo_curve_to (cr, 109.984, 42.031, 110.359, 41.484, 110.359, 40.547);

  cairo_curve_to (cr, 110.359, 39.141, 109.484, 38.359, 107.859, 38.359);

  cairo_line_to (cr, 104.047, 38.359);
  cairo_line_to (cr, 104.047, 46.5);
  cairo_line_to (cr, 105.109, 46.5);
  cairo_close_path (cr);
  cairo_move_to (cr, 105.109, 42.078);
  cairo_line_to (cr, 105.109, 39.281);
  cairo_line_to (cr, 107.641, 39.281);
  cairo_curve_to (cr, 108.234, 39.281, 108.578, 39.375, 108.828, 39.594);

  cairo_curve_to (cr, 109.125, 39.828, 109.266, 40.203, 109.266, 40.672);

  cairo_curve_to (cr, 109.266, 41.641, 108.766, 42.078, 107.641, 42.078);

  cairo_close_path (cr);
  cairo_move_to (cr, 111.121, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 77 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 112.355, 46.672);
  cairo_line_to (cr, 113.902, 46.672);
  cairo_curve_to (cr, 114.277, 46.672, 114.59, 46.562, 114.84, 46.328);

  cairo_curve_to (cr, 115.137, 46.047, 115.277, 45.734, 115.277, 45.281);

  cairo_curve_to (cr, 115.277, 44.359, 114.73, 43.844, 113.762, 43.844);

  cairo_line_to (cr, 111.73, 43.844);
  cairo_line_to (cr, 111.73, 48.75);
  cairo_line_to (cr, 112.355, 48.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 112.355, 46.125);
  cairo_line_to (cr, 112.355, 44.406);
  cairo_line_to (cr, 113.668, 44.406);
  cairo_curve_to (cr, 114.262, 44.406, 114.621, 44.719, 114.621, 45.266);

  cairo_curve_to (cr, 114.621, 45.797, 114.262, 46.125, 113.668, 46.125);

  cairo_close_path (cr);
  cairo_move_to (cr, 115.621, 48.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 78 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 127.609, 43);
  cairo_line_to (cr, 130.313, 43);
  cairo_curve_to (cr, 131.25, 43, 131.672, 43.437, 131.672, 44.453);

  cairo_line_to (cr, 131.656, 45.172);
  cairo_curve_to (cr, 131.656, 45.672, 131.75, 46.172, 131.906, 46.5);

  cairo_line_to (cr, 133.172, 46.5);
  cairo_line_to (cr, 133.172, 46.25);
  cairo_curve_to (cr, 132.781, 45.969, 132.703, 45.687, 132.688, 44.609);

  cairo_curve_to (cr, 132.672, 43.266, 132.453, 42.859, 131.563, 42.484);

  cairo_curve_to (cr, 132.484, 42.031, 132.859, 41.484, 132.859, 40.547);

  cairo_curve_to (cr, 132.859, 39.141, 131.984, 38.359, 130.359, 38.359);

  cairo_line_to (cr, 126.547, 38.359);
  cairo_line_to (cr, 126.547, 46.5);
  cairo_line_to (cr, 127.609, 46.5);
  cairo_close_path (cr);
  cairo_move_to (cr, 127.609, 42.078);
  cairo_line_to (cr, 127.609, 39.281);
  cairo_line_to (cr, 130.141, 39.281);
  cairo_curve_to (cr, 130.734, 39.281, 131.078, 39.375, 131.328, 39.594);

  cairo_curve_to (cr, 131.625, 39.828, 131.766, 40.203, 131.766, 40.672);

  cairo_curve_to (cr, 131.766, 41.641, 131.266, 42.078, 130.141, 42.078);

  cairo_close_path (cr);
  cairo_move_to (cr, 133.621, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 79 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 137.621, 45.281);
  cairo_curve_to (cr, 137.621, 44.953, 137.605, 44.859, 137.496, 44.625);

  cairo_curve_to (cr, 137.23, 44.062, 136.668, 43.766, 135.84, 43.766);

  cairo_curve_to (cr, 134.762, 43.766, 134.09, 44.312, 134.09, 45.203);

  cairo_curve_to (cr, 134.09, 45.812, 134.402, 46.187, 135.059, 46.344);

  cairo_line_to (cr, 136.277, 46.672);
  cairo_curve_to (cr, 136.902, 46.828, 137.168, 47.078, 137.168, 47.469);

  cairo_curve_to (cr, 137.168, 47.734, 137.027, 48, 136.824, 48.141);

  cairo_curve_to (cr, 136.621, 48.281, 136.324, 48.359, 135.918, 48.359);

  cairo_curve_to (cr, 135.387, 48.359, 135.027, 48.219, 134.793, 47.937);

  cairo_curve_to (cr, 134.605, 47.734, 134.527, 47.5, 134.543, 47.187);

  cairo_line_to (cr, 133.949, 47.187);
  cairo_curve_to (cr, 133.949, 47.641, 134.043, 47.937, 134.23, 48.203);

  cairo_curve_to (cr, 134.574, 48.672, 135.137, 48.906, 135.887, 48.906);

  cairo_curve_to (cr, 136.465, 48.906, 136.949, 48.766, 137.262, 48.531);

  cairo_curve_to (cr, 137.59, 48.266, 137.793, 47.828, 137.793, 47.406);

  cairo_curve_to (cr, 137.793, 46.797, 137.418, 46.359, 136.762, 46.172);

  cairo_line_to (cr, 135.527, 45.844);
  cairo_curve_to (cr, 134.934, 45.687, 134.715, 45.5, 134.715, 45.125);

  cairo_curve_to (cr, 134.715, 44.625, 135.152, 44.297, 135.809, 44.297);

  cairo_curve_to (cr, 136.59, 44.297, 137.027, 44.641, 137.043, 45.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 138.121, 48.75);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 80 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 46 */
  cairo_move_to (cr, 146.172, 38.359);
  cairo_curve_to (cr, 145.031, 39.828, 144.328, 41.859, 144.328, 43.609);

  cairo_curve_to (cr, 144.328, 45.375, 145.031, 47.406, 146.172, 48.859);

  cairo_line_to (cr, 146.797, 48.859);
  cairo_curve_to (cr, 145.797, 47.266, 145.234, 45.391, 145.234, 43.609);

  cairo_curve_to (cr, 145.234, 41.844, 145.797, 39.953, 146.797, 38.359);

  cairo_close_path (cr);
  cairo_move_to (cr, 150.168, 40.75);
  cairo_line_to (cr, 150.168, 46.5);
  cairo_line_to (cr, 151.168, 46.5);
  cairo_line_to (cr, 151.168, 38.437);
  cairo_line_to (cr, 150.512, 38.437);
  cairo_curve_to (cr, 150.168, 39.672, 149.934, 39.844, 148.402, 40.047);

  cairo_line_to (cr, 148.402, 40.75);
  cairo_close_path (cr);
  cairo_move_to (cr, 162.672, 43.516);
  cairo_line_to (cr, 160.328, 43.516);
  cairo_line_to (cr, 160.328, 41.219);
  cairo_line_to (cr, 159.531, 41.219);
  cairo_line_to (cr, 159.531, 43.516);
  cairo_line_to (cr, 157.188, 43.516);
  cairo_line_to (cr, 157.188, 44.297);
  cairo_line_to (cr, 159.531, 44.297);
  cairo_line_to (cr, 159.531, 46.609);
  cairo_line_to (cr, 160.328, 46.609);
  cairo_line_to (cr, 160.328, 44.297);
  cairo_line_to (cr, 162.672, 44.297);
  cairo_close_path (cr);
  cairo_move_to (cr, 174.613, 46.516);
  cairo_line_to (cr, 173.535, 45.656);
  cairo_curve_to (cr, 174.316, 44.797, 174.707, 43.766, 174.707, 42.469);

  cairo_curve_to (cr, 174.707, 39.937, 173.098, 38.234, 170.723, 38.234);

  cairo_curve_to (cr, 168.348, 38.234, 166.754, 39.937, 166.754, 42.5);

  cairo_curve_to (cr, 166.754, 45.031, 168.348, 46.75, 170.723, 46.75);

  cairo_curve_to (cr, 171.551, 46.75, 172.223, 46.578, 172.895, 46.187);

  cairo_line_to (cr, 174.082, 47.156);
  cairo_close_path (cr);
  cairo_move_to (cr, 171.754, 44.219);
  cairo_line_to (cr, 171.238, 44.844);
  cairo_line_to (cr, 172.066, 45.531);
  cairo_curve_to (cr, 171.582, 45.766, 171.207, 45.844, 170.723, 45.844);

  cairo_curve_to (cr, 168.957, 45.844, 167.801, 44.5, 167.801, 42.5);

  cairo_curve_to (cr, 167.801, 40.484, 168.957, 39.141, 170.723, 39.141);

  cairo_curve_to (cr, 172.473, 39.141, 173.66, 40.484, 173.66, 42.484);

  cairo_curve_to (cr, 173.66, 43.547, 173.379, 44.344, 172.77, 45.031);

  cairo_close_path (cr);
  cairo_move_to (cr, 175.066, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 81 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 178.473, 40.297);
  cairo_line_to (cr, 175.957, 40.297);
  cairo_curve_to (cr, 176.02, 39.891, 176.238, 39.625, 176.816, 39.281);

  cairo_line_to (cr, 177.488, 38.891);
  cairo_curve_to (cr, 178.16, 38.531, 178.504, 38.031, 178.504, 37.437);

  cairo_curve_to (cr, 178.504, 37.031, 178.348, 36.656, 178.066, 36.391);

  cairo_curve_to (cr, 177.77, 36.141, 177.426, 36.016, 176.973, 36.016);

  cairo_curve_to (cr, 176.363, 36.016, 175.926, 36.234, 175.66, 36.641);

  cairo_curve_to (cr, 175.488, 36.906, 175.41, 37.203, 175.41, 37.687);

  cairo_line_to (cr, 175.988, 37.687);
  cairo_curve_to (cr, 176.02, 37.359, 176.051, 37.172, 176.129, 37.016);

  cairo_curve_to (cr, 176.285, 36.719, 176.598, 36.531, 176.957, 36.531);

  cairo_curve_to (cr, 177.488, 36.531, 177.895, 36.922, 177.895, 37.453);

  cairo_curve_to (cr, 177.895, 37.828, 177.676, 38.172, 177.254, 38.406);

  cairo_line_to (cr, 176.629, 38.781);
  cairo_curve_to (cr, 175.645, 39.344, 175.348, 39.812, 175.301, 40.875);

  cairo_line_to (cr, 178.473, 40.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 178.816, 40.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 82 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 7 */
  cairo_move_to (cr, 179.863, 48.859);
  cairo_curve_to (cr, 181.004, 47.406, 181.707, 45.375, 181.707, 43.625);

  cairo_curve_to (cr, 181.707, 41.859, 181.004, 39.828, 179.863, 38.359);

  cairo_line_to (cr, 179.254, 38.359);
  cairo_curve_to (cr, 180.238, 39.969, 180.801, 41.844, 180.801, 43.625);

  cairo_curve_to (cr, 180.801, 45.391, 180.238, 47.281, 179.254, 48.859);

  cairo_close_path (cr);
  cairo_move_to (cr, 182.563, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 83 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 122.547, 42.562);
  cairo_line_to (cr, 117.063, 42.562);
  cairo_line_to (cr, 117.063, 43.344);
  cairo_line_to (cr, 122.547, 43.344);
  cairo_close_path (cr);
  cairo_move_to (cr, 122.547, 44.484);
  cairo_line_to (cr, 117.063, 44.484);
  cairo_line_to (cr, 117.063, 45.266);
  cairo_line_to (cr, 122.547, 45.266);
  cairo_close_path (cr);
  cairo_move_to (cr, 123.066, 46.5);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 84 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 18 */
  cairo_move_to (cr, 111.297, 78.016);
  cairo_line_to (cr, 110.219, 77.156);
  cairo_curve_to (cr, 111, 76.297, 111.391, 75.266, 111.391, 73.969);

  cairo_curve_to (cr, 111.391, 71.437, 109.781, 69.734, 107.406, 69.734);

  cairo_curve_to (cr, 105.031, 69.734, 103.438, 71.437, 103.438, 74);

  cairo_curve_to (cr, 103.438, 76.531, 105.031, 78.25, 107.406, 78.25);

  cairo_curve_to (cr, 108.234, 78.25, 108.906, 78.078, 109.578, 77.687);

  cairo_line_to (cr, 110.766, 78.656);
  cairo_close_path (cr);
  cairo_move_to (cr, 108.438, 75.719);
  cairo_line_to (cr, 107.922, 76.344);
  cairo_line_to (cr, 108.75, 77.031);
  cairo_curve_to (cr, 108.266, 77.266, 107.891, 77.344, 107.406, 77.344);

  cairo_curve_to (cr, 105.641, 77.344, 104.484, 76, 104.484, 74);

  cairo_curve_to (cr, 104.484, 71.984, 105.641, 70.641, 107.406, 70.641);

  cairo_curve_to (cr, 109.156, 70.641, 110.344, 71.984, 110.344, 73.984);

  cairo_curve_to (cr, 110.344, 75.047, 110.063, 75.844, 109.453, 76.531);

  cairo_close_path (cr);
  cairo_move_to (cr, 111.75, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 85 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 25 */
  cairo_move_to (cr, 125.844, 72.562);
  cairo_curve_to (cr, 126.266, 72.672, 126.5, 72.812, 126.75, 73.031);

  cairo_curve_to (cr, 127.25, 73.484, 127.531, 74.359, 127.531, 75.344);

  cairo_curve_to (cr, 127.531, 76.062, 127.406, 76.766, 127.188, 77.156);

  cairo_curve_to (cr, 127.047, 77.422, 126.766, 77.594, 126.469, 77.594);

  cairo_curve_to (cr, 125.891, 77.594, 125.531, 77.125, 125.375, 76.219);

  cairo_curve_to (cr, 125.641, 75.469, 125.719, 75.141, 125.719, 74.578);

  cairo_curve_to (cr, 125.719, 73.812, 125.484, 73.344, 125.109, 73.344);

  cairo_curve_to (cr, 124.719, 73.344, 124.5, 73.812, 124.5, 74.578);

  cairo_curve_to (cr, 124.5, 75.141, 124.563, 75.469, 124.828, 76.219);

  cairo_curve_to (cr, 124.672, 77.125, 124.313, 77.594, 123.734, 77.594);

  cairo_curve_to (cr, 123.438, 77.594, 123.156, 77.422, 123.031, 77.156);

  cairo_curve_to (cr, 122.813, 76.766, 122.672, 76.062, 122.672, 75.344);

  cairo_curve_to (cr, 122.672, 74.437, 122.938, 73.594, 123.359, 73.141);

  cairo_curve_to (cr, 123.609, 72.859, 123.891, 72.703, 124.375, 72.562);

  cairo_line_to (cr, 124.375, 72.281);
  cairo_curve_to (cr, 123.766, 72.281, 123.359, 72.359, 123.016, 72.5);

  cairo_curve_to (cr, 122.078, 72.906, 121.469, 74.047, 121.469, 75.422);

  cairo_curve_to (cr, 121.469, 76.969, 122.359, 78.141, 123.516, 78.141);

  cairo_curve_to (cr, 124.203, 78.141, 124.719, 77.781, 125.109, 77.016);

  cairo_curve_to (cr, 125.484, 77.781, 126.016, 78.141, 126.703, 78.141);

  cairo_curve_to (cr, 127.859, 78.141, 128.734, 76.969, 128.734, 75.422);

  cairo_curve_to (cr, 128.734, 74.109, 128.188, 73, 127.328, 72.562);

  cairo_curve_to (cr, 126.953, 72.359, 126.516, 72.281, 125.844, 72.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 128.715, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 86 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 139.324, 72.391);
  cairo_curve_to (cr, 138.996, 70.609, 137.965, 69.734, 136.152, 69.734);

  cairo_curve_to (cr, 135.043, 69.734, 134.152, 70.078, 133.543, 70.75);

  cairo_curve_to (cr, 132.793, 71.547, 132.387, 72.719, 132.387, 74.031);

  cairo_curve_to (cr, 132.387, 75.359, 132.809, 76.516, 133.574, 77.312);

  cairo_curve_to (cr, 134.215, 77.953, 135.027, 78.25, 136.105, 78.25);

  cairo_curve_to (cr, 138.121, 78.25, 139.246, 77.187, 139.496, 75.031);

  cairo_line_to (cr, 138.418, 75.031);
  cairo_curve_to (cr, 138.324, 75.594, 138.199, 75.969, 138.043, 76.297);

  cairo_curve_to (cr, 137.699, 76.969, 136.996, 77.344, 136.121, 77.344);

  cairo_curve_to (cr, 134.48, 77.344, 133.434, 76.047, 133.434, 74.016);

  cairo_curve_to (cr, 133.434, 71.937, 134.418, 70.641, 136.027, 70.641);

  cairo_curve_to (cr, 136.699, 70.641, 137.309, 70.844, 137.652, 71.156);

  cairo_curve_to (cr, 137.965, 71.437, 138.121, 71.781, 138.246, 72.391);

  cairo_close_path (cr);
  cairo_move_to (cr, 139.961, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 87 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 141.195, 78.172);
  cairo_line_to (cr, 142.742, 78.172);
  cairo_curve_to (cr, 143.117, 78.172, 143.43, 78.062, 143.68, 77.828);

  cairo_curve_to (cr, 143.977, 77.547, 144.117, 77.234, 144.117, 76.781);

  cairo_curve_to (cr, 144.117, 75.859, 143.57, 75.344, 142.602, 75.344);

  cairo_line_to (cr, 140.57, 75.344);
  cairo_line_to (cr, 140.57, 80.25);
  cairo_line_to (cr, 141.195, 80.25);
  cairo_close_path (cr);
  cairo_move_to (cr, 141.195, 77.625);
  cairo_line_to (cr, 141.195, 75.906);
  cairo_line_to (cr, 142.508, 75.906);
  cairo_curve_to (cr, 143.102, 75.906, 143.461, 76.219, 143.461, 76.766);

  cairo_curve_to (cr, 143.461, 77.297, 143.102, 77.625, 142.508, 77.625);

  cairo_close_path (cr);
  cairo_move_to (cr, 144.461, 80.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 88 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 149.695, 74.5);
  cairo_line_to (cr, 152.398, 74.5);
  cairo_curve_to (cr, 153.336, 74.5, 153.758, 74.937, 153.758, 75.953);

  cairo_line_to (cr, 153.742, 76.672);
  cairo_curve_to (cr, 153.742, 77.172, 153.836, 77.672, 153.992, 78);

  cairo_line_to (cr, 155.258, 78);
  cairo_line_to (cr, 155.258, 77.75);
  cairo_curve_to (cr, 154.867, 77.469, 154.789, 77.187, 154.773, 76.109);

  cairo_curve_to (cr, 154.758, 74.766, 154.539, 74.359, 153.648, 73.984);

  cairo_curve_to (cr, 154.57, 73.531, 154.945, 72.984, 154.945, 72.047);

  cairo_curve_to (cr, 154.945, 70.641, 154.07, 69.859, 152.445, 69.859);

  cairo_line_to (cr, 148.633, 69.859);
  cairo_line_to (cr, 148.633, 78);
  cairo_line_to (cr, 149.695, 78);
  cairo_close_path (cr);
  cairo_move_to (cr, 149.695, 73.578);
  cairo_line_to (cr, 149.695, 70.781);
  cairo_line_to (cr, 152.227, 70.781);
  cairo_curve_to (cr, 152.82, 70.781, 153.164, 70.875, 153.414, 71.094);

  cairo_curve_to (cr, 153.711, 71.328, 153.852, 71.703, 153.852, 72.172);

  cairo_curve_to (cr, 153.852, 73.141, 153.352, 73.578, 152.227, 73.578);

  cairo_close_path (cr);
  cairo_move_to (cr, 155.707, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 89 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 156.941, 78.172);
  cairo_line_to (cr, 158.488, 78.172);
  cairo_curve_to (cr, 158.863, 78.172, 159.176, 78.062, 159.426, 77.828);

  cairo_curve_to (cr, 159.723, 77.547, 159.863, 77.234, 159.863, 76.781);

  cairo_curve_to (cr, 159.863, 75.859, 159.316, 75.344, 158.348, 75.344);

  cairo_line_to (cr, 156.316, 75.344);
  cairo_line_to (cr, 156.316, 80.25);
  cairo_line_to (cr, 156.941, 80.25);
  cairo_close_path (cr);
  cairo_move_to (cr, 156.941, 77.625);
  cairo_line_to (cr, 156.941, 75.906);
  cairo_line_to (cr, 158.254, 75.906);
  cairo_curve_to (cr, 158.848, 75.906, 159.207, 76.219, 159.207, 76.766);

  cairo_curve_to (cr, 159.207, 77.297, 158.848, 77.625, 158.254, 77.625);

  cairo_close_path (cr);
  cairo_move_to (cr, 160.207, 80.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 90 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 8 */
  cairo_move_to (cr, 186.922, 63.25);
  cairo_line_to (cr, 186.922, 69);
  cairo_line_to (cr, 187.922, 69);
  cairo_line_to (cr, 187.922, 60.937);
  cairo_line_to (cr, 187.266, 60.937);
  cairo_curve_to (cr, 186.922, 62.172, 186.688, 62.344, 185.156, 62.547);

  cairo_line_to (cr, 185.156, 63.25);
  cairo_close_path (cr);
  cairo_move_to (cr, 190.254, 69);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 91 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 1.6875);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 170.5, 73.5);
  cairo_line_to (cr, 206.5, 73.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 92 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 118.047, 74.062);
  cairo_line_to (cr, 112.563, 74.062);
  cairo_line_to (cr, 112.563, 74.844);
  cairo_line_to (cr, 118.047, 74.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 118.047, 75.984);
  cairo_line_to (cr, 112.563, 75.984);
  cairo_line_to (cr, 112.563, 76.766);
  cairo_line_to (cr, 118.047, 76.766);
  cairo_close_path (cr);
  cairo_move_to (cr, 118.566, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 93 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 167.547, 74.062);
  cairo_line_to (cr, 162.063, 74.062);
  cairo_line_to (cr, 162.063, 74.844);
  cairo_line_to (cr, 167.547, 74.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 167.547, 75.984);
  cairo_line_to (cr, 162.063, 75.984);
  cairo_line_to (cr, 162.063, 76.766);
  cairo_line_to (cr, 167.547, 76.766);
  cairo_close_path (cr);
  cairo_move_to (cr, 168.066, 78);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 94 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 25 */
  cairo_move_to (cr, 175.344, 81.562);
  cairo_curve_to (cr, 175.766, 81.672, 176, 81.812, 176.25, 82.031);

  cairo_curve_to (cr, 176.75, 82.484, 177.031, 83.359, 177.031, 84.344);

  cairo_curve_to (cr, 177.031, 85.062, 176.906, 85.766, 176.688, 86.156);

  cairo_curve_to (cr, 176.547, 86.422, 176.266, 86.594, 175.969, 86.594);

  cairo_curve_to (cr, 175.391, 86.594, 175.031, 86.125, 174.875, 85.219);

  cairo_curve_to (cr, 175.141, 84.469, 175.219, 84.141, 175.219, 83.578);

  cairo_curve_to (cr, 175.219, 82.812, 174.984, 82.344, 174.609, 82.344);

  cairo_curve_to (cr, 174.219, 82.344, 174, 82.812, 174, 83.578);

  cairo_curve_to (cr, 174, 84.141, 174.063, 84.469, 174.328, 85.219);

  cairo_curve_to (cr, 174.172, 86.125, 173.813, 86.594, 173.234, 86.594);

  cairo_curve_to (cr, 172.938, 86.594, 172.656, 86.422, 172.531, 86.156);

  cairo_curve_to (cr, 172.313, 85.766, 172.172, 85.062, 172.172, 84.344);

  cairo_curve_to (cr, 172.172, 83.437, 172.438, 82.594, 172.859, 82.141);

  cairo_curve_to (cr, 173.109, 81.859, 173.391, 81.703, 173.875, 81.562);

  cairo_line_to (cr, 173.875, 81.281);
  cairo_curve_to (cr, 173.266, 81.281, 172.859, 81.359, 172.516, 81.5);

  cairo_curve_to (cr, 171.578, 81.906, 170.969, 83.047, 170.969, 84.422);

  cairo_curve_to (cr, 170.969, 85.969, 171.859, 87.141, 173.016, 87.141);

  cairo_curve_to (cr, 173.703, 87.141, 174.219, 86.781, 174.609, 86.016);

  cairo_curve_to (cr, 174.984, 86.781, 175.516, 87.141, 176.203, 87.141);

  cairo_curve_to (cr, 177.359, 87.141, 178.234, 85.969, 178.234, 84.422);

  cairo_curve_to (cr, 178.234, 83.109, 177.688, 82, 176.828, 81.562);

  cairo_curve_to (cr, 176.453, 81.359, 176.016, 81.281, 175.344, 81.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 178.215, 87);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 95 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 15 */
  cairo_move_to (cr, 188.824, 81.391);
  cairo_curve_to (cr, 188.496, 79.609, 187.465, 78.734, 185.652, 78.734);

  cairo_curve_to (cr, 184.543, 78.734, 183.652, 79.078, 183.043, 79.75);

  cairo_curve_to (cr, 182.293, 80.547, 181.887, 81.719, 181.887, 83.031);

  cairo_curve_to (cr, 181.887, 84.359, 182.309, 85.516, 183.074, 86.312);

  cairo_curve_to (cr, 183.715, 86.953, 184.527, 87.25, 185.605, 87.25);

  cairo_curve_to (cr, 187.621, 87.25, 188.746, 86.187, 188.996, 84.031);

  cairo_line_to (cr, 187.918, 84.031);
  cairo_curve_to (cr, 187.824, 84.594, 187.699, 84.969, 187.543, 85.297);

  cairo_curve_to (cr, 187.199, 85.969, 186.496, 86.344, 185.621, 86.344);

  cairo_curve_to (cr, 183.98, 86.344, 182.934, 85.047, 182.934, 83.016);

  cairo_curve_to (cr, 182.934, 80.937, 183.918, 79.641, 185.527, 79.641);

  cairo_curve_to (cr, 186.199, 79.641, 186.809, 79.844, 187.152, 80.156);

  cairo_curve_to (cr, 187.465, 80.437, 187.621, 80.781, 187.746, 81.391);

  cairo_close_path (cr);
  cairo_move_to (cr, 189.461, 87);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 96 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 193.461, 85.781);
  cairo_curve_to (cr, 193.461, 85.453, 193.445, 85.359, 193.336, 85.125);

  cairo_curve_to (cr, 193.07, 84.562, 192.508, 84.266, 191.68, 84.266);

  cairo_curve_to (cr, 190.602, 84.266, 189.93, 84.812, 189.93, 85.703);

  cairo_curve_to (cr, 189.93, 86.312, 190.242, 86.687, 190.898, 86.844);

  cairo_line_to (cr, 192.117, 87.172);
  cairo_curve_to (cr, 192.742, 87.328, 193.008, 87.578, 193.008, 87.969);

  cairo_curve_to (cr, 193.008, 88.234, 192.867, 88.5, 192.664, 88.641);

  cairo_curve_to (cr, 192.461, 88.781, 192.164, 88.859, 191.758, 88.859);

  cairo_curve_to (cr, 191.227, 88.859, 190.867, 88.719, 190.633, 88.437);

  cairo_curve_to (cr, 190.445, 88.234, 190.367, 88, 190.383, 87.687);

  cairo_line_to (cr, 189.789, 87.687);
  cairo_curve_to (cr, 189.789, 88.141, 189.883, 88.437, 190.07, 88.703);

  cairo_curve_to (cr, 190.414, 89.172, 190.977, 89.406, 191.727, 89.406);

  cairo_curve_to (cr, 192.305, 89.406, 192.789, 89.266, 193.102, 89.031);

  cairo_curve_to (cr, 193.43, 88.766, 193.633, 88.328, 193.633, 87.906);

  cairo_curve_to (cr, 193.633, 87.297, 193.258, 86.859, 192.602, 86.672);

  cairo_line_to (cr, 191.367, 86.344);
  cairo_curve_to (cr, 190.773, 86.187, 190.555, 86, 190.555, 85.625);

  cairo_curve_to (cr, 190.555, 85.125, 190.992, 84.797, 191.648, 84.797);

  cairo_curve_to (cr, 192.43, 84.797, 192.867, 85.141, 192.883, 85.781);

  cairo_close_path (cr);
  cairo_move_to (cr, 193.961, 89.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 97 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 199.195, 83.5);
  cairo_line_to (cr, 201.898, 83.5);
  cairo_curve_to (cr, 202.836, 83.5, 203.258, 83.937, 203.258, 84.953);

  cairo_line_to (cr, 203.242, 85.672);
  cairo_curve_to (cr, 203.242, 86.172, 203.336, 86.672, 203.492, 87);

  cairo_line_to (cr, 204.758, 87);
  cairo_line_to (cr, 204.758, 86.75);
  cairo_curve_to (cr, 204.367, 86.469, 204.289, 86.187, 204.273, 85.109);

  cairo_curve_to (cr, 204.258, 83.766, 204.039, 83.359, 203.148, 82.984);

  cairo_curve_to (cr, 204.07, 82.531, 204.445, 81.984, 204.445, 81.047);

  cairo_curve_to (cr, 204.445, 79.641, 203.57, 78.859, 201.945, 78.859);

  cairo_line_to (cr, 198.133, 78.859);
  cairo_line_to (cr, 198.133, 87);
  cairo_line_to (cr, 199.195, 87);
  cairo_close_path (cr);
  cairo_move_to (cr, 199.195, 82.578);
  cairo_line_to (cr, 199.195, 79.781);
  cairo_line_to (cr, 201.727, 79.781);
  cairo_curve_to (cr, 202.32, 79.781, 202.664, 79.875, 202.914, 80.094);

  cairo_curve_to (cr, 203.211, 80.328, 203.352, 80.703, 203.352, 81.172);

  cairo_curve_to (cr, 203.352, 82.141, 202.852, 82.578, 201.727, 82.578);

  cairo_close_path (cr);
  cairo_move_to (cr, 205.207, 87);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 98 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 22 */
  cairo_move_to (cr, 209.207, 85.781);
  cairo_curve_to (cr, 209.207, 85.453, 209.191, 85.359, 209.082, 85.125);

  cairo_curve_to (cr, 208.816, 84.562, 208.254, 84.266, 207.426, 84.266);

  cairo_curve_to (cr, 206.348, 84.266, 205.676, 84.812, 205.676, 85.703);

  cairo_curve_to (cr, 205.676, 86.312, 205.988, 86.687, 206.645, 86.844);

  cairo_line_to (cr, 207.863, 87.172);
  cairo_curve_to (cr, 208.488, 87.328, 208.754, 87.578, 208.754, 87.969);

  cairo_curve_to (cr, 208.754, 88.234, 208.613, 88.5, 208.41, 88.641);

  cairo_curve_to (cr, 208.207, 88.781, 207.91, 88.859, 207.504, 88.859);

  cairo_curve_to (cr, 206.973, 88.859, 206.613, 88.719, 206.379, 88.437);

  cairo_curve_to (cr, 206.191, 88.234, 206.113, 88, 206.129, 87.687);

  cairo_line_to (cr, 205.535, 87.687);
  cairo_curve_to (cr, 205.535, 88.141, 205.629, 88.437, 205.816, 88.703);

  cairo_curve_to (cr, 206.16, 89.172, 206.723, 89.406, 207.473, 89.406);

  cairo_curve_to (cr, 208.051, 89.406, 208.535, 89.266, 208.848, 89.031);

  cairo_curve_to (cr, 209.176, 88.766, 209.379, 88.328, 209.379, 87.906);

  cairo_curve_to (cr, 209.379, 87.297, 209.004, 86.859, 208.348, 86.672);

  cairo_line_to (cr, 207.113, 86.344);
  cairo_curve_to (cr, 206.52, 86.187, 206.301, 86, 206.301, 85.625);

  cairo_curve_to (cr, 206.301, 85.125, 206.738, 84.797, 207.395, 84.797);

  cairo_curve_to (cr, 208.176, 84.797, 208.613, 85.141, 208.629, 85.781);

  cairo_close_path (cr);
  cairo_move_to (cr, 209.707, 89.25);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_restore (cr);

  return cr;
} /* end of figure_rc_page_1_render() */

/* Total number of pages */
int figure_rc_total_pages;

/* Array of the individual page render functions */
cairo_t * (*figure_rc_render[1])(cairo_surface_t *, cairo_t *);

/* array of pointers to the widths and heights */
int figure_rc_width[1];
int figure_rc_height[1];

/* This function should be called at the beginning of the user program */
void figure_rc_init(void)
{

  figure_rc_total_pages = 1;

  figure_rc_render[0] = figure_rc_page_1_render;

  figure_rc_width[0] = figure_rc_page_1_width;
  figure_rc_height[0] = figure_rc_page_1_height;
}

float figure_rc_width_max = 310;
float figure_rc_height_max = 94;
