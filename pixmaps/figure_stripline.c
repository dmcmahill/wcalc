/* ***** Generated from pstoedit ***** */
#include <cairo.h>
#include <stdio.h>

/*
 * Original bounding box = for page # 1 is
 * LL: x: 103 y: 671 UR: x: 308 y: 765
 * The figure has been offset by (-103, 765)
 * to move LL to (0,0).  The width and height
 * can be read from the following two variables:
 */
static int figure_stripline_page_1_width = 205;
static int figure_stripline_page_1_height = 94;

static cairo_t * figure_stripline_page_1_render(cairo_surface_t *cs, cairo_t *cr)
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


  /*
   * Path # 1 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 5, 45);
  cairo_line_to (cr, 149, 45);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 2 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 3 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 149, 45);
  cairo_line_to (cr, 203, 9);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 4 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 104, 45);
  cairo_line_to (cr, 162.5, 9);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 148.5,9);

  /*
   * Path # 5 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 149, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 148.5,9);

  /*
   * Path # 6 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 149, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 7 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 54);
  cairo_line_to (cr, 149, 90);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 8 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 54);
  cairo_line_to (cr, 149, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 9 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 6 */
  cairo_move_to (cr, 0.5, 45);
  cairo_line_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 10 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 6 */
  cairo_move_to (cr, 0.5, 45);
  cairo_line_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 11 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 149, 45);
  cairo_line_to (cr, 149, 81);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 12 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 11.75, 49.5);
  cairo_line_to (cr, 11.75, 76.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 13 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 11.75, 45);
  cairo_line_to (cr, 13.438, 49.5);
  cairo_line_to (cr, 10.063, 49.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 14 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 11.75, 45);
  cairo_line_to (cr, 13.438, 49.5);
  cairo_line_to (cr, 10.063, 49.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 15 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 11.75, 81);
  cairo_line_to (cr, 10.063, 76.5);
  cairo_line_to (cr, 13.438, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 16 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 11.75, 81);
  cairo_line_to (cr, 10.063, 76.5);
  cairo_line_to (cr, 13.438, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 13 */
  cairo_move_to (cr, 8.984, 67.172);
  cairo_line_to (cr, 8.984, 70.875);
  cairo_line_to (cr, 10.031, 70.875);
  cairo_line_to (cr, 10.031, 62.734);
  cairo_line_to (cr, 8.984, 62.734);
  cairo_line_to (cr, 8.984, 66.25);
  cairo_line_to (cr, 4.734, 66.25);
  cairo_line_to (cr, 4.734, 62.734);
  cairo_line_to (cr, 3.688, 62.734);
  cairo_line_to (cr, 3.688, 70.875);
  cairo_line_to (cr, 4.734, 70.875);
  cairo_line_to (cr, 4.734, 67.172);
  cairo_close_path (cr);
  cairo_move_to (cr, 10.871, 70.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 18 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 167, 85.5);
  cairo_line_to (cr, 176, 85.5);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 162.5, 85.5);
  cairo_line_to (cr, 167, 83.812);
  cairo_line_to (cr, 167, 87.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 20 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 162.5, 85.5);
  cairo_line_to (cr, 167, 83.812);
  cairo_line_to (cr, 167, 87.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 283.5,671.625, 24.992,12.938);

  /*
   * Path # 21 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 180.5, 80.437);
  cairo_line_to (cr, 205.492, 80.437);
  cairo_line_to (cr, 205.492, 93.375);
  cairo_line_to (cr, 180.5, 93.375);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 22 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 42 */
  cairo_move_to (cr, 188.516, 86.828);
  cairo_line_to (cr, 185.078, 86.828);
  cairo_line_to (cr, 185.078, 87.75);
  cairo_line_to (cr, 187.594, 87.75);
  cairo_line_to (cr, 187.594, 87.969);
  cairo_curve_to (cr, 187.594, 89.422, 186.5, 90.469, 185, 90.469);

  cairo_curve_to (cr, 184.172, 90.469, 183.406, 90.172, 182.922, 89.641);

  cairo_curve_to (cr, 182.375, 89.062, 182.047, 88.094, 182.047, 87.094);

  cairo_curve_to (cr, 182.047, 85.094, 183.203, 83.766, 184.938, 83.766);

  cairo_curve_to (cr, 186.203, 83.766, 187.109, 84.406, 187.328, 85.453);

  cairo_line_to (cr, 188.406, 85.453);
  cairo_curve_to (cr, 188.109, 83.812, 186.844, 82.859, 184.953, 82.859);

  cairo_curve_to (cr, 183.953, 82.859, 183.141, 83.109, 182.484, 83.641);

  cairo_curve_to (cr, 181.531, 84.422, 181, 85.687, 181, 87.141);

  cairo_curve_to (cr, 181, 89.641, 182.547, 91.375, 184.781, 91.375);

  cairo_curve_to (cr, 185.891, 91.375, 186.781, 90.969, 187.594, 90.094);

  cairo_line_to (cr, 187.844, 91.172);
  cairo_line_to (cr, 188.516, 91.172);
  cairo_close_path (cr);
  cairo_move_to (cr, 196.563, 82.984);
  cairo_line_to (cr, 195.563, 82.984);
  cairo_line_to (cr, 195.563, 89.641);
  cairo_line_to (cr, 191.25, 82.984);
  cairo_line_to (cr, 190.109, 82.984);
  cairo_line_to (cr, 190.109, 91.125);
  cairo_line_to (cr, 191.109, 91.125);
  cairo_line_to (cr, 191.109, 84.531);
  cairo_line_to (cr, 195.375, 91.125);
  cairo_line_to (cr, 196.563, 91.125);
  cairo_close_path (cr);
  cairo_move_to (cr, 198.371, 91.125);
  cairo_line_to (cr, 201.559, 91.125);
  cairo_curve_to (cr, 203.637, 91.125, 204.918, 89.578, 204.918, 87.047);

  cairo_curve_to (cr, 204.918, 84.531, 203.652, 82.984, 201.559, 82.984);

  cairo_line_to (cr, 198.371, 82.984);
  cairo_close_path (cr);
  cairo_move_to (cr, 199.434, 90.203);
  cairo_line_to (cr, 199.434, 83.906);
  cairo_line_to (cr, 201.371, 83.906);
  cairo_curve_to (cr, 202.996, 83.906, 203.855, 84.984, 203.855, 87.062);

  cairo_curve_to (cr, 203.855, 89.125, 202.996, 90.203, 201.371, 90.203);

  cairo_close_path (cr);
  cairo_move_to (cr, 205.492, 91.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 166.5,693, 27,9);

  /*
   * Path # 23 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 63.5, 63);
  cairo_line_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 63.5, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 166.5,693, 27,9);

  /*
   * Path # 24 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 63.5, 63);
  cairo_line_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 63.5, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 25 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 27);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 26 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 27);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 27 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 149, 27);
  cairo_line_to (cr, 122, 27);
  cairo_line_to (cr, 63.5, 63);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 149, 27);
  cairo_line_to (cr, 122, 27);
  cairo_line_to (cr, 63.5, 63);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 52.25, 72);
  cairo_line_to (cr, 61.25, 72);
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
  cairo_move_to (cr, 56.75, 58.5);
  cairo_line_to (cr, 56.75, 56.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 31 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 56.75, 63);
  cairo_line_to (cr, 55.063, 58.5);
  cairo_line_to (cr, 58.438, 58.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 32 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 56.75, 63);
  cairo_line_to (cr, 55.063, 58.5);
  cairo_line_to (cr, 58.438, 58.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  cairo_move_to (cr, 56.75, 76.5);
  cairo_line_to (cr, 56.75, 78.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 34 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 56.75, 72);
  cairo_line_to (cr, 58.438, 76.5);
  cairo_line_to (cr, 55.063, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 35 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 56.75, 72);
  cairo_line_to (cr, 58.438, 76.5);
  cairo_line_to (cr, 55.063, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 36 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 52.25, 63);
  cairo_line_to (cr, 61.25, 63);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 37 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 61 */
  cairo_move_to (cr, 29.25, 63.656);
  cairo_line_to (cr, 31.953, 63.656);
  cairo_line_to (cr, 31.953, 62.734);
  cairo_line_to (cr, 25.484, 62.734);
  cairo_line_to (cr, 25.484, 63.656);
  cairo_line_to (cr, 28.203, 63.656);
  cairo_line_to (cr, 28.203, 70.875);
  cairo_line_to (cr, 29.25, 70.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 32.918, 65.031);
  cairo_line_to (cr, 32.918, 70.875);
  cairo_line_to (cr, 33.855, 70.875);
  cairo_line_to (cr, 33.855, 67.203);
  cairo_curve_to (cr, 33.855, 66.359, 34.48, 65.672, 35.246, 65.672);

  cairo_curve_to (cr, 35.949, 65.672, 36.355, 66.094, 36.355, 66.844);

  cairo_line_to (cr, 36.355, 70.875);
  cairo_line_to (cr, 37.293, 70.875);
  cairo_line_to (cr, 37.293, 67.203);
  cairo_curve_to (cr, 37.293, 66.359, 37.918, 65.672, 38.699, 65.672);

  cairo_curve_to (cr, 39.387, 65.672, 39.793, 66.109, 39.793, 66.844);

  cairo_line_to (cr, 39.793, 70.875);
  cairo_line_to (cr, 40.746, 70.875);
  cairo_line_to (cr, 40.746, 66.484);
  cairo_curve_to (cr, 40.746, 65.437, 40.121, 64.859, 39.027, 64.859);

  cairo_curve_to (cr, 38.23, 64.859, 37.746, 65.094, 37.199, 65.75);

  cairo_curve_to (cr, 36.855, 65.125, 36.371, 64.859, 35.605, 64.859);

  cairo_curve_to (cr, 34.809, 64.859, 34.277, 65.156, 33.777, 65.859);

  cairo_line_to (cr, 33.777, 65.031);
  cairo_close_path (cr);
  cairo_move_to (cr, 47.289, 68.266);
  cairo_curve_to (cr, 47.289, 67.375, 47.227, 66.844, 47.055, 66.406);

  cairo_curve_to (cr, 46.664, 65.437, 45.773, 64.859, 44.664, 64.859);

  cairo_curve_to (cr, 43.008, 64.859, 41.945, 66.094, 41.945, 68.031);

  cairo_curve_to (cr, 41.945, 69.953, 42.961, 71.125, 44.633, 71.125);

  cairo_curve_to (cr, 45.992, 71.125, 46.93, 70.375, 47.164, 69.094);

  cairo_line_to (cr, 46.227, 69.094);
  cairo_curve_to (cr, 45.961, 69.875, 45.43, 70.266, 44.664, 70.266);

  cairo_curve_to (cr, 44.07, 70.266, 43.555, 70, 43.242, 69.516);

  cairo_curve_to (cr, 43.023, 69.172, 42.945, 68.844, 42.93, 68.266);

  cairo_close_path (cr);
  cairo_move_to (cr, 42.945, 67.5);
  cairo_curve_to (cr, 43.023, 66.422, 43.695, 65.719, 44.648, 65.719);

  cairo_curve_to (cr, 45.602, 65.719, 46.273, 66.453, 46.273, 67.5);

  cairo_close_path (cr);
  cairo_move_to (cr, 50.621, 65.031);
  cairo_line_to (cr, 49.652, 65.031);
  cairo_line_to (cr, 49.652, 63.422);
  cairo_line_to (cr, 48.715, 63.422);
  cairo_line_to (cr, 48.715, 65.031);
  cairo_line_to (cr, 47.902, 65.031);
  cairo_line_to (cr, 47.902, 65.781);
  cairo_line_to (cr, 48.715, 65.781);
  cairo_line_to (cr, 48.715, 70.203);
  cairo_curve_to (cr, 48.715, 70.812, 49.121, 71.125, 49.855, 71.125);

  cairo_curve_to (cr, 50.105, 71.125, 50.309, 71.109, 50.621, 71.047);

  cairo_line_to (cr, 50.621, 70.266);
  cairo_curve_to (cr, 50.48, 70.312, 50.355, 70.312, 50.168, 70.312);

  cairo_curve_to (cr, 49.762, 70.312, 49.652, 70.203, 49.652, 69.797);

  cairo_line_to (cr, 49.652, 65.781);
  cairo_line_to (cr, 50.621, 65.781);
  cairo_close_path (cr);
  cairo_move_to (cr, 50.871, 70.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 38 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 42 */
  cairo_move_to (cr, 184.016, 32.828);
  cairo_line_to (cr, 180.578, 32.828);
  cairo_line_to (cr, 180.578, 33.75);
  cairo_line_to (cr, 183.094, 33.75);
  cairo_line_to (cr, 183.094, 33.969);
  cairo_curve_to (cr, 183.094, 35.422, 182, 36.469, 180.5, 36.469);

  cairo_curve_to (cr, 179.672, 36.469, 178.906, 36.172, 178.422, 35.641);

  cairo_curve_to (cr, 177.875, 35.062, 177.547, 34.094, 177.547, 33.094);

  cairo_curve_to (cr, 177.547, 31.094, 178.703, 29.766, 180.438, 29.766);

  cairo_curve_to (cr, 181.703, 29.766, 182.609, 30.406, 182.828, 31.453);

  cairo_line_to (cr, 183.906, 31.453);
  cairo_curve_to (cr, 183.609, 29.812, 182.344, 28.859, 180.453, 28.859);

  cairo_curve_to (cr, 179.453, 28.859, 178.641, 29.109, 177.984, 29.641);

  cairo_curve_to (cr, 177.031, 30.422, 176.5, 31.687, 176.5, 33.141);

  cairo_curve_to (cr, 176.5, 35.641, 178.047, 37.375, 180.281, 37.375);

  cairo_curve_to (cr, 181.391, 37.375, 182.281, 36.969, 183.094, 36.094);

  cairo_line_to (cr, 183.344, 37.172);
  cairo_line_to (cr, 184.016, 37.172);
  cairo_close_path (cr);
  cairo_move_to (cr, 192.063, 28.984);
  cairo_line_to (cr, 191.063, 28.984);
  cairo_line_to (cr, 191.063, 35.641);
  cairo_line_to (cr, 186.75, 28.984);
  cairo_line_to (cr, 185.609, 28.984);
  cairo_line_to (cr, 185.609, 37.125);
  cairo_line_to (cr, 186.609, 37.125);
  cairo_line_to (cr, 186.609, 30.531);
  cairo_line_to (cr, 190.875, 37.125);
  cairo_line_to (cr, 192.063, 37.125);
  cairo_close_path (cr);
  cairo_move_to (cr, 193.871, 37.125);
  cairo_line_to (cr, 197.059, 37.125);
  cairo_curve_to (cr, 199.137, 37.125, 200.418, 35.578, 200.418, 33.047);

  cairo_curve_to (cr, 200.418, 30.531, 199.152, 28.984, 197.059, 28.984);

  cairo_line_to (cr, 193.871, 28.984);
  cairo_close_path (cr);
  cairo_move_to (cr, 194.934, 36.203);
  cairo_line_to (cr, 194.934, 29.906);
  cairo_line_to (cr, 196.871, 29.906);
  cairo_curve_to (cr, 198.496, 29.906, 199.355, 30.984, 199.355, 33.062);

  cairo_curve_to (cr, 199.355, 35.125, 198.496, 36.203, 196.871, 36.203);

  cairo_close_path (cr);
  cairo_move_to (cr, 200.992, 37.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
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
  /* Path Elements 0 to 27 */
  cairo_move_to (cr, 119.813, 71.734);
  cairo_curve_to (cr, 119.563, 72.437, 119.156, 72.719, 118.344, 72.719);

  cairo_curve_to (cr, 117.797, 72.719, 117.344, 72.578, 117.031, 72.312);

  cairo_curve_to (cr, 116.813, 72.109, 116.719, 71.891, 116.719, 71.531);

  cairo_curve_to (cr, 116.719, 70.875, 117.109, 70.453, 117.703, 70.453);

  cairo_curve_to (cr, 117.813, 70.453, 117.906, 70.453, 118.016, 70.484);

  cairo_curve_to (cr, 118.188, 70.531, 118.297, 70.531, 118.422, 70.531);

  cairo_curve_to (cr, 118.75, 70.531, 119, 70.375, 119, 70.172);

  cairo_curve_to (cr, 119, 70, 118.875, 69.906, 118.609, 69.906);

  cairo_curve_to (cr, 118.484, 69.906, 118.484, 69.906, 118.156, 69.953);

  cairo_line_to (cr, 118.094, 69.953);
  cairo_curve_to (cr, 117.781, 69.984, 117.75, 69.984, 117.656, 69.984);

  cairo_curve_to (cr, 117.078, 69.984, 116.703, 69.594, 116.703, 68.984);

  cairo_curve_to (cr, 116.703, 68.312, 117.188, 67.797, 117.766, 67.797);

  cairo_curve_to (cr, 118.172, 67.797, 118.438, 67.969, 118.563, 68.328);

  cairo_line_to (cr, 118.594, 68.406);
  cairo_curve_to (cr, 118.719, 68.812, 118.922, 68.984, 119.203, 68.984);

  cairo_curve_to (cr, 119.516, 68.984, 119.75, 68.734, 119.75, 68.422);

  cairo_curve_to (cr, 119.75, 67.844, 119, 67.406, 117.938, 67.406);

  cairo_curve_to (cr, 117.203, 67.406, 116.703, 67.562, 116.25, 67.906);

  cairo_curve_to (cr, 115.844, 68.219, 115.656, 68.547, 115.656, 68.969);

  cairo_curve_to (cr, 115.656, 69.344, 115.828, 69.687, 116.109, 69.891);

  cairo_curve_to (cr, 116.266, 69.984, 116.375, 70.047, 116.719, 70.219);

  cairo_curve_to (cr, 115.891, 70.484, 115.5, 70.953, 115.5, 71.625);

  cairo_curve_to (cr, 115.5, 72.578, 116.5, 73.266, 117.891, 73.266);

  cairo_curve_to (cr, 119.156, 73.266, 119.859, 72.812, 120.078, 71.828);

  cairo_close_path (cr);
  cairo_move_to (cr, 120.188, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 40 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 120.656, 71.844);
  cairo_line_to (cr, 120.656, 75.375);
  cairo_line_to (cr, 121.219, 75.375);
  cairo_line_to (cr, 121.219, 73.547);
  cairo_curve_to (cr, 121.219, 72.703, 121.578, 72.328, 122.344, 72.344);

  cairo_line_to (cr, 122.344, 71.766);
  cairo_curve_to (cr, 122.25, 71.75, 122.203, 71.75, 122.125, 71.75);

  cairo_curve_to (cr, 121.766, 71.75, 121.484, 71.969, 121.172, 72.484);

  cairo_line_to (cr, 121.172, 71.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 122.434, 75.375);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
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
  /* Path Elements 0 to 65 */
  cairo_move_to (cr, 123.418, 71.969);
  cairo_line_to (cr, 123.418, 73.125);
  cairo_line_to (cr, 124.09, 73.125);
  cairo_line_to (cr, 124.09, 73.328);
  cairo_curve_to (cr, 124.09, 74.094, 123.949, 74.312, 123.418, 74.344);

  cairo_line_to (cr, 123.418, 74.766);
  cairo_curve_to (cr, 124.168, 74.781, 124.605, 74.25, 124.605, 73.297);

  cairo_line_to (cr, 124.605, 71.969);
  cairo_close_path (cr);
  cairo_move_to (cr, 128.434, 67.281);
  cairo_line_to (cr, 127.465, 67.281);
  cairo_line_to (cr, 127.465, 65.672);
  cairo_line_to (cr, 126.527, 65.672);
  cairo_line_to (cr, 126.527, 67.281);
  cairo_line_to (cr, 125.715, 67.281);
  cairo_line_to (cr, 125.715, 68.031);
  cairo_line_to (cr, 126.527, 68.031);
  cairo_line_to (cr, 126.527, 72.453);
  cairo_curve_to (cr, 126.527, 73.062, 126.934, 73.375, 127.668, 73.375);

  cairo_curve_to (cr, 127.918, 73.375, 128.121, 73.359, 128.434, 73.297);

  cairo_line_to (cr, 128.434, 72.516);
  cairo_curve_to (cr, 128.293, 72.562, 128.168, 72.562, 127.98, 72.562);

  cairo_curve_to (cr, 127.574, 72.562, 127.465, 72.453, 127.465, 72.047);

  cairo_line_to (cr, 127.465, 68.031);
  cairo_line_to (cr, 128.434, 68.031);
  cairo_close_path (cr);
  cairo_move_to (cr, 134.73, 72.578);
  cairo_curve_to (cr, 134.637, 72.594, 134.59, 72.594, 134.527, 72.594);

  cairo_curve_to (cr, 134.199, 72.594, 134.027, 72.437, 134.027, 72.141);

  cairo_line_to (cr, 134.027, 68.703);
  cairo_curve_to (cr, 134.027, 67.672, 133.246, 67.109, 131.793, 67.109);

  cairo_curve_to (cr, 130.918, 67.109, 130.23, 67.359, 129.824, 67.797);

  cairo_curve_to (cr, 129.559, 68.094, 129.434, 68.422, 129.418, 69.016);

  cairo_line_to (cr, 130.371, 69.016);
  cairo_curve_to (cr, 130.449, 68.297, 130.871, 67.969, 131.762, 67.969);

  cairo_curve_to (cr, 132.621, 67.969, 133.09, 68.281, 133.09, 68.844);

  cairo_line_to (cr, 133.09, 69.094);
  cairo_curve_to (cr, 133.074, 69.484, 132.871, 69.641, 132.105, 69.734);

  cairo_curve_to (cr, 130.762, 69.906, 130.559, 69.953, 130.199, 70.094);

  cairo_curve_to (cr, 129.512, 70.375, 129.152, 70.891, 129.152, 71.656);

  cairo_curve_to (cr, 129.152, 72.719, 129.902, 73.375, 131.105, 73.375);

  cairo_curve_to (cr, 131.855, 73.375, 132.449, 73.125, 133.121, 72.516);

  cairo_curve_to (cr, 133.184, 73.125, 133.48, 73.375, 134.09, 73.375);

  cairo_curve_to (cr, 134.293, 73.375, 134.418, 73.359, 134.73, 73.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 133.09, 71.281);
  cairo_curve_to (cr, 133.09, 71.594, 132.996, 71.781, 132.715, 72.047);

  cairo_curve_to (cr, 132.324, 72.391, 131.855, 72.562, 131.309, 72.562);

  cairo_curve_to (cr, 130.574, 72.562, 130.137, 72.219, 130.137, 71.625);

  cairo_curve_to (cr, 130.137, 71.016, 130.543, 70.703, 131.574, 70.562);

  cairo_curve_to (cr, 132.574, 70.422, 132.762, 70.375, 133.09, 70.234);

  cairo_close_path (cr);
  cairo_move_to (cr, 135.734, 67.281);
  cairo_line_to (cr, 135.734, 73.125);
  cairo_line_to (cr, 136.672, 73.125);
  cairo_line_to (cr, 136.672, 69.906);
  cairo_curve_to (cr, 136.672, 68.703, 137.313, 67.922, 138.281, 67.922);

  cairo_curve_to (cr, 139.031, 67.922, 139.5, 68.375, 139.5, 69.078);

  cairo_line_to (cr, 139.5, 73.125);
  cairo_line_to (cr, 140.438, 73.125);
  cairo_line_to (cr, 140.438, 68.703);
  cairo_curve_to (cr, 140.438, 67.734, 139.703, 67.109, 138.563, 67.109);

  cairo_curve_to (cr, 137.688, 67.109, 137.125, 67.453, 136.594, 68.266);

  cairo_line_to (cr, 136.594, 67.281);
  cairo_close_path (cr);
  cairo_move_to (cr, 141.191, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 42 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 23 */
  cairo_move_to (cr, 144.082, 68.172);
  cairo_curve_to (cr, 143.504, 68.344, 143.145, 68.5, 142.816, 68.719);

  cairo_curve_to (cr, 142.066, 69.219, 141.645, 70, 141.645, 70.844);

  cairo_curve_to (cr, 141.645, 72.172, 142.723, 73.266, 144.02, 73.266);

  cairo_curve_to (cr, 144.707, 73.266, 145.301, 73, 145.848, 72.437);

  cairo_curve_to (cr, 146.379, 71.891, 146.629, 71.281, 146.629, 70.531);

  cairo_curve_to (cr, 146.629, 69.812, 146.379, 69.125, 145.926, 68.578);

  cairo_curve_to (cr, 145.535, 68.125, 145.27, 67.922, 144.316, 67.375);

  cairo_curve_to (cr, 143.301, 66.766, 142.863, 66.359, 142.863, 65.984);

  cairo_curve_to (cr, 142.863, 65.625, 143.223, 65.344, 143.676, 65.344);

  cairo_curve_to (cr, 144.035, 65.344, 144.332, 65.469, 144.613, 65.766);

  cairo_curve_to (cr, 145.238, 66.375, 145.348, 66.437, 145.645, 66.437);

  cairo_curve_to (cr, 145.957, 66.437, 146.176, 66.234, 146.176, 65.969);

  cairo_curve_to (cr, 146.176, 65.359, 145.191, 64.875, 144.004, 64.875);

  cairo_curve_to (cr, 142.941, 64.875, 142.301, 65.281, 142.301, 65.969);

  cairo_curve_to (cr, 142.301, 66.391, 142.488, 66.703, 142.973, 67.156);

  cairo_close_path (cr);
  cairo_move_to (cr, 144.41, 68.469);
  cairo_curve_to (cr, 145.176, 69.172, 145.52, 69.953, 145.52, 70.906);

  cairo_curve_to (cr, 145.52, 72.016, 144.988, 72.75, 144.207, 72.75);

  cairo_curve_to (cr, 143.41, 72.75, 142.863, 71.875, 142.863, 70.594);

  cairo_curve_to (cr, 142.863, 69.391, 143.379, 68.687, 144.41, 68.469);

  cairo_close_path (cr);
  cairo_move_to (cr, 146.746, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 166.5,693, 27,9);

  /*
   * Path # 43 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 63.5, 63);
  cairo_line_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 63.5, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 44 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 90.5, 72);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 27);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 45 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 90.5, 63);
  cairo_line_to (cr, 149, 27);
  cairo_line_to (cr, 122, 27);
  cairo_line_to (cr, 63.5, 63);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 46 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 6 */
  cairo_move_to (cr, 0.5, 45);
  cairo_line_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 148.5,9);

  /*
   * Path # 47 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_line_to (cr, 149, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 48 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 81);
  cairo_line_to (cr, 203, 45);
  cairo_line_to (cr, 203, 54);
  cairo_line_to (cr, 149, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 148.5,9);

  /*
   * Path # 49 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 45);
  cairo_line_to (cr, 0.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 148.5,9);

  /*
   * Path # 50 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 45);
  cairo_line_to (cr, 0.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 51 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 36);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 52 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 36);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 53 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 149, 36);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 54 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 149, 36);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 55 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 149, 36);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 148.5,9);

  /*
   * Path # 56 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 149, 36);
  cairo_line_to (cr, 149, 45);
  cairo_line_to (cr, 0.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 57 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 149, 36);
  cairo_line_to (cr, 203, 0);
  cairo_line_to (cr, 203, 9);
  cairo_line_to (cr, 149, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 58 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 167, 31.5);
  cairo_line_to (cr, 176, 31.5);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 162.5, 31.5);
  cairo_line_to (cr, 167, 29.812);
  cairo_line_to (cr, 167, 33.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 60 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 162.5, 31.5);
  cairo_line_to (cr, 167, 29.812);
  cairo_line_to (cr, 167, 33.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 61 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 90.5, 24.75);
  cairo_line_to (cr, 90.5, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 62 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 63.5, 24.75);
  cairo_line_to (cr, 63.5, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 63 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 32, 36);
  cairo_line_to (cr, 90.5, 0);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 64 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 26.832, 33.641);
  cairo_line_to (cr, 77.668, 2.35901);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 23, 36);
  cairo_line_to (cr, 25.949, 32.203);
  cairo_line_to (cr, 27.719, 35.078);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 66 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 23, 36);
  cairo_line_to (cr, 25.949, 32.203);
  cairo_line_to (cr, 27.719, 35.078);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 81.5, 0);
  cairo_line_to (cr, 78.551, 3.797);
  cairo_line_to (cr, 76.781, 0.921997);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 68 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 81.5, 0);
  cairo_line_to (cr, 78.551, 3.797);
  cairo_line_to (cr, 76.781, 0.921997);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 69 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 27.5, 36);
  cairo_line_to (cr, 18.5, 36);
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
  /* Path Elements 0 to 7 */
  cairo_move_to (cr, 38.453, 15.484);
  cairo_line_to (cr, 37.406, 15.484);
  cairo_line_to (cr, 37.406, 23.625);
  cairo_line_to (cr, 42.531, 23.625);
  cairo_line_to (cr, 42.531, 22.703);
  cairo_line_to (cr, 38.453, 22.703);
  cairo_close_path (cr);
  cairo_move_to (cr, 42.754, 23.625);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 71 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 86, 0);
  cairo_line_to (cr, 72.5, 0);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 72 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 68, 29.25);
  cairo_line_to (cr, 86, 29.25);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 63.5, 29.25);
  cairo_line_to (cr, 68, 27.562);
  cairo_line_to (cr, 68, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 74 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 63.5, 29.25);
  cairo_line_to (cr, 68, 27.562);
  cairo_line_to (cr, 68, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 90.5, 29.25);
  cairo_line_to (cr, 86, 30.937);
  cairo_line_to (cr, 86, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 76 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 90.5, 29.25);
  cairo_line_to (cr, 86, 30.937);
  cairo_line_to (cr, 86, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 14 */
  cairo_move_to (cr, 80.906, 25.875);
  cairo_line_to (cr, 83, 17.734);
  cairo_line_to (cr, 81.828, 17.734);
  cairo_line_to (cr, 80.313, 24.344);
  cairo_line_to (cr, 78.438, 17.734);
  cairo_line_to (cr, 77.313, 17.734);
  cairo_line_to (cr, 75.469, 24.344);
  cairo_line_to (cr, 73.922, 17.734);
  cairo_line_to (cr, 72.75, 17.734);
  cairo_line_to (cr, 74.859, 25.875);
  cairo_line_to (cr, 76.016, 25.875);
  cairo_line_to (cr, 77.859, 19.187);
  cairo_line_to (cr, 79.766, 25.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 83.117, 25.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_restore (cr);

  return cr;
} /* end of figure_stripline_page_1_render() */

/* Total number of pages */
int figure_stripline_total_pages;

/* Array of the individual page render functions */
cairo_t * (*figure_stripline_render[1])(cairo_surface_t *, cairo_t *);

/* array of pointers to the widths and heights */
int figure_stripline_width[1];
int figure_stripline_height[1];

/* This function should be called at the beginning of the user program */
void figure_stripline_init(void)
{

  figure_stripline_total_pages = 1;

  figure_stripline_render[0] = figure_stripline_page_1_render;

  figure_stripline_width[0] = figure_stripline_page_1_width;
  figure_stripline_height[0] = figure_stripline_page_1_height;
}

float figure_stripline_width_max = 205;
float figure_stripline_height_max = 94;
