/* ***** Generated from pstoedit ***** */
#include <cairo.h>
#include <stdio.h>

/*
 * Original bounding box = for page # 1 is
 * LL: x: 123 y: 549 UR: x: 306 y: 684
 * The figure has been offset by (-123, 684)
 * to move LL to (0,0).  The width and height
 * can be read from the following two variables:
 */
static int figure_coax_page_1_width = 183;
static int figure_coax_page_1_height = 135;

static cairo_t * figure_coax_page_1_render(cairo_surface_t *cs, cairo_t *cr)
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
   * Path # 1 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 156, 72);
  cairo_curve_to (cr, 156, 106.793, 127.793, 135, 93, 135);

  cairo_curve_to (cr, 58.207, 135, 30, 106.793, 30, 72);

  cairo_curve_to (cr, 30, 37.207, 58.207, 9, 93, 9);

  cairo_curve_to (cr, 127.793, 9, 156, 37.207, 156, 72);

  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 2 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 156, 72);
  cairo_curve_to (cr, 156, 106.793, 127.793, 135, 93, 135);

  cairo_curve_to (cr, 58.207, 135, 30, 106.793, 30, 72);

  cairo_curve_to (cr, 30, 37.207, 58.207, 9, 93, 9);

  cairo_curve_to (cr, 127.793, 9, 156, 37.207, 156, 72);

  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 12, 85.5);
  cairo_line_to (cr, 12, 76.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 4 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 90);
  cairo_line_to (cr, 10.313, 85.5);
  cairo_line_to (cr, 13.688, 85.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 5 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 90);
  cairo_line_to (cr, 10.313, 85.5);
  cairo_line_to (cr, 13.688, 85.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  cairo_move_to (cr, 12, 72);
  cairo_line_to (cr, 13.688, 76.5);
  cairo_line_to (cr, 10.313, 76.5);
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
  cairo_move_to (cr, 12, 72);
  cairo_line_to (cr, 13.688, 76.5);
  cairo_line_to (cr, 10.313, 76.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 123.188,596.813, 5.625,12.937);

  /*
   * Path # 8 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 0.188004, 74.25);
  cairo_line_to (cr, 5.813, 74.25);
  cairo_line_to (cr, 5.813, 87.187);
  cairo_line_to (cr, 0.188004, 87.187);
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
  /* Path Elements 0 to 12 */
  cairo_move_to (cr, 5.51601, 81.062);
  cairo_curve_to (cr, 5.46899, 80.484, 5.34399, 80.125, 5.125, 79.797);

  cairo_curve_to (cr, 4.719, 79.25, 4, 78.922, 3.172, 78.922);

  cairo_curve_to (cr, 1.563, 78.922, 0.530998, 80.172, 0.530998, 82.109);

  cairo_curve_to (cr, 0.530998, 84, 1.563, 85.187, 3.156, 85.187);

  cairo_curve_to (cr, 4.578, 85.187, 5.46899, 84.359, 5.578, 82.922);

  cairo_line_to (cr, 4.625, 82.922);
  cairo_curve_to (cr, 4.469, 83.859, 3.984, 84.328, 3.188, 84.328);

  cairo_curve_to (cr, 2.141, 84.328, 1.516, 83.5, 1.516, 82.109);

  cairo_curve_to (cr, 1.516, 80.656, 2.125, 79.781, 3.156, 79.781);

  cairo_curve_to (cr, 3.953, 79.781, 4.453, 80.234, 4.563, 81.062);

  cairo_close_path (cr);
  cairo_move_to (cr, 5.813, 84.937);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
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
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 147, 72);
  cairo_curve_to (cr, 147, 101.82, 122.82, 126, 93, 126);

  cairo_curve_to (cr, 63.18, 126, 39, 101.82, 39, 72);

  cairo_curve_to (cr, 39, 42.18, 63.18, 18, 93, 18);

  cairo_curve_to (cr, 122.82, 18, 147, 42.18, 147, 72);

  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 11 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 147, 72);
  cairo_curve_to (cr, 147, 101.82, 122.82, 126, 93, 126);

  cairo_curve_to (cr, 63.18, 126, 39, 101.82, 39, 72);

  cairo_curve_to (cr, 39, 42.18, 63.18, 18, 93, 18);

  cairo_curve_to (cr, 122.82, 18, 147, 42.18, 147, 72);

  cairo_set_source_rgb (cr, 0.598,0.598,0.598);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 12 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 102, 90);
  cairo_curve_to (cr, 102, 94.969, 97.969, 99, 93, 99);

  cairo_curve_to (cr, 88.031, 99, 84, 94.969, 84, 90);

  cairo_curve_to (cr, 84, 85.031, 88.031, 81, 93, 81);

  cairo_curve_to (cr, 97.969, 81, 102, 85.031, 102, 90);

  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 13 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 102, 90);
  cairo_curve_to (cr, 102, 94.969, 97.969, 99, 93, 99);

  cairo_curve_to (cr, 88.031, 99, 84, 94.969, 84, 90);

  cairo_curve_to (cr, 84, 85.031, 88.031, 81, 93, 81);

  cairo_curve_to (cr, 97.969, 81, 102, 85.031, 102, 90);

  cairo_set_source_rgb (cr, 0.266,0.266,0.266);
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
  cairo_move_to (cr, 93, 72);
  cairo_line_to (cr, 3, 72);
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
  cairo_move_to (cr, 93, 90);
  cairo_line_to (cr, 3, 90);
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
  cairo_move_to (cr, 120, 81);
  cairo_line_to (cr, 102, 81);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 17 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 120, 99);
  cairo_line_to (cr, 102, 99);
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
  cairo_move_to (cr, 111, 94.5);
  cairo_line_to (cr, 111, 85.5);
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
  cairo_move_to (cr, 111, 99);
  cairo_line_to (cr, 109.313, 94.5);
  cairo_line_to (cr, 112.688, 94.5);
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
  cairo_move_to (cr, 111, 99);
  cairo_line_to (cr, 109.313, 94.5);
  cairo_line_to (cr, 112.688, 94.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 21 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 111, 81);
  cairo_line_to (cr, 112.688, 85.5);
  cairo_line_to (cr, 109.313, 85.5);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 111, 81);
  cairo_line_to (cr, 112.688, 85.5);
  cairo_line_to (cr, 109.313, 85.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
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
  /* Path Elements 0 to 44 */
  cairo_move_to (cr, 119.465, 92.969);
  cairo_line_to (cr, 115.246, 92.969);
  cairo_curve_to (cr, 115.355, 92.297, 115.715, 91.875, 116.699, 91.281);

  cairo_line_to (cr, 117.824, 90.656);
  cairo_curve_to (cr, 118.949, 90.047, 119.527, 89.219, 119.527, 88.219);

  cairo_curve_to (cr, 119.527, 87.562, 119.246, 86.937, 118.777, 86.5);

  cairo_curve_to (cr, 118.309, 86.078, 117.715, 85.875, 116.965, 85.875);

  cairo_curve_to (cr, 115.934, 85.875, 115.184, 86.234, 114.746, 86.922);

  cairo_curve_to (cr, 114.465, 87.344, 114.34, 87.844, 114.309, 88.656);

  cairo_line_to (cr, 115.309, 88.656);
  cairo_curve_to (cr, 115.34, 88.109, 115.402, 87.797, 115.543, 87.516);

  cairo_curve_to (cr, 115.809, 87.031, 116.324, 86.734, 116.918, 86.734);

  cairo_curve_to (cr, 117.824, 86.734, 118.512, 87.375, 118.512, 88.25);

  cairo_curve_to (cr, 118.512, 88.891, 118.137, 89.453, 117.418, 89.859);

  cairo_line_to (cr, 116.387, 90.453);
  cairo_curve_to (cr, 114.715, 91.406, 114.215, 92.156, 114.137, 93.922);

  cairo_line_to (cr, 119.465, 93.922);
  cairo_close_path (cr);
  cairo_move_to (cr, 126.047, 93.391);
  cairo_curve_to (cr, 125.953, 93.406, 125.906, 93.406, 125.844, 93.406);

  cairo_curve_to (cr, 125.516, 93.406, 125.344, 93.25, 125.344, 92.953);

  cairo_line_to (cr, 125.344, 89.516);
  cairo_curve_to (cr, 125.344, 88.484, 124.563, 87.922, 123.109, 87.922);

  cairo_curve_to (cr, 122.234, 87.922, 121.547, 88.172, 121.141, 88.609);

  cairo_curve_to (cr, 120.875, 88.906, 120.75, 89.234, 120.734, 89.828);

  cairo_line_to (cr, 121.688, 89.828);
  cairo_curve_to (cr, 121.766, 89.109, 122.188, 88.781, 123.078, 88.781);

  cairo_curve_to (cr, 123.938, 88.781, 124.406, 89.094, 124.406, 89.656);

  cairo_line_to (cr, 124.406, 89.906);
  cairo_curve_to (cr, 124.391, 90.297, 124.188, 90.453, 123.422, 90.547);

  cairo_curve_to (cr, 122.078, 90.719, 121.875, 90.766, 121.516, 90.906);

  cairo_curve_to (cr, 120.828, 91.187, 120.469, 91.703, 120.469, 92.469);

  cairo_curve_to (cr, 120.469, 93.531, 121.219, 94.187, 122.422, 94.187);

  cairo_curve_to (cr, 123.172, 94.187, 123.766, 93.937, 124.438, 93.328);

  cairo_curve_to (cr, 124.5, 93.937, 124.797, 94.187, 125.406, 94.187);

  cairo_curve_to (cr, 125.609, 94.187, 125.734, 94.172, 126.047, 94.094);

  cairo_close_path (cr);
  cairo_move_to (cr, 124.406, 92.094);
  cairo_curve_to (cr, 124.406, 92.406, 124.313, 92.594, 124.031, 92.859);

  cairo_curve_to (cr, 123.641, 93.203, 123.172, 93.375, 122.625, 93.375);

  cairo_curve_to (cr, 121.891, 93.375, 121.453, 93.031, 121.453, 92.437);

  cairo_curve_to (cr, 121.453, 91.828, 121.859, 91.516, 122.891, 91.375);

  cairo_curve_to (cr, 123.891, 91.234, 124.078, 91.187, 124.406, 91.047);

  cairo_close_path (cr);
  cairo_move_to (cr, 126.254, 93.937);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 24 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 183, 18);
  cairo_line_to (cr, 3, 18);
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
  cairo_move_to (cr, 165, 121.5);
  cairo_line_to (cr, 165, 22.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 26 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 165, 126);
  cairo_line_to (cr, 163.313, 121.5);
  cairo_line_to (cr, 166.688, 121.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
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
  cairo_move_to (cr, 165, 126);
  cairo_line_to (cr, 163.313, 121.5);
  cairo_line_to (cr, 166.688, 121.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
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
  cairo_move_to (cr, 165, 18);
  cairo_line_to (cr, 166.688, 22.5);
  cairo_line_to (cr, 163.313, 22.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 29 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 165, 18);
  cairo_line_to (cr, 166.688, 22.5);
  cairo_line_to (cr, 163.313, 22.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 290.746,605.813, 12.508,12.937);

  /*
   * Path # 30 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 167.746, 65.25);
  cairo_line_to (cr, 180.254, 65.25);
  cairo_line_to (cr, 180.254, 78.187);
  cairo_line_to (cr, 167.746, 78.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 31 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 34 */
  cairo_move_to (cr, 173.465, 74.969);
  cairo_line_to (cr, 169.246, 74.969);
  cairo_curve_to (cr, 169.355, 74.297, 169.715, 73.875, 170.699, 73.281);

  cairo_line_to (cr, 171.824, 72.656);
  cairo_curve_to (cr, 172.949, 72.047, 173.527, 71.219, 173.527, 70.219);

  cairo_curve_to (cr, 173.527, 69.562, 173.246, 68.937, 172.777, 68.5);

  cairo_curve_to (cr, 172.309, 68.078, 171.715, 67.875, 170.965, 67.875);

  cairo_curve_to (cr, 169.934, 67.875, 169.184, 68.234, 168.746, 68.922);

  cairo_curve_to (cr, 168.465, 69.344, 168.34, 69.844, 168.309, 70.656);

  cairo_line_to (cr, 169.309, 70.656);
  cairo_curve_to (cr, 169.34, 70.109, 169.402, 69.797, 169.543, 69.516);

  cairo_curve_to (cr, 169.809, 69.031, 170.324, 68.734, 170.918, 68.734);

  cairo_curve_to (cr, 171.824, 68.734, 172.512, 69.375, 172.512, 70.25);

  cairo_curve_to (cr, 172.512, 70.891, 172.137, 71.453, 171.418, 71.859);

  cairo_line_to (cr, 170.387, 72.453);
  cairo_curve_to (cr, 168.715, 73.406, 168.215, 74.156, 168.137, 75.922);

  cairo_line_to (cr, 173.465, 75.922);
  cairo_close_path (cr);
  cairo_move_to (cr, 174.609, 67.797);
  cairo_line_to (cr, 174.609, 75.937);
  cairo_line_to (cr, 175.453, 75.937);
  cairo_line_to (cr, 175.453, 75.187);
  cairo_curve_to (cr, 175.906, 75.875, 176.516, 76.187, 177.344, 76.187);

  cairo_curve_to (cr, 178.891, 76.187, 179.922, 74.937, 179.922, 73);

  cairo_curve_to (cr, 179.922, 71.094, 178.953, 69.922, 177.375, 69.922);

  cairo_curve_to (cr, 176.563, 69.922, 175.984, 70.219, 175.547, 70.891);

  cairo_line_to (cr, 175.547, 67.797);
  cairo_close_path (cr);
  cairo_move_to (cr, 177.203, 70.797);
  cairo_curve_to (cr, 178.25, 70.797, 178.938, 71.703, 178.938, 73.094);

  cairo_curve_to (cr, 178.938, 74.422, 178.234, 75.328, 177.203, 75.328);

  cairo_curve_to (cr, 176.188, 75.328, 175.547, 74.437, 175.547, 73.062);

  cairo_curve_to (cr, 175.547, 71.687, 176.188, 70.797, 177.203, 70.797);

  cairo_close_path (cr);
  cairo_move_to (cr, 180.254, 75.937);
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
  /* Path Elements 0 to 67 */
  cairo_move_to (cr, 77.469, 42.531);
  cairo_line_to (cr, 77.469, 48.375);
  cairo_line_to (cr, 78.422, 48.375);
  cairo_line_to (cr, 78.422, 45.344);
  cairo_curve_to (cr, 78.422, 43.937, 79.016, 43.312, 80.313, 43.344);

  cairo_line_to (cr, 80.313, 42.391);
  cairo_curve_to (cr, 80.156, 42.375, 80.063, 42.359, 79.953, 42.359);

  cairo_curve_to (cr, 79.344, 42.359, 78.875, 42.719, 78.344, 43.594);

  cairo_line_to (cr, 78.344, 42.531);
  cairo_close_path (cr);
  cairo_move_to (cr, 81.418, 47.219);
  cairo_line_to (cr, 81.418, 48.375);
  cairo_line_to (cr, 82.09, 48.375);
  cairo_line_to (cr, 82.09, 48.578);
  cairo_curve_to (cr, 82.09, 49.344, 81.949, 49.562, 81.418, 49.594);

  cairo_line_to (cr, 81.418, 50.016);
  cairo_curve_to (cr, 82.168, 50.031, 82.605, 49.5, 82.605, 48.547);

  cairo_line_to (cr, 82.605, 47.219);
  cairo_close_path (cr);
  cairo_move_to (cr, 87.559, 41.156);
  cairo_line_to (cr, 90.262, 41.156);
  cairo_line_to (cr, 90.262, 40.234);
  cairo_line_to (cr, 83.793, 40.234);
  cairo_line_to (cr, 83.793, 41.156);
  cairo_line_to (cr, 86.512, 41.156);
  cairo_line_to (cr, 86.512, 48.375);
  cairo_line_to (cr, 87.559, 48.375);
  cairo_close_path (cr);
  cairo_move_to (cr, 96.477, 47.828);
  cairo_curve_to (cr, 96.383, 47.844, 96.336, 47.844, 96.273, 47.844);

  cairo_curve_to (cr, 95.945, 47.844, 95.773, 47.687, 95.773, 47.391);

  cairo_line_to (cr, 95.773, 43.953);
  cairo_curve_to (cr, 95.773, 42.922, 94.992, 42.359, 93.539, 42.359);

  cairo_curve_to (cr, 92.664, 42.359, 91.977, 42.609, 91.57, 43.047);

  cairo_curve_to (cr, 91.305, 43.344, 91.18, 43.672, 91.164, 44.266);

  cairo_line_to (cr, 92.117, 44.266);
  cairo_curve_to (cr, 92.195, 43.547, 92.617, 43.219, 93.508, 43.219);

  cairo_curve_to (cr, 94.367, 43.219, 94.836, 43.531, 94.836, 44.094);

  cairo_line_to (cr, 94.836, 44.344);
  cairo_curve_to (cr, 94.82, 44.734, 94.617, 44.891, 93.852, 44.984);

  cairo_curve_to (cr, 92.508, 45.156, 92.305, 45.203, 91.945, 45.344);

  cairo_curve_to (cr, 91.258, 45.625, 90.898, 46.141, 90.898, 46.906);

  cairo_curve_to (cr, 90.898, 47.969, 91.648, 48.625, 92.852, 48.625);

  cairo_curve_to (cr, 93.602, 48.625, 94.195, 48.375, 94.867, 47.766);

  cairo_curve_to (cr, 94.93, 48.375, 95.227, 48.625, 95.836, 48.625);

  cairo_curve_to (cr, 96.039, 48.625, 96.164, 48.609, 96.477, 48.531);

  cairo_close_path (cr);
  cairo_move_to (cr, 94.836, 46.531);
  cairo_curve_to (cr, 94.836, 46.844, 94.742, 47.031, 94.461, 47.297);

  cairo_curve_to (cr, 94.07, 47.641, 93.602, 47.812, 93.055, 47.812);

  cairo_curve_to (cr, 92.32, 47.812, 91.883, 47.469, 91.883, 46.875);

  cairo_curve_to (cr, 91.883, 46.266, 92.289, 45.953, 93.32, 45.812);

  cairo_curve_to (cr, 94.32, 45.672, 94.508, 45.625, 94.836, 45.484);

  cairo_close_path (cr);
  cairo_move_to (cr, 97.48, 42.531);
  cairo_line_to (cr, 97.48, 48.375);
  cairo_line_to (cr, 98.418, 48.375);
  cairo_line_to (cr, 98.418, 45.156);
  cairo_curve_to (cr, 98.418, 43.953, 99.059, 43.172, 100.027, 43.172);

  cairo_curve_to (cr, 100.777, 43.172, 101.246, 43.625, 101.246, 44.328);

  cairo_line_to (cr, 101.246, 48.375);
  cairo_line_to (cr, 102.184, 48.375);
  cairo_line_to (cr, 102.184, 43.953);
  cairo_curve_to (cr, 102.184, 42.984, 101.449, 42.359, 100.309, 42.359);

  cairo_curve_to (cr, 99.434, 42.359, 98.871, 42.703, 98.34, 43.516);

  cairo_line_to (cr, 98.34, 42.531);
  cairo_close_path (cr);
  cairo_move_to (cr, 102.938, 48.375);
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
  /* Path Elements 0 to 27 */
  cairo_move_to (cr, 73.672, 44.437);
  cairo_curve_to (cr, 73.391, 45.297, 72.891, 45.625, 71.938, 45.625);

  cairo_curve_to (cr, 71.281, 45.625, 70.734, 45.469, 70.375, 45.141);

  cairo_curve_to (cr, 70.109, 44.906, 70, 44.641, 70, 44.203);

  cairo_curve_to (cr, 70, 43.406, 70.453, 42.891, 71.172, 42.891);

  cairo_curve_to (cr, 71.297, 42.891, 71.406, 42.906, 71.547, 42.937);

  cairo_curve_to (cr, 71.75, 42.984, 71.875, 43, 72.016, 43);

  cairo_curve_to (cr, 72.422, 43, 72.719, 42.812, 72.719, 42.562);

  cairo_curve_to (cr, 72.719, 42.359, 72.547, 42.25, 72.25, 42.25);

  cairo_curve_to (cr, 72.094, 42.25, 72.094, 42.25, 71.703, 42.297);

  cairo_line_to (cr, 71.625, 42.297);
  cairo_curve_to (cr, 71.266, 42.328, 71.219, 42.328, 71.109, 42.328);

  cairo_curve_to (cr, 70.422, 42.328, 69.984, 41.859, 69.984, 41.141);

  cairo_curve_to (cr, 69.984, 40.328, 70.547, 39.703, 71.25, 39.703);

  cairo_curve_to (cr, 71.734, 39.703, 72.047, 39.906, 72.188, 40.344);

  cairo_line_to (cr, 72.219, 40.437);
  cairo_curve_to (cr, 72.375, 40.922, 72.609, 41.141, 72.953, 41.141);

  cairo_curve_to (cr, 73.313, 41.141, 73.609, 40.844, 73.609, 40.469);

  cairo_curve_to (cr, 73.609, 39.75, 72.703, 39.219, 71.438, 39.219);

  cairo_curve_to (cr, 70.578, 39.219, 69.969, 39.422, 69.453, 39.828);

  cairo_curve_to (cr, 68.953, 40.203, 68.734, 40.609, 68.734, 41.125);

  cairo_curve_to (cr, 68.734, 41.562, 68.938, 41.984, 69.266, 42.219);

  cairo_curve_to (cr, 69.453, 42.344, 69.578, 42.406, 70, 42.609);

  cairo_curve_to (cr, 69, 42.953, 68.547, 43.5, 68.547, 44.312);

  cairo_curve_to (cr, 68.547, 45.469, 69.734, 46.297, 71.391, 46.297);

  cairo_curve_to (cr, 72.891, 46.297, 73.719, 45.75, 74, 44.562);

  cairo_close_path (cr);
  cairo_move_to (cr, 74.176, 46.125);
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
  cairo_move_to (cr, 106.016, 43.984);
  cairo_curve_to (cr, 105.438, 44.156, 105.078, 44.312, 104.75, 44.531);

  cairo_curve_to (cr, 104, 45.031, 103.578, 45.812, 103.578, 46.656);

  cairo_curve_to (cr, 103.578, 47.984, 104.656, 49.078, 105.953, 49.078);

  cairo_curve_to (cr, 106.641, 49.078, 107.234, 48.812, 107.781, 48.25);

  cairo_curve_to (cr, 108.313, 47.703, 108.563, 47.094, 108.563, 46.344);

  cairo_curve_to (cr, 108.563, 45.625, 108.313, 44.937, 107.859, 44.391);

  cairo_curve_to (cr, 107.469, 43.937, 107.203, 43.734, 106.25, 43.187);

  cairo_curve_to (cr, 105.234, 42.578, 104.797, 42.172, 104.797, 41.797);

  cairo_curve_to (cr, 104.797, 41.437, 105.156, 41.156, 105.609, 41.156);

  cairo_curve_to (cr, 105.969, 41.156, 106.266, 41.281, 106.547, 41.578);

  cairo_curve_to (cr, 107.172, 42.187, 107.281, 42.25, 107.578, 42.25);

  cairo_curve_to (cr, 107.891, 42.25, 108.109, 42.047, 108.109, 41.781);

  cairo_curve_to (cr, 108.109, 41.172, 107.125, 40.687, 105.938, 40.687);

  cairo_curve_to (cr, 104.875, 40.687, 104.234, 41.094, 104.234, 41.781);

  cairo_curve_to (cr, 104.234, 42.203, 104.422, 42.516, 104.906, 42.969);

  cairo_close_path (cr);
  cairo_move_to (cr, 106.344, 44.281);
  cairo_curve_to (cr, 107.109, 44.984, 107.453, 45.766, 107.453, 46.719);

  cairo_curve_to (cr, 107.453, 47.828, 106.922, 48.562, 106.141, 48.562);

  cairo_curve_to (cr, 105.344, 48.562, 104.797, 47.687, 104.797, 46.406);

  cairo_curve_to (cr, 104.797, 45.203, 105.313, 44.5, 106.344, 44.281);

  cairo_close_path (cr);
  cairo_move_to (cr, 108.68, 48.937);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 35 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 183, 126);
  cairo_line_to (cr, 93, 126);
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
  cairo_move_to (cr, 93, 9);
  cairo_line_to (cr, 3, 9);
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
  cairo_move_to (cr, 12, 27);
  cairo_line_to (cr, 12, 22.5);
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
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 18);
  cairo_line_to (cr, 13.688, 22.5);
  cairo_line_to (cr, 10.313, 22.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 39 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 18);
  cairo_line_to (cr, 13.688, 22.5);
  cairo_line_to (cr, 10.313, 22.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_rectangle (cr, 133.438,641.813, 3.125,12.937);

  /*
   * Path # 40 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 10.438, 29.25);
  cairo_line_to (cr, 13.563, 29.25);
  cairo_line_to (cr, 13.563, 42.187);
  cairo_line_to (cr, 10.438, 42.187);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 41 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 17 */
  cairo_move_to (cr, 13.313, 34.094);
  cairo_line_to (cr, 12.344, 34.094);
  cairo_line_to (cr, 12.344, 32.484);
  cairo_line_to (cr, 11.406, 32.484);
  cairo_line_to (cr, 11.406, 34.094);
  cairo_line_to (cr, 10.594, 34.094);
  cairo_line_to (cr, 10.594, 34.844);
  cairo_line_to (cr, 11.406, 34.844);
  cairo_line_to (cr, 11.406, 39.266);
  cairo_curve_to (cr, 11.406, 39.875, 11.813, 40.187, 12.547, 40.187);

  cairo_curve_to (cr, 12.797, 40.187, 13, 40.172, 13.313, 40.109);

  cairo_line_to (cr, 13.313, 39.328);
  cairo_curve_to (cr, 13.172, 39.375, 13.047, 39.375, 12.859, 39.375);

  cairo_curve_to (cr, 12.453, 39.375, 12.344, 39.266, 12.344, 38.859);

  cairo_line_to (cr, 12.344, 34.844);
  cairo_line_to (cr, 13.313, 34.844);
  cairo_close_path (cr);
  cairo_move_to (cr, 13.563, 39.937);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 42 (polyline):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0.5625);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 1 */
  cairo_move_to (cr, 12, 0);
  cairo_line_to (cr, 12, 4.5);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 43 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 9);
  cairo_line_to (cr, 10.313, 4.5);
  cairo_line_to (cr, 13.688, 4.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 1,1,1);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 44 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 0);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 3 */
  cairo_move_to (cr, 12, 9);
  cairo_line_to (cr, 10.313, 4.5);
  cairo_line_to (cr, 13.688, 4.5);
  cairo_close_path (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 45 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 2.25);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 156, 72);
  cairo_curve_to (cr, 156, 106.793, 127.793, 135, 93, 135);

  cairo_curve_to (cr, 58.207, 135, 30, 106.793, 30, 72);

  cairo_curve_to (cr, 30, 37.207, 58.207, 9, 93, 9);

  cairo_curve_to (cr, 127.793, 9, 156, 37.207, 156, 72);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 46 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 2.25);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 147, 72);
  cairo_curve_to (cr, 147, 101.82, 122.82, 126, 93, 126);

  cairo_curve_to (cr, 63.18, 126, 39, 101.82, 39, 72);

  cairo_curve_to (cr, 39, 42.18, 63.18, 18, 93, 18);

  cairo_curve_to (cr, 122.82, 18, 147, 42.18, 147, 72);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);

  /*
   * Path # 47 (polygon):
   */

  cairo_save (cr);
  cairo_set_line_width (cr, 2.25);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
  cairo_set_dash (cr, NULL, 0, 0.0);
  /* Path Elements 0 to 4 */
  cairo_move_to (cr, 102, 90);
  cairo_curve_to (cr, 102, 94.969, 97.969, 99, 93, 99);

  cairo_curve_to (cr, 88.031, 99, 84, 94.969, 84, 90);

  cairo_curve_to (cr, 84, 85.031, 88.031, 81, 93, 81);

  cairo_curve_to (cr, 97.969, 81, 102, 85.031, 102, 90);

  cairo_set_source_rgb (cr, 0,0,0);
  cairo_stroke (cr);
  cairo_restore (cr);
  cairo_restore (cr);

  return cr;
} /* end of figure_coax_page_1_render() */

/* Total number of pages */
int figure_coax_total_pages;

/* Array of the individual page render functions */
cairo_t * (*figure_coax_render[1])(cairo_surface_t *, cairo_t *);

/* array of pointers to the widths and heights */
int figure_coax_width[1];
int figure_coax_height[1];

/* This function should be called at the beginning of the user program */
void figure_coax_init(void)
{

  figure_coax_total_pages = 1;

  figure_coax_render[0] = figure_coax_page_1_render;

  figure_coax_width[0] = figure_coax_page_1_width;
  figure_coax_height[0] = figure_coax_page_1_height;
}

float figure_coax_width_max = 183;
float figure_coax_height_max = 135;
