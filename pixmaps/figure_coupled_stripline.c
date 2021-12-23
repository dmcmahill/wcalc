/* ***** Generated from pstoedit ***** */
#include <cairo.h>
#include <stdio.h>

/*
 * Original bounding box = for page # 1 is
 * LL: x: 103 y: 671 UR: x: 346 y: 765
 * The figure has been offset by (-103, 765)
 * to move LL to (0,0).  The width and height
 * can be read from the following two variables:
 */
static int figure_coupled_stripline_page_1_width = 243;
static int figure_coupled_stripline_page_1_height = 94;

static cairo_t * figure_coupled_stripline_page_1_render(cairo_surface_t *cs, cairo_t *cr)
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
  cairo_move_to (cr, 5, 81);
  cairo_line_to (cr, 149, 81);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 2 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 6 */
  cairo_move_to (cr, 0.5, 45);
  cairo_line_to (cr, 0.5, 81);
  cairo_line_to (cr, 189.5, 81);
  cairo_line_to (cr, 243.5, 45);
  cairo_line_to (cr, 243.5, 9);
  cairo_line_to (cr, 189.5, 45);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 3 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 6 */
  cairo_move_to (cr, 0.5, 45);
  cairo_line_to (cr, 0.5, 81);
  cairo_line_to (cr, 189.5, 81);
  cairo_line_to (cr, 243.5, 45);
  cairo_line_to (cr, 243.5, 9);
  cairo_line_to (cr, 189.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
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
  cairo_move_to (cr, 189.5, 45);
  cairo_line_to (cr, 189.5, 81);
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
  cairo_move_to (cr, 11.75, 49.5);
  cairo_line_to (cr, 11.75, 76.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 6 (polygon):
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
   * Path # 7 (polygon):
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
   * Path # 8 (polygon):
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
   * Path # 9 (polygon):
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
   * Path # 10 (polygon):
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
   * Path # 11 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 207.5, 85.5);
  cairo_line_to (cr, 216.5, 85.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 12 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 203, 85.5);
  cairo_line_to (cr, 207.5, 83.812);
  cairo_line_to (cr, 207.5, 87.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
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
  cairo_move_to (cr, 203, 85.5);
  cairo_line_to (cr, 207.5, 83.812);
  cairo_line_to (cr, 207.5, 87.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 319.5,671.625, 24.992,12.938);

  /*
   * Path # 14 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 216.5, 80.437);
  cairo_line_to (cr, 241.492, 80.437);
  cairo_line_to (cr, 241.492, 93.375);
  cairo_line_to (cr, 216.5, 93.375);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 15 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 42 */
  cairo_move_to (cr, 224.516, 86.828);
  cairo_line_to (cr, 221.078, 86.828);
  cairo_line_to (cr, 221.078, 87.75);
  cairo_line_to (cr, 223.594, 87.75);
  cairo_line_to (cr, 223.594, 87.969);
  cairo_curve_to (cr, 223.594, 89.422, 222.5, 90.469, 221, 90.469);

  cairo_curve_to (cr, 220.172, 90.469, 219.406, 90.172, 218.922, 89.641);

  cairo_curve_to (cr, 218.375, 89.062, 218.047, 88.094, 218.047, 87.094);

  cairo_curve_to (cr, 218.047, 85.094, 219.203, 83.766, 220.938, 83.766);

  cairo_curve_to (cr, 222.203, 83.766, 223.109, 84.406, 223.328, 85.453);

  cairo_line_to (cr, 224.406, 85.453);
  cairo_curve_to (cr, 224.109, 83.812, 222.844, 82.859, 220.953, 82.859);

  cairo_curve_to (cr, 219.953, 82.859, 219.141, 83.109, 218.484, 83.641);

  cairo_curve_to (cr, 217.531, 84.422, 217, 85.687, 217, 87.141);

  cairo_curve_to (cr, 217, 89.641, 218.547, 91.375, 220.781, 91.375);

  cairo_curve_to (cr, 221.891, 91.375, 222.781, 90.969, 223.594, 90.094);

  cairo_line_to (cr, 223.844, 91.172);
  cairo_line_to (cr, 224.516, 91.172);
  cairo_close_path (cr);
  cairo_move_to (cr, 232.563, 82.984);
  cairo_line_to (cr, 231.563, 82.984);
  cairo_line_to (cr, 231.563, 89.641);
  cairo_line_to (cr, 227.25, 82.984);
  cairo_line_to (cr, 226.109, 82.984);
  cairo_line_to (cr, 226.109, 91.125);
  cairo_line_to (cr, 227.109, 91.125);
  cairo_line_to (cr, 227.109, 84.531);
  cairo_line_to (cr, 231.375, 91.125);
  cairo_line_to (cr, 232.563, 91.125);
  cairo_close_path (cr);
  cairo_move_to (cr, 234.371, 91.125);
  cairo_line_to (cr, 237.559, 91.125);
  cairo_curve_to (cr, 239.637, 91.125, 240.918, 89.578, 240.918, 87.047);

  cairo_curve_to (cr, 240.918, 84.531, 239.652, 82.984, 237.559, 82.984);

  cairo_line_to (cr, 234.371, 82.984);
  cairo_close_path (cr);
  cairo_move_to (cr, 235.434, 90.203);
  cairo_line_to (cr, 235.434, 83.906);
  cairo_line_to (cr, 237.371, 83.906);
  cairo_curve_to (cr, 238.996, 83.906, 239.855, 84.984, 239.855, 87.062);

  cairo_curve_to (cr, 239.855, 89.125, 238.996, 90.203, 237.371, 90.203);

  cairo_close_path (cr);
  cairo_move_to (cr, 241.492, 91.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 162,693, 27,9);

  /*
   * Path # 16 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 59, 63);
  cairo_line_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 59, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 162,693, 27,9);

  /*
   * Path # 17 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 59, 63);
  cairo_line_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 59, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 18 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 144.5, 36);
  cairo_line_to (cr, 144.5, 27);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 19 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 144.5, 36);
  cairo_line_to (cr, 144.5, 27);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 20 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 144.5, 27);
  cairo_line_to (cr, 117.5, 27);
  cairo_line_to (cr, 59, 63);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 21 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 144.5, 27);
  cairo_line_to (cr, 117.5, 27);
  cairo_line_to (cr, 59, 63);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 47.75, 72);
  cairo_line_to (cr, 56.75, 72);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 23 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 52.25, 58.5);
  cairo_line_to (cr, 52.25, 56.25);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 24 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 52.25, 63);
  cairo_line_to (cr, 50.563, 58.5);
  cairo_line_to (cr, 53.938, 58.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 25 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 52.25, 63);
  cairo_line_to (cr, 50.563, 58.5);
  cairo_line_to (cr, 53.938, 58.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  cairo_move_to (cr, 52.25, 76.5);
  cairo_line_to (cr, 52.25, 78.75);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 52.25, 72);
  cairo_line_to (cr, 53.938, 76.5);
  cairo_line_to (cr, 50.563, 76.5);
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
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 52.25, 72);
  cairo_line_to (cr, 53.938, 76.5);
  cairo_line_to (cr, 50.563, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  cairo_move_to (cr, 47.75, 63);
  cairo_line_to (cr, 56.75, 63);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 30 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 61 */
  cairo_move_to (cr, 24.75, 63.656);
  cairo_line_to (cr, 27.453, 63.656);
  cairo_line_to (cr, 27.453, 62.734);
  cairo_line_to (cr, 20.984, 62.734);
  cairo_line_to (cr, 20.984, 63.656);
  cairo_line_to (cr, 23.703, 63.656);
  cairo_line_to (cr, 23.703, 70.875);
  cairo_line_to (cr, 24.75, 70.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 28.418, 65.031);
  cairo_line_to (cr, 28.418, 70.875);
  cairo_line_to (cr, 29.355, 70.875);
  cairo_line_to (cr, 29.355, 67.203);
  cairo_curve_to (cr, 29.355, 66.359, 29.98, 65.672, 30.746, 65.672);

  cairo_curve_to (cr, 31.449, 65.672, 31.855, 66.094, 31.855, 66.844);

  cairo_line_to (cr, 31.855, 70.875);
  cairo_line_to (cr, 32.793, 70.875);
  cairo_line_to (cr, 32.793, 67.203);
  cairo_curve_to (cr, 32.793, 66.359, 33.418, 65.672, 34.199, 65.672);

  cairo_curve_to (cr, 34.887, 65.672, 35.293, 66.109, 35.293, 66.844);

  cairo_line_to (cr, 35.293, 70.875);
  cairo_line_to (cr, 36.246, 70.875);
  cairo_line_to (cr, 36.246, 66.484);
  cairo_curve_to (cr, 36.246, 65.437, 35.621, 64.859, 34.527, 64.859);

  cairo_curve_to (cr, 33.73, 64.859, 33.246, 65.094, 32.699, 65.75);

  cairo_curve_to (cr, 32.355, 65.125, 31.871, 64.859, 31.105, 64.859);

  cairo_curve_to (cr, 30.309, 64.859, 29.777, 65.156, 29.277, 65.859);

  cairo_line_to (cr, 29.277, 65.031);
  cairo_close_path (cr);
  cairo_move_to (cr, 42.789, 68.266);
  cairo_curve_to (cr, 42.789, 67.375, 42.727, 66.844, 42.555, 66.406);

  cairo_curve_to (cr, 42.164, 65.437, 41.273, 64.859, 40.164, 64.859);

  cairo_curve_to (cr, 38.508, 64.859, 37.445, 66.094, 37.445, 68.031);

  cairo_curve_to (cr, 37.445, 69.953, 38.461, 71.125, 40.133, 71.125);

  cairo_curve_to (cr, 41.492, 71.125, 42.43, 70.375, 42.664, 69.094);

  cairo_line_to (cr, 41.727, 69.094);
  cairo_curve_to (cr, 41.461, 69.875, 40.93, 70.266, 40.164, 70.266);

  cairo_curve_to (cr, 39.57, 70.266, 39.055, 70, 38.742, 69.516);

  cairo_curve_to (cr, 38.523, 69.172, 38.445, 68.844, 38.43, 68.266);

  cairo_close_path (cr);
  cairo_move_to (cr, 38.445, 67.5);
  cairo_curve_to (cr, 38.523, 66.422, 39.195, 65.719, 40.148, 65.719);

  cairo_curve_to (cr, 41.102, 65.719, 41.773, 66.453, 41.773, 67.5);

  cairo_close_path (cr);
  cairo_move_to (cr, 46.121, 65.031);
  cairo_line_to (cr, 45.152, 65.031);
  cairo_line_to (cr, 45.152, 63.422);
  cairo_line_to (cr, 44.215, 63.422);
  cairo_line_to (cr, 44.215, 65.031);
  cairo_line_to (cr, 43.402, 65.031);
  cairo_line_to (cr, 43.402, 65.781);
  cairo_line_to (cr, 44.215, 65.781);
  cairo_line_to (cr, 44.215, 70.203);
  cairo_curve_to (cr, 44.215, 70.812, 44.621, 71.125, 45.355, 71.125);

  cairo_curve_to (cr, 45.605, 71.125, 45.809, 71.109, 46.121, 71.047);

  cairo_line_to (cr, 46.121, 70.266);
  cairo_curve_to (cr, 45.98, 70.312, 45.855, 70.312, 45.668, 70.312);

  cairo_curve_to (cr, 45.262, 70.312, 45.152, 70.203, 45.152, 69.797);

  cairo_line_to (cr, 45.152, 65.781);
  cairo_line_to (cr, 46.121, 65.781);
  cairo_close_path (cr);
  cairo_move_to (cr, 46.371, 70.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
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
  /* Path Elements 0 to 27 */
  cairo_move_to (cr, 160.313, 71.734);
  cairo_curve_to (cr, 160.063, 72.437, 159.656, 72.719, 158.844, 72.719);

  cairo_curve_to (cr, 158.297, 72.719, 157.844, 72.578, 157.531, 72.312);

  cairo_curve_to (cr, 157.313, 72.109, 157.219, 71.891, 157.219, 71.531);

  cairo_curve_to (cr, 157.219, 70.875, 157.609, 70.453, 158.203, 70.453);

  cairo_curve_to (cr, 158.313, 70.453, 158.406, 70.453, 158.516, 70.484);

  cairo_curve_to (cr, 158.688, 70.531, 158.797, 70.531, 158.922, 70.531);

  cairo_curve_to (cr, 159.25, 70.531, 159.5, 70.375, 159.5, 70.172);

  cairo_curve_to (cr, 159.5, 70, 159.375, 69.906, 159.109, 69.906);

  cairo_curve_to (cr, 158.984, 69.906, 158.984, 69.906, 158.656, 69.953);

  cairo_line_to (cr, 158.594, 69.953);
  cairo_curve_to (cr, 158.281, 69.984, 158.25, 69.984, 158.156, 69.984);

  cairo_curve_to (cr, 157.578, 69.984, 157.203, 69.594, 157.203, 68.984);

  cairo_curve_to (cr, 157.203, 68.312, 157.688, 67.797, 158.266, 67.797);

  cairo_curve_to (cr, 158.672, 67.797, 158.938, 67.969, 159.063, 68.328);

  cairo_line_to (cr, 159.094, 68.406);
  cairo_curve_to (cr, 159.219, 68.812, 159.422, 68.984, 159.703, 68.984);

  cairo_curve_to (cr, 160.016, 68.984, 160.25, 68.734, 160.25, 68.422);

  cairo_curve_to (cr, 160.25, 67.844, 159.5, 67.406, 158.438, 67.406);

  cairo_curve_to (cr, 157.703, 67.406, 157.203, 67.562, 156.75, 67.906);

  cairo_curve_to (cr, 156.344, 68.219, 156.156, 68.547, 156.156, 68.969);

  cairo_curve_to (cr, 156.156, 69.344, 156.328, 69.687, 156.609, 69.891);

  cairo_curve_to (cr, 156.766, 69.984, 156.875, 70.047, 157.219, 70.219);

  cairo_curve_to (cr, 156.391, 70.484, 156, 70.953, 156, 71.625);

  cairo_curve_to (cr, 156, 72.578, 157, 73.266, 158.391, 73.266);

  cairo_curve_to (cr, 159.656, 73.266, 160.359, 72.812, 160.578, 71.828);

  cairo_close_path (cr);
  cairo_move_to (cr, 160.688, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 32 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 10 */
  cairo_move_to (cr, 161.156, 71.844);
  cairo_line_to (cr, 161.156, 75.375);
  cairo_line_to (cr, 161.719, 75.375);
  cairo_line_to (cr, 161.719, 73.547);
  cairo_curve_to (cr, 161.719, 72.703, 162.078, 72.328, 162.844, 72.344);

  cairo_line_to (cr, 162.844, 71.766);
  cairo_curve_to (cr, 162.75, 71.75, 162.703, 71.75, 162.625, 71.75);

  cairo_curve_to (cr, 162.266, 71.75, 161.984, 71.969, 161.672, 72.484);

  cairo_line_to (cr, 161.672, 71.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 162.934, 75.375);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 33 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 65 */
  cairo_move_to (cr, 163.918, 71.969);
  cairo_line_to (cr, 163.918, 73.125);
  cairo_line_to (cr, 164.59, 73.125);
  cairo_line_to (cr, 164.59, 73.328);
  cairo_curve_to (cr, 164.59, 74.094, 164.449, 74.312, 163.918, 74.344);

  cairo_line_to (cr, 163.918, 74.766);
  cairo_curve_to (cr, 164.668, 74.781, 165.105, 74.25, 165.105, 73.297);

  cairo_line_to (cr, 165.105, 71.969);
  cairo_close_path (cr);
  cairo_move_to (cr, 168.934, 67.281);
  cairo_line_to (cr, 167.965, 67.281);
  cairo_line_to (cr, 167.965, 65.672);
  cairo_line_to (cr, 167.027, 65.672);
  cairo_line_to (cr, 167.027, 67.281);
  cairo_line_to (cr, 166.215, 67.281);
  cairo_line_to (cr, 166.215, 68.031);
  cairo_line_to (cr, 167.027, 68.031);
  cairo_line_to (cr, 167.027, 72.453);
  cairo_curve_to (cr, 167.027, 73.062, 167.434, 73.375, 168.168, 73.375);

  cairo_curve_to (cr, 168.418, 73.375, 168.621, 73.359, 168.934, 73.297);

  cairo_line_to (cr, 168.934, 72.516);
  cairo_curve_to (cr, 168.793, 72.562, 168.668, 72.562, 168.48, 72.562);

  cairo_curve_to (cr, 168.074, 72.562, 167.965, 72.453, 167.965, 72.047);

  cairo_line_to (cr, 167.965, 68.031);
  cairo_line_to (cr, 168.934, 68.031);
  cairo_close_path (cr);
  cairo_move_to (cr, 175.23, 72.578);
  cairo_curve_to (cr, 175.137, 72.594, 175.09, 72.594, 175.027, 72.594);

  cairo_curve_to (cr, 174.699, 72.594, 174.527, 72.437, 174.527, 72.141);

  cairo_line_to (cr, 174.527, 68.703);
  cairo_curve_to (cr, 174.527, 67.672, 173.746, 67.109, 172.293, 67.109);

  cairo_curve_to (cr, 171.418, 67.109, 170.73, 67.359, 170.324, 67.797);

  cairo_curve_to (cr, 170.059, 68.094, 169.934, 68.422, 169.918, 69.016);

  cairo_line_to (cr, 170.871, 69.016);
  cairo_curve_to (cr, 170.949, 68.297, 171.371, 67.969, 172.262, 67.969);

  cairo_curve_to (cr, 173.121, 67.969, 173.59, 68.281, 173.59, 68.844);

  cairo_line_to (cr, 173.59, 69.094);
  cairo_curve_to (cr, 173.574, 69.484, 173.371, 69.641, 172.605, 69.734);

  cairo_curve_to (cr, 171.262, 69.906, 171.059, 69.953, 170.699, 70.094);

  cairo_curve_to (cr, 170.012, 70.375, 169.652, 70.891, 169.652, 71.656);

  cairo_curve_to (cr, 169.652, 72.719, 170.402, 73.375, 171.605, 73.375);

  cairo_curve_to (cr, 172.355, 73.375, 172.949, 73.125, 173.621, 72.516);

  cairo_curve_to (cr, 173.684, 73.125, 173.98, 73.375, 174.59, 73.375);

  cairo_curve_to (cr, 174.793, 73.375, 174.918, 73.359, 175.23, 73.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 173.59, 71.281);
  cairo_curve_to (cr, 173.59, 71.594, 173.496, 71.781, 173.215, 72.047);

  cairo_curve_to (cr, 172.824, 72.391, 172.355, 72.562, 171.809, 72.562);

  cairo_curve_to (cr, 171.074, 72.562, 170.637, 72.219, 170.637, 71.625);

  cairo_curve_to (cr, 170.637, 71.016, 171.043, 70.703, 172.074, 70.562);

  cairo_curve_to (cr, 173.074, 70.422, 173.262, 70.375, 173.59, 70.234);

  cairo_close_path (cr);
  cairo_move_to (cr, 176.234, 67.281);
  cairo_line_to (cr, 176.234, 73.125);
  cairo_line_to (cr, 177.172, 73.125);
  cairo_line_to (cr, 177.172, 69.906);
  cairo_curve_to (cr, 177.172, 68.703, 177.813, 67.922, 178.781, 67.922);

  cairo_curve_to (cr, 179.531, 67.922, 180, 68.375, 180, 69.078);

  cairo_line_to (cr, 180, 73.125);
  cairo_line_to (cr, 180.938, 73.125);
  cairo_line_to (cr, 180.938, 68.703);
  cairo_curve_to (cr, 180.938, 67.734, 180.203, 67.109, 179.063, 67.109);

  cairo_curve_to (cr, 178.188, 67.109, 177.625, 67.453, 177.094, 68.266);

  cairo_line_to (cr, 177.094, 67.281);
  cairo_close_path (cr);
  cairo_move_to (cr, 181.691, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
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
  /* Path Elements 0 to 23 */
  cairo_move_to (cr, 184.582, 68.172);
  cairo_curve_to (cr, 184.004, 68.344, 183.645, 68.5, 183.316, 68.719);

  cairo_curve_to (cr, 182.566, 69.219, 182.145, 70, 182.145, 70.844);

  cairo_curve_to (cr, 182.145, 72.172, 183.223, 73.266, 184.52, 73.266);

  cairo_curve_to (cr, 185.207, 73.266, 185.801, 73, 186.348, 72.437);

  cairo_curve_to (cr, 186.879, 71.891, 187.129, 71.281, 187.129, 70.531);

  cairo_curve_to (cr, 187.129, 69.812, 186.879, 69.125, 186.426, 68.578);

  cairo_curve_to (cr, 186.035, 68.125, 185.77, 67.922, 184.816, 67.375);

  cairo_curve_to (cr, 183.801, 66.766, 183.363, 66.359, 183.363, 65.984);

  cairo_curve_to (cr, 183.363, 65.625, 183.723, 65.344, 184.176, 65.344);

  cairo_curve_to (cr, 184.535, 65.344, 184.832, 65.469, 185.113, 65.766);

  cairo_curve_to (cr, 185.738, 66.375, 185.848, 66.437, 186.145, 66.437);

  cairo_curve_to (cr, 186.457, 66.437, 186.676, 66.234, 186.676, 65.969);

  cairo_curve_to (cr, 186.676, 65.359, 185.691, 64.875, 184.504, 64.875);

  cairo_curve_to (cr, 183.441, 64.875, 182.801, 65.281, 182.801, 65.969);

  cairo_curve_to (cr, 182.801, 66.391, 182.988, 66.703, 183.473, 67.156);

  cairo_close_path (cr);
  cairo_move_to (cr, 184.91, 68.469);
  cairo_curve_to (cr, 185.676, 69.172, 186.02, 69.953, 186.02, 70.906);

  cairo_curve_to (cr, 186.02, 72.016, 185.488, 72.75, 184.707, 72.75);

  cairo_curve_to (cr, 183.91, 72.75, 183.363, 71.875, 183.363, 70.594);

  cairo_curve_to (cr, 183.363, 69.391, 183.879, 68.687, 184.91, 68.469);

  cairo_close_path (cr);
  cairo_move_to (cr, 187.246, 73.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 211.5,693, 27,9);

  /*
   * Path # 35 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 108.5, 63);
  cairo_line_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 108.5, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 211.5,693, 27,9);

  /*
   * Path # 36 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 108.5, 63);
  cairo_line_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 108.5, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 37 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 194, 36);
  cairo_line_to (cr, 194, 27);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 38 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 194, 36);
  cairo_line_to (cr, 194, 27);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 39 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 194, 27);
  cairo_line_to (cr, 167, 27);
  cairo_line_to (cr, 108.5, 63);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 194, 27);
  cairo_line_to (cr, 167, 27);
  cairo_line_to (cr, 108.5, 63);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 162,693, 27,9);

  /*
   * Path # 41 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 59, 63);
  cairo_line_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 59, 72);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
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
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 86, 72);
  cairo_line_to (cr, 144.5, 36);
  cairo_line_to (cr, 144.5, 27);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 43 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 86, 63);
  cairo_line_to (cr, 144.5, 27);
  cairo_line_to (cr, 117.5, 27);
  cairo_line_to (cr, 59, 63);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 211.5,693, 27,9);

  /*
   * Path # 44 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 108.5, 63);
  cairo_line_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 108.5, 72);
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
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 135.5, 72);
  cairo_line_to (cr, 194, 36);
  cairo_line_to (cr, 194, 27);
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
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 135.5, 63);
  cairo_line_to (cr, 194, 27);
  cairo_line_to (cr, 167, 27);
  cairo_line_to (cr, 108.5, 63);
  cairo_close_path (cr);
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
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 189.5, 36);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 189.5, 36);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 189,9);

  /*
   * Path # 49 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 189.5, 36);
  cairo_line_to (cr, 189.5, 45);
  cairo_line_to (cr, 0.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 189,9);

  /*
   * Path # 50 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 189.5, 36);
  cairo_line_to (cr, 189.5, 45);
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
  cairo_move_to (cr, 189.5, 36);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 243.5, 9);
  cairo_line_to (cr, 189.5, 45);
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
  cairo_move_to (cr, 189.5, 36);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 243.5, 9);
  cairo_line_to (cr, 189.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 53 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 54.5, 0);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 189.5, 36);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,720, 189,9);

  /*
   * Path # 54 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 36);
  cairo_line_to (cr, 189.5, 36);
  cairo_line_to (cr, 189.5, 45);
  cairo_line_to (cr, 0.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
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
  cairo_move_to (cr, 189.5, 36);
  cairo_line_to (cr, 243.5, 0);
  cairo_line_to (cr, 243.5, 9);
  cairo_line_to (cr, 189.5, 45);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 56 (polyline):
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
   * Path # 57 (polyline):
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
   * Path # 58 (polygon):
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
   * Path # 59 (polygon):
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
   * Path # 60 (polygon):
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
   * Path # 61 (polygon):
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
   * Path # 62 (polyline):
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
   * Path # 63 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 7 */
  cairo_move_to (cr, 33.953, 15.484);
  cairo_line_to (cr, 32.906, 15.484);
  cairo_line_to (cr, 32.906, 23.625);
  cairo_line_to (cr, 38.031, 23.625);
  cairo_line_to (cr, 38.031, 22.703);
  cairo_line_to (cr, 33.953, 22.703);
  cairo_close_path (cr);
  cairo_move_to (cr, 38.254, 23.625);
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
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 81.5, 0);
  cairo_line_to (cr, 68, 0);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 65 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 63.5, 29.25);
  cairo_line_to (cr, 81.5, 29.25);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 59, 29.25);
  cairo_line_to (cr, 63.5, 27.562);
  cairo_line_to (cr, 63.5, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
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
  cairo_move_to (cr, 59, 29.25);
  cairo_line_to (cr, 63.5, 27.562);
  cairo_line_to (cr, 63.5, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 86, 29.25);
  cairo_line_to (cr, 81.5, 30.937);
  cairo_line_to (cr, 81.5, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 69 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 86, 29.25);
  cairo_line_to (cr, 81.5, 30.937);
  cairo_line_to (cr, 81.5, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 14 */
  cairo_move_to (cr, 76.406, 25.875);
  cairo_line_to (cr, 78.5, 17.734);
  cairo_line_to (cr, 77.328, 17.734);
  cairo_line_to (cr, 75.813, 24.344);
  cairo_line_to (cr, 73.938, 17.734);
  cairo_line_to (cr, 72.813, 17.734);
  cairo_line_to (cr, 70.969, 24.344);
  cairo_line_to (cr, 69.422, 17.734);
  cairo_line_to (cr, 68.25, 17.734);
  cairo_line_to (cr, 70.359, 25.875);
  cairo_line_to (cr, 71.516, 25.875);
  cairo_line_to (cr, 73.359, 19.187);
  cairo_line_to (cr, 75.266, 25.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 78.617, 25.875);
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
  cairo_move_to (cr, 90.5, 29.25);
  cairo_line_to (cr, 104, 29.25);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 86, 29.25);
  cairo_line_to (cr, 90.5, 27.562);
  cairo_line_to (cr, 90.5, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
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
  cairo_move_to (cr, 86, 29.25);
  cairo_line_to (cr, 90.5, 27.562);
  cairo_line_to (cr, 90.5, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
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
  cairo_move_to (cr, 108.5, 29.25);
  cairo_line_to (cr, 104, 30.937);
  cairo_line_to (cr, 104, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
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
  cairo_move_to (cr, 108.5, 29.25);
  cairo_line_to (cr, 104, 30.937);
  cairo_line_to (cr, 104, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  cairo_move_to (cr, 101.734, 20.125);
  cairo_curve_to (cr, 101.734, 19.578, 101.703, 19.422, 101.531, 19.031);

  cairo_curve_to (cr, 101.078, 18.094, 100.109, 17.609, 98.719, 17.609);

  cairo_curve_to (cr, 96.906, 17.609, 95.797, 18.516, 95.797, 20);

  cairo_curve_to (cr, 95.797, 20.984, 96.328, 21.609, 97.406, 21.891);

  cairo_line_to (cr, 99.453, 22.422);
  cairo_curve_to (cr, 100.5, 22.703, 100.969, 23.109, 100.969, 23.75);

  cairo_curve_to (cr, 100.969, 24.172, 100.734, 24.625, 100.391, 24.875);

  cairo_curve_to (cr, 100.063, 25.109, 99.531, 25.219, 98.875, 25.219);

  cairo_curve_to (cr, 97.969, 25.219, 97.359, 25, 96.969, 24.531);

  cairo_curve_to (cr, 96.656, 24.172, 96.531, 23.781, 96.531, 23.281);

  cairo_line_to (cr, 95.547, 23.281);
  cairo_curve_to (cr, 95.547, 24.031, 95.703, 24.531, 96.031, 24.969);

  cairo_curve_to (cr, 96.594, 25.734, 97.547, 26.125, 98.797, 26.125);

  cairo_curve_to (cr, 99.781, 26.125, 100.594, 25.906, 101.125, 25.5);

  cairo_curve_to (cr, 101.672, 25.078, 102.016, 24.344, 102.016, 23.641);

  cairo_curve_to (cr, 102.016, 22.641, 101.391, 21.906, 100.266, 21.609);

  cairo_line_to (cr, 98.203, 21.062);
  cairo_curve_to (cr, 97.203, 20.781, 96.844, 20.469, 96.844, 19.844);

  cairo_curve_to (cr, 96.844, 19.031, 97.578, 18.484, 98.688, 18.484);

  cairo_curve_to (cr, 100, 18.484, 100.734, 19.062, 100.75, 20.125);

  cairo_close_path (cr);
  cairo_move_to (cr, 102.5, 25.875);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 77 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 189.5, 45);
  cairo_line_to (cr, 243.5, 9);
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
  /* Path Elements 0 to 42 */
  cairo_move_to (cr, 224.516, 32.828);
  cairo_line_to (cr, 221.078, 32.828);
  cairo_line_to (cr, 221.078, 33.75);
  cairo_line_to (cr, 223.594, 33.75);
  cairo_line_to (cr, 223.594, 33.969);
  cairo_curve_to (cr, 223.594, 35.422, 222.5, 36.469, 221, 36.469);

  cairo_curve_to (cr, 220.172, 36.469, 219.406, 36.172, 218.922, 35.641);

  cairo_curve_to (cr, 218.375, 35.062, 218.047, 34.094, 218.047, 33.094);

  cairo_curve_to (cr, 218.047, 31.094, 219.203, 29.766, 220.938, 29.766);

  cairo_curve_to (cr, 222.203, 29.766, 223.109, 30.406, 223.328, 31.453);

  cairo_line_to (cr, 224.406, 31.453);
  cairo_curve_to (cr, 224.109, 29.812, 222.844, 28.859, 220.953, 28.859);

  cairo_curve_to (cr, 219.953, 28.859, 219.141, 29.109, 218.484, 29.641);

  cairo_curve_to (cr, 217.531, 30.422, 217, 31.687, 217, 33.141);

  cairo_curve_to (cr, 217, 35.641, 218.547, 37.375, 220.781, 37.375);

  cairo_curve_to (cr, 221.891, 37.375, 222.781, 36.969, 223.594, 36.094);

  cairo_line_to (cr, 223.844, 37.172);
  cairo_line_to (cr, 224.516, 37.172);
  cairo_close_path (cr);
  cairo_move_to (cr, 232.563, 28.984);
  cairo_line_to (cr, 231.563, 28.984);
  cairo_line_to (cr, 231.563, 35.641);
  cairo_line_to (cr, 227.25, 28.984);
  cairo_line_to (cr, 226.109, 28.984);
  cairo_line_to (cr, 226.109, 37.125);
  cairo_line_to (cr, 227.109, 37.125);
  cairo_line_to (cr, 227.109, 30.531);
  cairo_line_to (cr, 231.375, 37.125);
  cairo_line_to (cr, 232.563, 37.125);
  cairo_close_path (cr);
  cairo_move_to (cr, 234.371, 37.125);
  cairo_line_to (cr, 237.559, 37.125);
  cairo_curve_to (cr, 239.637, 37.125, 240.918, 35.578, 240.918, 33.047);

  cairo_curve_to (cr, 240.918, 30.531, 239.652, 28.984, 237.559, 28.984);

  cairo_line_to (cr, 234.371, 28.984);
  cairo_close_path (cr);
  cairo_move_to (cr, 235.434, 36.203);
  cairo_line_to (cr, 235.434, 29.906);
  cairo_line_to (cr, 237.371, 29.906);
  cairo_curve_to (cr, 238.996, 29.906, 239.855, 30.984, 239.855, 33.062);

  cairo_curve_to (cr, 239.855, 35.125, 238.996, 36.203, 237.371, 36.203);

  cairo_close_path (cr);
  cairo_move_to (cr, 241.492, 37.125);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 79 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 135.5, 24.75);
  cairo_line_to (cr, 135.5, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 80 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 108.5, 24.75);
  cairo_line_to (cr, 108.5, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 81 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 207.5, 31.5);
  cairo_line_to (cr, 216.5, 31.5);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 203, 31.5);
  cairo_line_to (cr, 207.5, 29.812);
  cairo_line_to (cr, 207.5, 33.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 83 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 203, 31.5);
  cairo_line_to (cr, 207.5, 29.812);
  cairo_line_to (cr, 207.5, 33.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 84 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 131, 0);
  cairo_line_to (cr, 117.5, 0);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 85 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 113, 29.25);
  cairo_line_to (cr, 131, 29.25);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 108.5, 29.25);
  cairo_line_to (cr, 113, 27.562);
  cairo_line_to (cr, 113, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 87 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 108.5, 29.25);
  cairo_line_to (cr, 113, 27.562);
  cairo_line_to (cr, 113, 30.937);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 135.5, 29.25);
  cairo_line_to (cr, 131, 30.937);
  cairo_line_to (cr, 131, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 89 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 135.5, 29.25);
  cairo_line_to (cr, 131, 30.937);
  cairo_line_to (cr, 131, 27.562);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 14 */
  cairo_move_to (cr, 125.906, 25.875);
  cairo_line_to (cr, 128, 17.734);
  cairo_line_to (cr, 126.828, 17.734);
  cairo_line_to (cr, 125.313, 24.344);
  cairo_line_to (cr, 123.438, 17.734);
  cairo_line_to (cr, 122.313, 17.734);
  cairo_line_to (cr, 120.469, 24.344);
  cairo_line_to (cr, 118.922, 17.734);
  cairo_line_to (cr, 117.75, 17.734);
  cairo_line_to (cr, 119.859, 25.875);
  cairo_line_to (cr, 121.016, 25.875);
  cairo_line_to (cr, 122.859, 19.187);
  cairo_line_to (cr, 124.766, 25.875);
  cairo_close_path (cr);
  cairo_move_to (cr, 128.117, 25.875);
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
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 86, 24.75);
  cairo_line_to (cr, 86, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 92 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 59, 24.75);
  cairo_line_to (cr, 59, 60.75);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 189,9);

  /*
   * Path # 93 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 189.5, 81);
  cairo_line_to (cr, 189.5, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 189,9);

  /*
   * Path # 94 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 189.5, 81);
  cairo_line_to (cr, 189.5, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 95 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 189.5, 81);
  cairo_line_to (cr, 243.5, 45);
  cairo_line_to (cr, 243.5, 54);
  cairo_line_to (cr, 189.5, 90);
  cairo_close_path (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 96 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 189.5, 81);
  cairo_line_to (cr, 243.5, 45);
  cairo_line_to (cr, 243.5, 54);
  cairo_line_to (cr, 189.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 103.5,675, 189,9);

  /*
   * Path # 97 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.5, 81);
  cairo_line_to (cr, 189.5, 81);
  cairo_line_to (cr, 189.5, 90);
  cairo_line_to (cr, 0.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 98 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 189.5, 81);
  cairo_line_to (cr, 243.5, 45);
  cairo_line_to (cr, 243.5, 54);
  cairo_line_to (cr, 189.5, 90);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_restore (cr);

  return cr;
} /* end of figure_coupled_stripline_page_1_render() */

/* Total number of pages */
int figure_coupled_stripline_total_pages;

/* Array of the individual page render functions */
cairo_t * (*figure_coupled_stripline_render[1])(cairo_surface_t *, cairo_t *);

/* array of pointers to the widths and heights */
int figure_coupled_stripline_width[1];
int figure_coupled_stripline_height[1];

/* This function should be called at the beginning of the user program */
void figure_coupled_stripline_init(void)
{

  figure_coupled_stripline_total_pages = 1;

  figure_coupled_stripline_render[0] = figure_coupled_stripline_page_1_render;

  figure_coupled_stripline_width[0] = figure_coupled_stripline_page_1_width;
  figure_coupled_stripline_height[0] = figure_coupled_stripline_page_1_height;
}

float figure_coupled_stripline_width_max = 243;
float figure_coupled_stripline_height_max = 94;
