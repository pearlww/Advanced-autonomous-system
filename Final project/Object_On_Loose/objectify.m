function trial_score = objectify_fixlines()

%// read in raw data or object data
use_sim = true;

use_centers = true;   %// use only initially-computed centers
use_averaged = false; %// weighted-average centroids determine centers
use_sorted = false;   %// averaged centers reweighted by total count

%// box boundaries
X_min = 1;
X_max = 3;
X_avg = (X_min + X_max) / 2;
Y_min = 1;
Y_max = 2;
Y_avg = (Y_min + Y_max) / 2;

%// inital world/global pose of robot
x0 = 0.17;
y0 = 0.25;
q0 = 0;

%// distance from laser to robot center (along x-axis)
rho_l = 0.26;

%// fixed angle used on laser (arbitrarily chose left of robot)
q_used = 90 * (pi / 180);

if use_sim
     debug_generator = true; %// plot generated laser data
     mu = 0; %// assumes properly calibrated laser
     sigma = 0.005; %// assumes typical reading error of 5 mm
     point_density = 300; % 300 data points per meter
     generator(debug_generator, mu, sigma, point_density)
     
     object_points = importdata('object_points.txt');
     answers = importdata('answers.txt');
     
     obj_num = answers(1);
     obj_x = answers(2);
     obj_y = answers(3);
     obj_q = answers(4);
     is_flipped = answers(5);
     
     X = object_points(:, 1);
     Y = object_points(:, 2);     
     num_object_points = length(X);
end

if ~use_sim

     plugin_data = importdata('plugin_data.txt');

     %// odometry/local pose of robot
     pose_x = plugin_data(:, 1);
     pose_y = plugin_data(:, 2);
     pose_q = plugin_data(:, 3);

     %// laser reading for fixed angle used
     r_used = plugin_data(:, 4);

     %// number of datapoints available
     num_points = length(pose_x);

     %// world/global coords of robot
     C = cos(q0);
     S = sin(q0);
     world_x = zeros(num_points, 1);
     world_y = zeros(num_points, 1);
     world_q = zeros(num_points, 1);
     for i = 1:num_points
          world_x(i) = x0 + C * pose_x(i) - S * pose_y(i);
          world_y(i) = y0 + S * pose_x(i) + C * pose_y(i);
          world_q(i) = q0 + pose_q(i);
     end

     %// world/global coords of laser base
     laser_x = zeros(num_points, 1);
     laser_y = zeros(num_points, 1);
     laser_q = zeros(num_points, 1);
     for i = 1:num_points
          q = world_q(i);
          laser_x(i) = world_x(i) + rho_l * cos(q);
          laser_y(i) = world_y(i) + rho_l * sin(q);
          laser_q(i) = q;
     end

     %// world/global coords of laser readings
     point_x = zeros(num_points, 1);
     point_y = zeros(num_points, 1);
     point_q = zeros(num_points, 1);
     for i = 1:num_points
          r = r_used(i);
          q = laser_q(i) + q_used;
          point_x(i) = laser_x(i) + r * cos(q);
          point_y(i) = laser_y(i) + r * sin(q);
          point_q(i) = q;
     end

     %// isolate points in "object on loose" box
     object_x = zeros(num_points, 1);
     object_y = zeros(num_points, 1);
     num_object_points = 0;
     for i = 1:num_points
          x = point_x(i);
          y = point_y(i);
          if ((1 < x) && (x < 3) && (1 < y) && (y < 2))
               num_object_points = num_object_points + 1;
               object_x(num_object_points) = x;
               object_y(num_object_points) = y;
          end
     end

     %// remove unnecessary array/matrix entries
     X = object_x(1:num_object_points);
     Y = object_y(1:num_object_points);
end

%// convert to Hough space ************************************************

%// compute coordinate average

sum_x = 0;
sum_y = 0;

for point = 1:num_object_points
     sum_x = sum_x + X(point);
     sum_y = sum_y + Y(point);
end
     
x_avg = sum_x / num_object_points;
y_avg = sum_y / num_object_points;

%// center data at origin
for i = 1:num_object_points
     X(i) = X(i) - x_avg;
     Y(i) = Y(i) - y_avg;
end

%// compute all Hough curves
num_angles = 181; %// from 0 to 180, inclusive
num_Hough_points = num_angles * num_object_points;
Hough_Points = zeros(num_Hough_points, 2);
min_R = 0;
max_R = 0;
for pt_idx = 1:num_object_points
     x = X(pt_idx);
     y = Y(pt_idx);
     for q_idx = 1:num_angles
          idx = num_angles * (pt_idx - 1) + q_idx;
          Q = (q_idx - 1) * (pi / 180);
          R = x * cos(Q) + y * sin(Q);
          if R > max_R
               max_R = R;
          elseif R < min_R
                    min_R = R;
          end               
          Hough_Points(idx, 1) = Q;
          Hough_Points(idx, 2) = R;
     end
end

%// rescale Hough curves
min_Q = 0;
max_Q = pi;
Q_scale = 100 / (max_Q - min_Q);
R_scale = 100 / (max_R - min_R);

for i = 1:num_Hough_points
	Hough_Points(i, 1) = (Hough_Points(i, 1) - min_Q) * Q_scale;
	Hough_Points(i, 2) = (Hough_Points(i, 2) - min_R) * R_scale;
end

%// initialize counts matrix
counts = zeros(101^2, 3);
idx = 1;
for row = 0:100
     for col = 0:100
          counts(idx, 1) = row;
          counts(idx, 2) = col;
          idx = idx + 1;
     end
end

%// count neighbors
window_length = 1.5;
for point = 1:num_Hough_points
    q = Hough_Points(point, 1);
    r = Hough_Points(point, 2);
    
    q_min = max(0,   ceil(q - window_length));
    q_max = min(100, floor(q + window_length));

    r_min = max(0,   ceil(r - window_length));
    r_max = min(100, floor(r + window_length));
    
    for i = q_min:q_max
         row_start = 101 * i + 1;
         for j = r_min:r_max
              row = row_start + j;
              counts(row, 3) = counts(row, 3) + 1;
         end
    end
end

%// sort counts matrix
sorted_counts = sortrows(counts, 3, 'descend');

min_dist = 15;
min_dist_sq = min_dist^2;

%// find 1st centroid
center1q = sorted_counts(1, 1);
center1r = sorted_counts(1, 2);
idx = 2;

%// find 2nd centroid
while true
	center2q = sorted_counts(idx, 1);
	center2r = sorted_counts(idx, 2);
	idx = idx + 1;
    
     dist_sq = (center2q - center1q)^2 + (center2r - center1r)^2;
	if dist_sq > min_dist_sq
        break
    end
end

%// find 3rd centroid
while true
	center3q = sorted_counts(idx, 1);
	center3r = sorted_counts(idx, 2);
	idx = idx + 1;
    
     dist1_sq = (center3q - center1q)^2 + (center3r - center1r)^2;
	dist2_sq = (center3q - center2q)^2 + (center3r - center2r)^2;
     dist_sq = min([dist1_sq, dist2_sq]);
	if dist_sq > min_dist_sq
        break
    end
end

%// find 4th centroid
while true
	center4q = sorted_counts(idx, 1);
	center4r = sorted_counts(idx, 2);
	idx = idx + 1;
    
     dist1_sq = (center4q - center1q)^2 + (center4r - center1r)^2;
	dist2_sq = (center4q - center2q)^2 + (center4r - center2r)^2;
	dist3_sq = (center4q - center3q)^2 + (center4r - center3r)^2;
     dist_sq = min([dist1_sq, dist2_sq, dist3_sq]);
	if dist_sq > min_dist_sq
        break
    end
end

%// find 5th centroid
while true
	center5q = sorted_counts(idx, 1);
	center5r = sorted_counts(idx, 2);
	idx = idx + 1;
    
     dist1_sq = (center5q - center1q)^2 + (center5r - center1r)^2;
	dist2_sq = (center5q - center2q)^2 + (center5r - center2r)^2;
	dist3_sq = (center5q - center3q)^2 + (center5r - center3r)^2;
	dist4_sq = (center5q - center4q)^2 + (center5r - center4r)^2;
     dist_sq = min([dist1_sq, dist2_sq, dist3_sq, dist4_sq]);
	if dist_sq > min_dist_sq
        break
    end
end

%// find 6th centroid
while true
	center6q = sorted_counts(idx, 1);
	center6r = sorted_counts(idx, 2);
	idx = idx + 1;
    
     dist1_sq = (center6q - center1q)^2 + (center6r - center1r)^2;
	dist2_sq = (center6q - center2q)^2 + (center6r - center2r)^2;
	dist3_sq = (center6q - center3q)^2 + (center6r - center3r)^2;
	dist4_sq = (center6q - center4q)^2 + (center6r - center4r)^2;
	dist5_sq = (center6q - center5q)^2 + (center6r - center5r)^2;
     dist_sq = min([dist1_sq, dist2_sq, dist3_sq, dist4_sq, dist5_sq]);
	if dist_sq > min_dist_sq
        break
    end
end

%// merge centers into a single array
centers = [center1q, center1r;
           center2q, center2r;
           center3q, center3r;
           center4q, center4r;
           center5q, center5r;
           center6q, center6r];
  
%// recenter based on centroids and total neighbor counts
avg_centers = zeros(6, 3);
for c = 1:6
    qc = centers(c, 1);
    rc = centers(c, 2);
    
    q1 = round(max(  0, qc - window_length));
    q2 = round(min(100, qc + window_length));
    
    r1 = round(max(  0, rc - window_length));
    r2 = round(min(100, rc + window_length));
    
    q_ttl = 0;
    r_ttl = 0;
    vote_ttl = 0;    
    for i = q1:q2
         row_start = 101 * i + 1;
         for j = r1:r2
               row = row_start + j;
               new_counts = counts(row, 3);
               q_ttl = q_ttl + new_counts * counts(row, 1);
               r_ttl = r_ttl + new_counts * counts(row, 2);              
               vote_ttl = vote_ttl + new_counts;
          end
    end    
    
    avg_centers(c, 1) = q_ttl / vote_ttl;
    avg_centers(c, 2) = r_ttl / vote_ttl;
    avg_centers(c, 3) = vote_ttl;
end

%// sort counts matrix
sorted_centers = sortrows(avg_centers, 3, 'descend');

%// convert back to unscaled and uncentered coords ************************
for i = 1:num_object_points
     X(i) = X(i) + x_avg;     
     Y(i) = Y(i) + y_avg;
end

% %// probably don't need this
% for i = 1:num_Hough_points
% 	Hough_Points(i, 1) = min_Q + Hough_Points(i, 1) / Q_scale;
% 	Hough_Points(i, 2) = min_R + Hough_Points(i, 2) / R_scale;
% end

angles = zeros(6, 1);
slopes = zeros(6, 1);
intercepts = zeros(6, 1);

for c=1:6
     %// decide which of these is best ***********************************
     centers(c, 1) = min_Q + centers(c, 1) / Q_scale;
     centers(c, 2) = min_R + centers(c, 2) / R_scale;
     
     avg_centers(c, 1) = min_Q + avg_centers(c, 1) / Q_scale;
     avg_centers(c, 2) = min_R + avg_centers(c, 2) / R_scale; 
     
     sorted_centers(c, 1) = min_Q + sorted_centers(c, 1) / Q_scale;
     sorted_centers(c, 2) = min_R + sorted_centers(c, 2) / R_scale;     

     if use_centers
     	q = centers(c, 1);
          r = centers(c, 2);
     elseif use_averaged
     	q = avg_centers(c, 1);
          r = avg_centers(c, 2);
     else
     	q = sorted_centers(c, 1);
          r = sorted_centers(c, 2);
     end   
     
     angles(c) = q;     
     slopes(c) = -cot(q);
     % points (x0, y0) are on the lines
     x0 = x_avg + r * cos(q);
     y0 = y_avg + r * sin(q);
     % y-intercepts of the lines
     intercepts(c) = y0 - slopes(c) * x0;   
end

%// find lines ********************************************

%// find first line
C = 1;
angle1 = angles(C);
slope1 = slopes(C);
intercept1 = intercepts(C);
next_C = C + 1;

%// deal with lines having "same" slope as current line
%// narrowest correct angle diff is atan(0.10/0.40) = 14 deg
%// so lines with angle diff less than 0.7 are same or parallel
%// note: line will "match" with itself
angle_diffs = zeros(6, 1);
has_matched = zeros(6, 1);
has_matched(C) = 1;
for c=next_C:6
	angle_diffs(c) = abs(angles(c) - angle1);
     if angle_diffs(c) > pi
          angle_diffs(c) = angle_diffs(c)  - pi;
     end
     angle_diffs(c) = 180/pi * min(angle_diffs(c), pi - angle_diffs(c));

	if (angle_diffs(c) <= 10)
          %// line with similar slope that are too close then "match"
          slope0 = slopes(c);
          intercept0 = intercepts(c);
          
          x_temp = (intercept0 - intercept1) / (slope1 - slope0);
          y_temp = (slope1*intercept0 - slope0*intercept1) / (slope1 - slope0);
          
          %// if they cross in box with similar slopes, then they match!
          if (((X_min - 0.25 < x_temp) && (x_temp < X_max + 0.25)) ...
                         && ((Y_min - 0.25 < y_temp) && (y_temp < Y_max + 0.25)))
               has_matched(c) = 1;
          end
     end
end

%// keep first matching line, which received the most votes,
%// and ignore any "matches" for further consideration

%// find second line
C = next_C;
while (has_matched(C) == 1)
     C = C + 1;
end
angle2 = angles(C);
slope2 = slopes(C);
intercept2 = intercepts(C);
next_C = C + 1;

%// deal with lines having "same" slope again
angle_diffs = zeros(6, 1);
has_matched(C) = 1;
for c=next_C:6
	angle_diffs(c) = abs(angles(c) - angle2);
     if angle_diffs(c) > pi
          angle_diffs(c) = angle_diffs(c)  - pi;
     end
     angle_diffs(c) = 180/pi * min(angle_diffs(c), pi - angle_diffs(c));

	if (angle_diffs(c) <= 10)
          %// line with similar slope that are too close then "match"
          slope0 = slopes(c);
          intercept0 = intercepts(c);
          
          x_temp = (intercept0 - intercept2) / (slope2 - slope0);
          y_temp = (slope2*intercept0 - slope0*intercept2) / (slope2 - slope0);
          
          %// if they cross in box with similar slopes, then they match!
          if (((X_min - 0.25 < x_temp) && (x_temp < X_max + 0.25)) ...
                         && ((Y_min - 0.25 < y_temp) && (y_temp < Y_max + 0.25)))
               has_matched(c) = 1;
          end
     end
end

%// find third line
C = next_C;
while (has_matched(C) == 1)
     C = C + 1;
end
angle3 = angles(C);
slope3 = slopes(C);
intercept3 = intercepts(C);
next_C = C + 1;

%// deal with lines having "same" slopes again
angle_diffs = zeros(6, 1);
has_matched(C) = 1;
for c=next_C:6
	angle_diffs(c) = abs(angles(c) - angle3);
     if angle_diffs(c) > pi
          angle_diffs(c) = angle_diffs(c)  - pi;
     end
     angle_diffs(c) = 180/pi * min(angle_diffs(c), pi - angle_diffs(c));

	if (angle_diffs(c) <= 10)
          %// line with similar slope that are too close then "match"
          slope0 = slopes(c);
          intercept0 = intercepts(c);
          
          x_temp = (intercept0 - intercept3) / (slope3 - slope0);
          y_temp = (slope3*intercept0 - slope0*intercept3) / (slope3 - slope0);
          
          %// if they cross in box with similar slopes, then they match!
          if (((X_min - 0.25 < x_temp) && (x_temp < X_max + 0.25)) ...
                         && ((Y_min - 0.25 < y_temp) && (y_temp < Y_max + 0.25)))
               has_matched(c) = 1;
          end
     end
end 

%// find fourth line
C = next_C;
while (has_matched(C) == 1)
     C = C + 1;
end
angle4 = angles(C);
slope4 = slopes(C);
intercept4 = intercepts(C);

%// see if fourth line is needed
%// if line 4 is needed, it should be parallel or perpindicular to others
angle_diff = abs(angle4 - angle1);
if angle_diff > pi
	angle_diff = angle_diff - pi;
end
if angle_diff > pi/2
	angle_diff = angle_diff - pi/2;
end
angle_diff = 180/pi * min(angle_diff, pi/2 - angle_diff);

found_rectangle = false;
if (angle_diff <= 10)
     angle_diff = abs(angle4 - angle2);
     if angle_diff > pi
          angle_diff = angle_diff - pi;
     end
     if angle_diff > pi/2
          angle_diff = angle_diff - pi/2;
     end
     angle_diff = 180/pi * min(angle_diff, pi/2 - angle_diff);
     if (angle_diff <= 10)
          found_rectangle = true;
     end
end
found_triangle = ~found_rectangle;

if found_triangle
     num_lines = 3;
else
     num_lines = 4;
end
num_corners = num_lines;

%// find boundary lines of object

kept_slopes = zeros(4, 1);
kept_slopes(1) = slope1;
kept_slopes(2) = slope2;
kept_slopes(3) = slope3;
kept_slopes(4) = slope4;

kept_intercepts = zeros(4, 1);
kept_intercepts(1) = intercept1;
kept_intercepts(2) = intercept2;
kept_intercepts(3) = intercept3;
kept_intercepts(4) = intercept4;

%// find corners of object (4 lines intersect at 6 points)
corners = zeros(6, 2);
corner = 1;
for M = 1:4
     m_M = kept_slopes(M);
     y0_M = kept_intercepts(M);
     for N = M+1:4
          m_N = kept_slopes(N);
          y0_N = kept_intercepts(N);
          corners(corner, 1) = (y0_M - y0_N) / (m_N - m_M);
          corners(corner, 2) = (m_N*y0_M - m_M*y0_N) / (m_N - m_M);
          corner = corner + 1;
     end
end

found_corners = 1;
if found_rectangle
     kept_corners = zeros(num_corners, 2);
     for corner = 1:6
          x_temp = corners(corner, 1);
          y_temp = corners(corner, 2);
          if (((X_min - 0.25 < x_temp) ...
                    && (x_temp < X_max + 0.25)) ...
                    && ((Y_min - 0.25 < y_temp) ...
                    && (y_temp < Y_max + 0.25)))
               kept_corners(found_corners, 1) = x_temp;
               kept_corners(found_corners, 2) = y_temp;
               found_corners = found_corners + 1;
          end
     end
end

if found_triangle
     kept_corners = zeros(num_corners, 2);
     
     %// first choice - where lines 1, 2, and 3 intersect
     kept_corners(1, 1) = corners(1, 1);
     kept_corners(1, 2) = corners(1, 2);
     kept_corners(2, 1) = corners(2, 1);
     kept_corners(2, 2) = corners(2, 2);
     kept_corners(3, 1) = corners(4, 1);
     kept_corners(3, 2) = corners(4, 2);
end

x1 = kept_corners(1, 1);
x2 = kept_corners(2, 1);
x3 = kept_corners(3, 1);

y1 = kept_corners(1, 2);
y2 = kept_corners(2, 2); 
y3 = kept_corners(3, 2);

if found_rectangle
     
     x4 = kept_corners(4, 1);
     y4 = kept_corners(4, 2);
     
     v12 = [x2 - x1, y2 - y1];
     v13 = [x3 - x1, y3 - y1];
     v14 = [x4 - x1, y4 - y1];
     
     L12 = sqrt(v12(1)^2 + v12(2)^2);
     L13 = sqrt(v13(1)^2 + v13(2)^2);
     L14 = sqrt(v14(1)^2 + v14(2)^2);
     
     if (((L13 > L12) && (L12 > L14)) || ((L14 > L12) && (L12 > L13)))
          hypotneuse = L12;
          guess_q = atan(v12(2) / v12(1));
     elseif (((L12 > L13) && (L13 > L14)) || ((L14 > L13) && (L13 > L12)))
          hypotneuse = L13;
          guess_q = atan(v13(2) / v13(1));
     else
          hypotneuse = L14;
          guess_q = atan(v14(2) / v14(1));
     end
     
     %// split the difference between the two known objects
     if hypotneuse > 0.35
          guess_num = 1;
     else
          guess_num = 2;
     end     
     
     %// center of rectangle is average of corners
     guess_x = (x1 + x2 + x3 + x4) / 4;
     guess_y = (y1 + y2 + y3 + y4) / 4;
     
     %// compromise between calculations and observations
     guess_x = (guess_x + x_avg) / 2;
     guess_y = (guess_y + y_avg) / 2;
     
     %// assume all angles are positive
     if guess_q < 0
          guess_q = guess_q + pi;
     end     
     
     %// since there is no way to know if rectangle is flipped
     guess_flipped = -1;  
     
end         


if found_triangle         

     L12 = sqrt((x1 - x2)^2 + (y1 - y2)^2);
     L13 = sqrt((x1 - x3)^2 + (y1 - y3)^2);
     L23 = sqrt((x2 - x3)^2 + (y2 - y3)^2);
     
     angle1 = acos((L12^2 + L13^2 - L23^2) / (2*L12*L13));
     angle2 = acos((L12^2 + L23^2 - L13^2) / (2*L12*L23));
     angle3 = acos((L13^2 + L23^2 - L12^2) / (2*L13*L23));
     
     %// triangle is not approx right, wrong side #3 was chosen
     lines_swapped = false;
     max_angle = max([angle1, angle2, angle3]);
     rightness = 100 * max_angle / (pi/2);
     if (abs(rightness - 100) > 20)
          %// second choice - where lines 1, 2, and 4 intersect
          lines_swapped = true;
          %fprintf('\n\n##########SWAPPING##########');
          temp = kept_slopes(3);
          kept_slopes(3) = kept_slopes(4);
          kept_slopes(4) = temp;

          temp = kept_intercepts(3);
          kept_intercepts(3) = kept_intercepts(4);
          kept_intercepts(4) = temp;          
          
          x2 = corners(3, 1);
          y2 = corners(3, 2);

          x3 = corners(5, 1);
          y3 = corners(5, 2);
          
          L12 = sqrt((x1 - x2)^2 + (y1 - y2)^2);
          L13 = sqrt((x1 - x3)^2 + (y1 - y3)^2);
          L23 = sqrt((x2 - x3)^2 + (y2 - y3)^2);
     end     
     
     if ((L12 > L13) && (L12 > L23))
     %// L12 is hypotenuse and opposite corner, #3, is right angle
     
          guess_x = x3;
          guess_y = y3;
          
          side_diff = abs(L13 - L23);
          
          if (L13 > L23)
          %// v31 is long vector and v32 is short vector 
          
               long_side  = [x1 - x3, y1 - y3];
               short_side = [x2 - x3, y2 - y3];
          else
          %// v32 is long vector and v31 is short vector 
          
               long_side  = [x2 - x3, y2 - y3];
               short_side = [x1 - x3, y1 - y3];               
          end          
          
     elseif ((L13 > L12) && (L13 > L23))
     %// L13 is hypotenuse and opposite corner, #2, is right angle
     
          guess_x = x2;
          guess_y = y2;
          
          side_diff = abs(L12 - L23);
          
          if (L12 > L23)
          %// v21 is long vector and v23 is short vector 
          
               long_side  = [x1 - x2, y1 - y2];
               short_side = [x3 - x2, y3 - y2];
          else
          %// v23 is long vector and v21 is short vector 
          
               long_side  = [x3 - x2, y3 - y2];  
               short_side = [x1 - x2, y1 - y2];            
          end  
          
     else
     %// L23 is hypotenuse and opposite corner, #1, is right angle
     
          guess_x = x1;
          guess_y = y1;
          
          side_diff = abs(L12 - L13);
          
          if (L12 > L13)
          %// v21 is long vector and v31 is short vector 
          
               long_side  = [x1 - x2, y1 - y2];
               short_side = [x1 - x3, y1 - y3];
          else
          %// v13 is long vector and v12 is short vector 
          
               long_side  = [x3 - x1, y3 - y1];  
               short_side = [x2 - x1, y2 - y1];            
          end   
     end
     
     %// assume the triangle was not flipped
     guess_flipped = 0; 
     
     %// compute z-coord of cross product and check right-hand rule
     if (long_side(1) * short_side(2) - long_side(2) * short_side(1) < 0)
          guess_flipped = 1;
     end
     
     del_x = long_side(1);
     del_y = long_side(2);
     
     guess_q = atan(del_y / del_x);
     if del_x < 0
          guess_q = guess_q + pi;
     elseif (del_y < 0)
          guess_q = guess_q + 2*pi;
     end     
     
     %// split the difference between the two known objects
     if side_diff > 0.225
          guess_num = 3;
     else
          guess_num = 4;
     end     
end   

if use_sim
     %\\ Need only for displaying simulation score
     total_score = 0;
     if (obj_num == guess_num)
          total_score = total_score + 2;
          if (((obj_num == 1) && (guess_num == 2)) ...
                || ((obj_num == 2) && (guess_num == 1)) ...
                || ((obj_num == 3) && (guess_num == 4)) ...
                || ((obj_num == 4) && (guess_num == 3)))

               total_score = total_score + 1;
          end
     end

     if (abs(obj_x - guess_x) < 0.10)
          total_score = total_score + 2;
     elseif (abs(obj_x - guess_x) < 0.20)
          total_score = total_score + 1;  
     end

     if (abs(obj_y - guess_y) < 0.10)
          total_score = total_score + 2;
     elseif (abs(obj_y - guess_y) < 0.20)
          total_score = total_score + 1;  
     end

     if (abs(obj_q - guess_q) < 0.10)
          total_score = total_score + 2;
     elseif (abs(obj_q - guess_q) < 0.20)
          total_score = total_score + 1;  
     elseif (found_rectangle && ((obj_num == 1) || (obj_num == 2)))
          guess_q = guess_q + pi;
          if (abs(obj_q - guess_q) < 0.10)
               total_score = total_score + 2;
          elseif (abs(obj_q - guess_q) < 0.20)
               total_score = total_score + 1; 
          end
     end

     mystring = 'Object ID Num: True=%d \t\tGuessed=%d\n';
     fprintf(mystring, obj_num, guess_num)

     mystring = 'Pose x-coord:  True=%4.3f \tGuessed=%4.3f\n';
     fprintf(mystring, obj_x, guess_x)

     mystring = 'Pose y-coord:  True=%4.3f \tGuessed=%4.3f\n';
     fprintf(mystring, obj_y, guess_y)

     mystring = 'Pose q-coord:  True=%4.3f \tGuessed=%4.3f\n';
     fprintf(mystring, obj_q, guess_q)

     mystring = 'Was Flipped:   True=%d \t\tGuessed=%d\n';
     fprintf(mystring, is_flipped, guess_flipped)

     disp('********************************************************');
     mystring = 'Total Score: %d points out of 8 possible, or %d percent!\n';
     fprintf(mystring, total_score, round(total_score/8*100))

     
     %\\ Plot results
     T = linspace(0, 4);
     Y1 = slope1 * T + intercept1;
     Y2 = slope2 * T + intercept2;
     Y3 = slope3 * T + intercept3;
     Y4 = slope4 * T + intercept4;

     figure(2)
     plot(T, Y1, 'b', 'LineWidth', 2)      %// found line #1
     hold on
     plot(T, Y2, 'c', 'LineWidth', 2)      %// found line #2
     plot(T, Y3, 'm', 'LineWidth', 2)      %// found line #3
     if (found_rectangle || lines_swapped)
          plot(T, Y4, 'y', 'LineWidth', 2) %// found line #4, if needed
     end
     plot(X, Y, '+', 'MarkerEdgeColor', [0.8 0.8 0.8]) %// laser data
     plot(obj_x, obj_y, 'or', 'MarkerFaceColor', 'red', 'MarkerSize', 12)
     plot(guess_x, guess_y, '+g', 'MarkerSize', 12, 'LineWidth', 4)
     xlim([guess_x-0.5, guess_x+0.5])
     ylim([guess_y-0.5, guess_y+0.5])
     if (found_rectangle || lines_swapped)
          plot(T, Y4, 'y', 'LineWidth', 2)
          legend({'Extracted Line 1', 'Extracted Line 2', 'Extracted Line 3', ...
               'Extracted Line 4', 'Laser Data', 'Real Center', 'Guessed Center'}, ...
               'Location', 'southoutside')
     else
          legend({'Extracted Line 1', 'Extracted Line 2', 'Extracted Line 3', ...
          'Laser Data', 'Real Center', 'Guessed Center'}, ...
          'Location', 'southoutside')
     end
     title('Results of Locate and Identify Algorithm')
     hold off
end

trial_score = total_score;

end