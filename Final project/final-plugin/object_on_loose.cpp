//
// Student License - for use by students to meet course requirements and
// perform academic research at degree granting institutions only.  Not
// for government, commercial, or other organizational use.
// File: object_on_loose.cpp
//
// MATLAB Coder version            : 4.3
// C/C++ source code generated on  : 29-May-2020 15:35:37
//

// Include Files
#include "object_on_loose.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

// Variable Definitions
static boolean_T isInitialized_object_on_loose = false;

// Function Declarations
static void apply_row_permutation(double y[30603], const int idx[10201]);
static boolean_T sortLE(const double v[30603], int idx1, int idx2);
static void sortrows(double y[30603]);

// Function Definitions

//
// Arguments    : double y[30603]
//                const int idx[10201]
// Return Type  : void
//
static void apply_row_permutation(double y[30603], const int idx[10201])
{
  int j;
  int i;
  double ycol[10201];
  for (j = 0; j < 3; j++) {
    for (i = 0; i < 10201; i++) {
      ycol[i] = y[(idx[i] + 10201 * j) - 1];
    }

    memcpy(&y[j * 10201], &ycol[0], 10201U * sizeof(double));
  }
}

//
// Arguments    : const double v[30603]
//                int idx1
//                int idx2
// Return Type  : boolean_T
//
static boolean_T sortLE(const double v[30603], int idx1, int idx2)
{
  boolean_T p;
  double d;
  double d1;
  p = true;
  d = v[idx1 + 20401];
  d1 = v[idx2 + 20401];
  if ((!(d == d1)) && ((!rtIsNaN(d)) || (!rtIsNaN(d1))) && (!(d >= d1)) &&
      (!rtIsNaN(d))) {
    p = false;
  }

  return p;
}

//
// Arguments    : double y[30603]
// Return Type  : void
//
static void sortrows(double y[30603])
{
  int idx[10201];
  int k;
  int i;
  int i2;
  int j;
  int pEnd;
  int p;
  int q;
  int qEnd;
  int kEnd;
  int b_i;
  int i1;
  int iwork[10201];
  memset(&idx[0], 0, 10201U * sizeof(int));
  for (k = 0; k <= 10199; k += 2) {
    if (sortLE(y, k + 1, k + 2)) {
      idx[k] = k + 1;
      idx[k + 1] = k + 2;
    } else {
      idx[k] = k + 2;
      idx[k + 1] = k + 1;
    }
  }

  idx[10200] = 10201;
  i = 2;
  while (i < 10201) {
    i2 = i << 1;
    j = 1;
    for (pEnd = i + 1; pEnd < 10202; pEnd = qEnd + i) {
      p = j;
      q = pEnd;
      qEnd = j + i2;
      if (qEnd > 10202) {
        qEnd = 10202;
      }

      k = 0;
      kEnd = qEnd - j;
      while (k + 1 <= kEnd) {
        b_i = idx[q - 1];
        i1 = idx[p - 1];
        if (sortLE(y, i1, b_i)) {
          iwork[k] = i1;
          p++;
          if (p == pEnd) {
            while (q < qEnd) {
              k++;
              iwork[k] = idx[q - 1];
              q++;
            }
          }
        } else {
          iwork[k] = b_i;
          q++;
          if (q == qEnd) {
            while (p < pEnd) {
              k++;
              iwork[k] = idx[p - 1];
              p++;
            }
          }
        }

        k++;
      }

      for (k = 0; k < kEnd; k++) {
        idx[(j + k) - 1] = iwork[k];
      }

      j = qEnd;
    }

    i = i2;
  }

  apply_row_permutation(y, idx);
}

//
// Arguments    : const double x[10000]
//                const double y[10000]
//                double *guess_num
//                double *guess_x
//                double *guess_y
//                double *guess_q
//                double *guess_flipped
// Return Type  : void
//
void object_on_loose(const double x[10000], const double y[10000], double
                     *guess_num, double *guess_x, double *guess_y, double
                     *guess_q, double *guess_flipped)
{
  double sum_x;
  double sum_y;
  int point;
  double x_avg;
  static double X[10000];
  double y_avg;
  static double Y[10000];
  double min_R;
  int i;
  double R_scale;
  int idx;
  static double Hough_Points[3620000];
  int b_idx;
  static double counts[30603];
  double R;
  double q_min;
  double center1q;
  double center1r;
  double center2q;
  double r_min;
  double center2r;
  double v12_idx_1;
  int b_i;
  double center3q;
  double center3r;
  int counts_tmp;
  double v12_idx_0;
  double center4q;
  double center4r;
  double varargin_1[3];
  double x_data[5];
  boolean_T exitg1;
  double center5q;
  double center5r;
  double x_temp;
  double intercept2;
  double b_varargin_1[4];
  double center6q;
  double center6r;
  double slope3;
  double intercept3;
  double c_varargin_1[5];
  double centers[12];
  double slopes[6];
  double intercepts[6];
  double angles[6];
  signed char has_matched[6];
  double kept_slopes[4];
  double kept_intercepts[4];
  boolean_T found_rectangle;
  boolean_T found_triangle;
  double kept_corners[12];
  if (isInitialized_object_on_loose == false) {
    object_on_loose_initialize();
  }

  *guess_num = 2.0;
  *guess_x = 2.0;
  *guess_y = 1.5;
  *guess_q = 1.5707963267948966;
  *guess_flipped = 0.0;

  // // box boundaries
  // // convert to Hough space ************************************************
  // // compute coordinate average
  sum_x = 0.0;
  sum_y = 0.0;
  for (point = 0; point < 10000; point++) {
    X[point] = x[point];
    Y[point] = y[point];
    sum_x += x[point];
    sum_y += y[point];
  }

  x_avg = sum_x / 10000.0;
  y_avg = sum_y / 10000.0;

  // // center data at origin
  // // compute all Hough curves
  // // from 0 to 180, inclusive
  min_R = 0.0;
  sum_x = 0.0;
  for (i = 0; i < 10000; i++) {
    X[i] -= x_avg;
    Y[i] -= y_avg;
    for (idx = 0; idx < 181; idx++) {
      b_idx = 181 * i + idx;
      sum_y = ((static_cast<double>(idx) + 1.0) - 1.0) * 0.017453292519943295;
      R = X[i] * cos(sum_y) + Y[i] * sin(sum_y);
      if (R > sum_x) {
        sum_x = R;
      } else {
        if (R < min_R) {
          min_R = R;
        }
      }

      Hough_Points[b_idx] = sum_y;
      Hough_Points[b_idx + 1810000] = R;
    }
  }

  // // rescale Hough curves
  R_scale = 100.0 / (sum_x - min_R);
  for (i = 0; i < 1810000; i++) {
    Hough_Points[i] *= 31.830988618379067;
    Hough_Points[i + 1810000] = (Hough_Points[i + 1810000] - min_R) * R_scale;
  }

  // // initialize counts matrix
  memset(&counts[0], 0, 30603U * sizeof(double));
  b_idx = 0;
  for (idx = 0; idx < 101; idx++) {
    for (point = 0; point < 101; point++) {
      counts[b_idx + 10201] = point;
      counts[b_idx] = idx;
      b_idx++;
    }
  }

  // // count neighbors
  for (point = 0; point < 1810000; point++) {
    R = ceil(Hough_Points[point] - 1.5);
    if ((0.0 > R) || rtIsNaN(R)) {
      q_min = 0.0;
    } else {
      q_min = R;
    }

    R = floor(Hough_Points[point] + 1.5);
    sum_x = Hough_Points[point + 1810000];
    sum_y = ceil(sum_x - 1.5);
    if ((0.0 > sum_y) || rtIsNaN(sum_y)) {
      r_min = 0.0;
    } else {
      r_min = sum_y;
    }

    sum_y = floor(sum_x + 1.5);
    if ((100.0 < sum_y) || rtIsNaN(sum_y)) {
      sum_y = 100.0;
    }

    if ((100.0 < R) || rtIsNaN(R)) {
      R = 100.0;
    }

    b_i = static_cast<int>((R + (1.0 - q_min)));
    for (i = 0; i < b_i; i++) {
      sum_x = 101.0 * (q_min + static_cast<double>(i)) + 1.0;
      idx = static_cast<int>((sum_y + (1.0 - r_min)));
      for (b_idx = 0; b_idx < idx; b_idx++) {
        counts_tmp = static_cast<int>((sum_x + (r_min + static_cast<double>
          (b_idx)))) + 20401;
        counts[counts_tmp]++;
      }
    }
  }

  // // sort counts matrix
  sortrows(counts);

  // // find 1st centroid
  center1q = counts[0];
  center1r = counts[10201];
  b_idx = 1;

  // // find 2nd centroid
  do {
    center2q = counts[b_idx];
    center2r = counts[b_idx + 10201];
    b_idx++;
    q_min = center2q - center1q;
    v12_idx_1 = center2r - center1r;
  } while (!(q_min * q_min + v12_idx_1 * v12_idx_1 > 225.0));

  // // find 3rd centroid
  do {
    center3q = counts[b_idx];
    center3r = counts[b_idx + 10201];
    b_idx++;
    q_min = center3q - center1q;
    v12_idx_1 = center3r - center1r;
    sum_x = center3q - center2q;
    sum_y = center3r - center2r;
    v12_idx_0 = q_min * q_min + v12_idx_1 * v12_idx_1;
    v12_idx_1 = sum_x * sum_x + sum_y * sum_y;
    if ((v12_idx_0 > v12_idx_1) || (rtIsNaN(v12_idx_0) && (!rtIsNaN(v12_idx_1))))
    {
      v12_idx_0 = v12_idx_1;
    }
  } while (!(v12_idx_0 > 225.0));

  // // find 4th centroid
  do {
    center4q = counts[b_idx];
    center4r = counts[b_idx + 10201];
    b_idx++;
    q_min = center4q - center1q;
    v12_idx_1 = center4r - center1r;
    sum_x = center4q - center2q;
    sum_y = center4r - center2r;
    R = center4q - center3q;
    r_min = center4r - center3r;
    v12_idx_0 = q_min * q_min + v12_idx_1 * v12_idx_1;
    varargin_1[0] = v12_idx_0;
    q_min = sum_x * sum_x + sum_y * sum_y;
    varargin_1[1] = q_min;
    R = R * R + r_min * r_min;
    varargin_1[2] = R;
    x_data[0] = v12_idx_0;
    x_data[1] = q_min;
    x_data[2] = R;
    if (!rtIsNaN(v12_idx_0)) {
      idx = 1;
    } else {
      idx = 0;
      point = 2;
      exitg1 = false;
      while ((!exitg1) && (point <= 3)) {
        if (!rtIsNaN(x_data[point - 1])) {
          idx = point;
          exitg1 = true;
        } else {
          point++;
        }
      }
    }

    if (idx != 0) {
      v12_idx_0 = varargin_1[idx - 1];
      b_i = idx + 1;
      for (point = b_i; point < 4; point++) {
        r_min = varargin_1[point - 1];
        if (v12_idx_0 > r_min) {
          v12_idx_0 = r_min;
        }
      }
    }
  } while (!(v12_idx_0 > 225.0));

  // // find 5th centroid
  do {
    center5q = counts[b_idx];
    center5r = counts[b_idx + 10201];
    b_idx++;
    q_min = center5q - center1q;
    v12_idx_1 = center5r - center1r;
    sum_x = center5q - center2q;
    sum_y = center5r - center2r;
    R = center5q - center3q;
    r_min = center5r - center3r;
    x_temp = center5q - center4q;
    intercept2 = center5r - center4r;
    v12_idx_0 = q_min * q_min + v12_idx_1 * v12_idx_1;
    b_varargin_1[0] = v12_idx_0;
    q_min = sum_x * sum_x + sum_y * sum_y;
    b_varargin_1[1] = q_min;
    R = R * R + r_min * r_min;
    b_varargin_1[2] = R;
    sum_y = x_temp * x_temp + intercept2 * intercept2;
    b_varargin_1[3] = sum_y;
    x_data[0] = v12_idx_0;
    x_data[1] = q_min;
    x_data[2] = R;
    x_data[3] = sum_y;
    if (!rtIsNaN(v12_idx_0)) {
      idx = 1;
    } else {
      idx = 0;
      point = 2;
      exitg1 = false;
      while ((!exitg1) && (point <= 4)) {
        if (!rtIsNaN(x_data[point - 1])) {
          idx = point;
          exitg1 = true;
        } else {
          point++;
        }
      }
    }

    if (idx != 0) {
      v12_idx_0 = b_varargin_1[idx - 1];
      b_i = idx + 1;
      for (point = b_i; point < 5; point++) {
        r_min = b_varargin_1[point - 1];
        if (v12_idx_0 > r_min) {
          v12_idx_0 = r_min;
        }
      }
    }
  } while (!(v12_idx_0 > 225.0));

  // // find 6th centroid
  do {
    center6q = counts[b_idx];
    center6r = counts[b_idx + 10201];
    b_idx++;
    q_min = center6q - center1q;
    v12_idx_1 = center6r - center1r;
    sum_x = center6q - center2q;
    sum_y = center6r - center2r;
    R = center6q - center3q;
    r_min = center6r - center3r;
    x_temp = center6q - center4q;
    intercept2 = center6r - center4r;
    slope3 = center6q - center5q;
    intercept3 = center6r - center5r;
    v12_idx_0 = q_min * q_min + v12_idx_1 * v12_idx_1;
    c_varargin_1[0] = v12_idx_0;
    q_min = sum_x * sum_x + sum_y * sum_y;
    c_varargin_1[1] = q_min;
    R = R * R + r_min * r_min;
    c_varargin_1[2] = R;
    sum_y = x_temp * x_temp + intercept2 * intercept2;
    c_varargin_1[3] = sum_y;
    sum_x = slope3 * slope3 + intercept3 * intercept3;
    c_varargin_1[4] = sum_x;
    x_data[0] = v12_idx_0;
    x_data[1] = q_min;
    x_data[2] = R;
    x_data[3] = sum_y;
    x_data[4] = sum_x;
    if (!rtIsNaN(v12_idx_0)) {
      idx = 1;
    } else {
      idx = 0;
      point = 2;
      exitg1 = false;
      while ((!exitg1) && (point <= 5)) {
        if (!rtIsNaN(x_data[point - 1])) {
          idx = point;
          exitg1 = true;
        } else {
          point++;
        }
      }
    }

    if (idx != 0) {
      v12_idx_0 = c_varargin_1[idx - 1];
      b_i = idx + 1;
      for (point = b_i; point < 6; point++) {
        r_min = c_varargin_1[point - 1];
        if (v12_idx_0 > r_min) {
          v12_idx_0 = r_min;
        }
      }
    }
  } while (!(v12_idx_0 > 225.0));

  // // merge centers into a single array
  centers[0] = counts[0];
  centers[6] = counts[10201];
  centers[1] = center2q;
  centers[7] = center2r;
  centers[2] = center3q;
  centers[8] = center3r;
  centers[3] = center4q;
  centers[9] = center4r;
  centers[4] = center5q;
  centers[10] = center5r;
  centers[5] = center6q;
  centers[11] = center6r;

  // // recenter based on centroids and total neighbor counts
  // // convert back to unscaled and uncentered coords ************************
  // // find lines ********************************************
  // // find first line
  for (point = 0; point < 6; point++) {
    // // decide which of these is best ***********************************
    r_min = centers[point] / 31.830988618379067;
    centers[point] = r_min;
    R = min_R + centers[point + 6] / R_scale;
    centers[point + 6] = R;
    angles[point] = r_min;
    q_min = -(1.0 / tan(r_min));
    slopes[point] = q_min;

    //  points (x0, y0) are on the lines
    //  y-intercepts of the lines
    intercepts[point] = (y_avg + R * sin(r_min)) - q_min * (x_avg + R * cos
      (r_min));
    has_matched[point] = 0;
  }

  R = slopes[0];
  q_min = intercepts[0];

  // // deal with lines having "same" slope as current line
  // // narrowest correct angle diff is atan(0.10/0.40) = 14 deg
  // // so lines with angle diff less than 0.7 are same or parallel
  // // note: line will "match" with itself
  has_matched[0] = 1;
  for (point = 0; point < 5; point++) {
    r_min = fabs(angles[point + 1] - angles[0]);
    if (r_min > 3.1415926535897931) {
      r_min -= 3.1415926535897931;
    }

    if ((!(r_min < 3.1415926535897931 - r_min)) && (!rtIsNaN(3.1415926535897931
          - r_min))) {
      r_min = 3.1415926535897931 - r_min;
    }

    if (57.295779513082323 * r_min <= 10.0) {
      // // line with similar slope that are too close then "match"
      r_min = intercepts[point + 1];
      sum_x = slopes[point + 1];
      sum_y = R - sum_x;
      x_temp = (r_min - q_min) / sum_y;
      sum_x = (R * r_min - sum_x * q_min) / sum_y;

      // // if they cross in box with similar slopes, then they match!
      if ((0.75 < x_temp) && (x_temp < 3.25) && (0.75 < sum_x) && (sum_x < 2.25))
      {
        has_matched[point + 1] = 1;
      }
    }
  }

  // // keep first matching line, which received the most votes,
  // // and ignore any "matches" for further consideration
  // // find second line
  for (idx = 1; has_matched[idx] == 1; idx++) {
  }

  sum_y = angles[idx];
  q_min = slopes[idx];
  intercept2 = intercepts[idx];

  // // deal with lines having "same" slope again
  has_matched[idx] = 1;
  b_i = 4 - idx;
  for (point = 0; point <= b_i; point++) {
    b_idx = (idx + point) + 1;
    r_min = fabs(angles[b_idx] - sum_y);
    if (r_min > 3.1415926535897931) {
      r_min -= 3.1415926535897931;
    }

    if ((!(r_min < 3.1415926535897931 - r_min)) && (!rtIsNaN(3.1415926535897931
          - r_min))) {
      r_min = 3.1415926535897931 - r_min;
    }

    if (57.295779513082323 * r_min <= 10.0) {
      // // line with similar slope that are too close then "match"
      r_min = q_min - slopes[b_idx];
      x_temp = (intercepts[b_idx] - intercept2) / r_min;
      sum_x = (q_min * intercepts[b_idx] - slopes[b_idx] * intercept2) / r_min;

      // // if they cross in box with similar slopes, then they match!
      if ((0.75 < x_temp) && (x_temp < 3.25) && (0.75 < sum_x) && (sum_x < 2.25))
      {
        has_matched[b_idx] = 1;
      }
    }
  }

  // // find third line
  for (idx++; has_matched[idx] == 1; idx++) {
  }

  slope3 = slopes[idx];
  intercept3 = intercepts[idx];

  // // deal with lines having "same" slopes again
  has_matched[idx] = 1;
  b_i = 4 - idx;
  for (point = 0; point <= b_i; point++) {
    b_idx = (idx + point) + 1;
    r_min = fabs(angles[b_idx] - angles[idx]);
    if (r_min > 3.1415926535897931) {
      r_min -= 3.1415926535897931;
    }

    if ((!(r_min < 3.1415926535897931 - r_min)) && (!rtIsNaN(3.1415926535897931
          - r_min))) {
      r_min = 3.1415926535897931 - r_min;
    }

    if (57.295779513082323 * r_min <= 10.0) {
      // // line with similar slope that are too close then "match"
      r_min = slope3 - slopes[b_idx];
      x_temp = (intercepts[b_idx] - intercept3) / r_min;
      sum_x = (slope3 * intercepts[b_idx] - slopes[b_idx] * intercept3) / r_min;

      // // if they cross in box with similar slopes, then they match!
      if ((0.75 < x_temp) && (x_temp < 3.25) && (0.75 < sum_x) && (sum_x < 2.25))
      {
        has_matched[b_idx] = 1;
      }
    }
  }

  // // find fourth line
  for (idx++; has_matched[idx] == 1; idx++) {
  }

  kept_slopes[3] = slopes[idx];
  kept_intercepts[3] = intercepts[idx];

  // // see if fourth line is needed
  // // if line 4 is needed, it should be parallel or perpindicular to others
  sum_x = fabs(angles[idx] - angles[0]);
  if (sum_x > 3.1415926535897931) {
    sum_x -= 3.1415926535897931;
  }

  if (sum_x > 1.5707963267948966) {
    sum_x -= 1.5707963267948966;
  }

  found_rectangle = false;
  if ((!(sum_x < 1.5707963267948966 - sum_x)) && (!rtIsNaN(1.5707963267948966 -
        sum_x))) {
    sum_x = 1.5707963267948966 - sum_x;
  }

  if (57.295779513082323 * sum_x <= 10.0) {
    sum_x = fabs(angles[idx] - sum_y);
    if (sum_x > 3.1415926535897931) {
      sum_x -= 3.1415926535897931;
    }

    if (sum_x > 1.5707963267948966) {
      sum_x -= 1.5707963267948966;
    }

    if ((!(sum_x < 1.5707963267948966 - sum_x)) && (!rtIsNaN(1.5707963267948966
          - sum_x))) {
      sum_x = 1.5707963267948966 - sum_x;
    }

    found_rectangle = (57.295779513082323 * sum_x <= 10.0);
  }

  found_triangle = !found_rectangle;

  // // find boundary lines of object
  kept_slopes[0] = slopes[0];
  kept_slopes[1] = q_min;
  kept_slopes[2] = slope3;
  kept_intercepts[0] = intercepts[0];
  kept_intercepts[1] = intercept2;
  kept_intercepts[2] = intercept3;

  // // find corners of object (4 lines intersect at 6 points)
  memset(&centers[0], 0, 12U * sizeof(double));
  b_idx = 0;
  sum_x = slopes[0];
  sum_y = intercepts[0];
  for (point = 0; point < 3; point++) {
    idx = point + 1;
    R = kept_slopes[idx] - sum_x;
    centers[b_idx] = (sum_y - kept_intercepts[idx]) / R;
    centers[b_idx + 6] = (kept_slopes[idx] * sum_y - sum_x * kept_intercepts[idx])
      / R;
    b_idx++;
  }

  for (point = 0; point < 2; point++) {
    idx = point + 2;
    R = kept_slopes[idx] - q_min;
    centers[b_idx] = (intercept2 - kept_intercepts[idx]) / R;
    centers[b_idx + 6] = (kept_slopes[idx] * intercept2 - q_min *
                          kept_intercepts[idx]) / R;
    b_idx++;
  }

  R = kept_slopes[3] - slope3;
  for (point = 0; point < 1; point++) {
    centers[b_idx] = (intercept3 - kept_intercepts[3]) / R;
    centers[b_idx + 6] = (kept_slopes[3] * intercept3 - slope3 *
                          kept_intercepts[3]) / R;
    b_idx++;
  }

  idx = 0;
  memset(&kept_corners[0], 0, 12U * sizeof(double));
  if (found_rectangle) {
    for (b_idx = 0; b_idx < 6; b_idx++) {
      if ((0.75 < centers[b_idx]) && (centers[b_idx] < 3.25)) {
        r_min = centers[b_idx + 6];
        if ((0.75 < r_min) && (r_min < 2.25)) {
          kept_corners[idx] = centers[b_idx];
          kept_corners[idx + 6] = r_min;
          idx++;
        }
      }
    }
  }

  if (found_triangle) {
    // // first choice - where lines 1, 2, and 3 intersect
    kept_corners[0] = centers[0];
    kept_corners[6] = centers[6];
    kept_corners[1] = centers[1];
    kept_corners[7] = centers[7];
    kept_corners[2] = centers[3];
    kept_corners[8] = centers[9];
  }

  slope3 = kept_corners[1];
  intercept3 = kept_corners[2];
  center4q = kept_corners[7];
  center4r = kept_corners[8];
  if (found_rectangle) {
    v12_idx_0 = kept_corners[1] - kept_corners[0];
    v12_idx_1 = kept_corners[7] - kept_corners[6];
    q_min = kept_corners[2] - kept_corners[0];
    r_min = kept_corners[8] - kept_corners[6];
    sum_x = kept_corners[3] - kept_corners[0];
    sum_y = kept_corners[9] - kept_corners[6];
    x_temp = sqrt(v12_idx_0 * v12_idx_0 + v12_idx_1 * v12_idx_1);
    intercept2 = sqrt(q_min * q_min + r_min * r_min);
    R = sqrt(sum_x * sum_x + sum_y * sum_y);
    if (((intercept2 > x_temp) && (x_temp > R)) || ((R > x_temp) && (x_temp >
          intercept2))) {
      R = x_temp;
      *guess_q = atan(v12_idx_1 / v12_idx_0);
    } else if (((x_temp > intercept2) && (intercept2 > R)) || ((R > intercept2) &&
                (intercept2 > x_temp))) {
      R = intercept2;
      *guess_q = atan(r_min / q_min);
    } else {
      *guess_q = atan(sum_y / sum_x);
    }

    // // split the difference between the two known objects
    if (R > 0.35) {
      *guess_num = 1.0;
    }

    // // center of rectangle is average of corners
    // // compromise between calculations and observations
    *guess_x = ((((kept_corners[0] + kept_corners[1]) + kept_corners[2]) +
                 kept_corners[3]) / 4.0 + x_avg) / 2.0;
    *guess_y = ((((kept_corners[6] + kept_corners[7]) + kept_corners[8]) +
                 kept_corners[9]) / 4.0 + y_avg) / 2.0;

    // // assume all angles are positive
    if (*guess_q < 0.0) {
      *guess_q += 3.1415926535897931;
    }

    // // since there is no way to know if rectangle is flipped
    *guess_flipped = -1.0;
  }

  if (found_triangle) {
    q_min = kept_corners[0] - kept_corners[1];
    v12_idx_1 = kept_corners[6] - kept_corners[7];
    x_temp = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);
    q_min = kept_corners[0] - kept_corners[2];
    v12_idx_1 = kept_corners[6] - kept_corners[8];
    intercept2 = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);
    q_min = kept_corners[1] - kept_corners[2];
    v12_idx_1 = kept_corners[7] - kept_corners[8];
    sum_y = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);

    // // triangle is not approx right, wrong side #3 was chosen
    r_min = x_temp * x_temp;
    R = sum_y * sum_y;
    q_min = intercept2 * intercept2;
    sum_x = acos(((r_min + q_min) - R) / (2.0 * x_temp * intercept2));
    varargin_1[0] = sum_x;
    varargin_1[1] = acos(((r_min + R) - q_min) / (2.0 * x_temp * sum_y));
    varargin_1[2] = acos(((q_min + R) - r_min) / (2.0 * intercept2 * sum_y));
    if (!rtIsNaN(sum_x)) {
      b_idx = 1;
    } else {
      b_idx = 0;
      point = 2;
      exitg1 = false;
      while ((!exitg1) && (point <= 3)) {
        if (!rtIsNaN(varargin_1[point - 1])) {
          b_idx = point;
          exitg1 = true;
        } else {
          point++;
        }
      }
    }

    if (b_idx != 0) {
      sum_x = varargin_1[b_idx - 1];
      b_i = b_idx + 1;
      for (point = b_i; point < 4; point++) {
        r_min = varargin_1[point - 1];
        if (sum_x < r_min) {
          sum_x = r_min;
        }
      }
    }

    if (fabs(100.0 * sum_x / 1.5707963267948966 - 100.0) > 20.0) {
      // // second choice - where lines 1, 2, and 4 intersect
      slope3 = centers[2];
      center4q = centers[8];
      intercept3 = centers[4];
      center4r = centers[10];
      q_min = kept_corners[0] - centers[2];
      v12_idx_1 = kept_corners[6] - centers[8];
      x_temp = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);
      q_min = kept_corners[0] - centers[4];
      v12_idx_1 = kept_corners[6] - centers[10];
      intercept2 = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);
      q_min = centers[2] - centers[4];
      v12_idx_1 = centers[8] - centers[10];
      sum_y = sqrt(q_min * q_min + v12_idx_1 * v12_idx_1);
    }

    if ((x_temp > intercept2) && (x_temp > sum_y)) {
      // // L12 is hypotenuse and opposite corner, #3, is right angle
      *guess_x = intercept3;
      *guess_y = center4r;
      sum_x = fabs(intercept2 - sum_y);
      if (intercept2 > sum_y) {
        // // v31 is long vector and v32 is short vector
        v12_idx_0 = kept_corners[0] - intercept3;
        v12_idx_1 = kept_corners[6] - center4r;
        q_min = slope3 - intercept3;
        r_min = center4q - center4r;
      } else {
        // // v32 is long vector and v31 is short vector
        v12_idx_0 = slope3 - intercept3;
        v12_idx_1 = center4q - center4r;
        q_min = kept_corners[0] - intercept3;
        r_min = kept_corners[6] - center4r;
      }
    } else if ((intercept2 > x_temp) && (intercept2 > sum_y)) {
      // // L13 is hypotenuse and opposite corner, #2, is right angle
      *guess_x = slope3;
      *guess_y = center4q;
      sum_x = fabs(x_temp - sum_y);
      if (x_temp > sum_y) {
        // // v21 is long vector and v23 is short vector
        v12_idx_0 = kept_corners[0] - slope3;
        v12_idx_1 = kept_corners[6] - center4q;
        q_min = intercept3 - slope3;
        r_min = center4r - center4q;
      } else {
        // // v23 is long vector and v21 is short vector
        v12_idx_0 = intercept3 - slope3;
        v12_idx_1 = center4r - center4q;
        q_min = kept_corners[0] - slope3;
        r_min = kept_corners[6] - center4q;
      }
    } else {
      // // L23 is hypotenuse and opposite corner, #1, is right angle
      *guess_x = kept_corners[0];
      *guess_y = kept_corners[6];
      sum_x = fabs(x_temp - intercept2);
      if (x_temp > intercept2) {
        // // v21 is long vector and v31 is short vector
        v12_idx_0 = kept_corners[0] - slope3;
        v12_idx_1 = kept_corners[6] - center4q;
        q_min = kept_corners[0] - intercept3;
        r_min = kept_corners[6] - center4r;
      } else {
        // // v13 is long vector and v12 is short vector
        v12_idx_0 = intercept3 - kept_corners[0];
        v12_idx_1 = center4r - kept_corners[6];
        q_min = slope3 - kept_corners[0];
        r_min = center4q - kept_corners[6];
      }
    }

    // // assume the triangle was not flipped
    *guess_flipped = 0.0;

    // // compute z-coord of cross product and check right-hand rule
    if (v12_idx_0 * r_min - v12_idx_1 * q_min < 0.0) {
      *guess_flipped = 1.0;
    }

    *guess_q = atan(v12_idx_1 / v12_idx_0);
    if (v12_idx_0 < 0.0) {
      *guess_q += 3.1415926535897931;
    } else {
      if (v12_idx_1 < 0.0) {
        *guess_q += 6.2831853071795862;
      }
    }

    // // split the difference between the two known objects
    if (sum_x > 0.225) {
      *guess_num = 3.0;
    } else {
      *guess_num = 4.0;
    }
  }
}

//
// Arguments    : void
// Return Type  : void
//
void object_on_loose_initialize()
{
  rt_InitInfAndNaN();
  isInitialized_object_on_loose = true;
}

//
// Arguments    : void
// Return Type  : void
//
void object_on_loose_terminate()
{
  // (no terminate code required)
  isInitialized_object_on_loose = false;
}

//
// File trailer for object_on_loose.cpp
//
// [EOF]
//
