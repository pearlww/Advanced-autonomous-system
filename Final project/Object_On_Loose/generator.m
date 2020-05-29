function generator(need_debug, mu, sigma, point_density)

     % resolution and frequency of laserscanner
     % mu = 0 assumes scanner is properly calibrated
     % sigma = 0.01 assumes a typical deviation is 1 cm
     % point_density = 100 asssumes 100 samples per m

     X_min = 1;
     X_max = 3;
     Y_min = 1;
     Y_max = 2;

     rng shuffle
     
     obj_num = randi(4);

     if (obj_num < 3)
         is_rectangle = true;
         num_sides = 4;
         is_flipped = -1;

         if (obj_num == 1)
             side_length1 = 0.15;
             side_length2 = 0.40;        
         else
             side_length1 = 0.20;
             side_length2 = 0.30;        
         end

         L1 = 0.5 * side_length1;
         L2 = 0.5 * side_length2;

         cornersX = [-L2, -L2,  L2,  L2];
         cornersY = [ L1, -L1, -L1,  L1];
         side_lengths = [side_length1, side_length2, side_length1, side_length2]; 
         perimeter = 2 * (side_length1 + side_length2);

     else
         is_rectangle = false;
         num_sides = 3;

         if (obj_num == 3)
             side_length1 = 0.10;
             side_length2 = 0.40;       
         else
             side_length1 = 0.15;
             side_length2 = 0.30;      
         end

         cornersX = [0, 0, side_length2, 0];
         cornersY = [side_length1, 0, 0, 0]; 
         
         head_or_tails = rand();
         if head_or_tails < 0.5 % flip triangle over
             is_flipped = 1;
             cornersY(1) = -cornersY(1);
         else
             is_flipped = 0;
         end           
         
         hypotenuse = sqrt(side_length1 ^ 2 + side_length2 ^ 2);
         side_lengths = [side_length1, side_length2, hypotenuse, 0]; 
         perimeter = side_length1 + side_length2 + hypotenuse;

     end

     is_triangle = ~is_rectangle;
 
     new_cornersX = zeros(1, 4);
     new_cornersY = zeros(1, 4);

     obj_fits = false;
     while ~obj_fits
         obj_fits = true; 
         x0  = X_min + (X_max - X_min) * rand();
         y0  = Y_min + (Y_max - Y_min) * rand();
         
         theta = pi * rand();
         %// rectangles are symmetric modulo 180 degrees
         if is_triangle
             theta = 2 * theta;
         end

         C = cos(theta);
         S = sin(theta);

         for c = 1:num_sides
             x_old = cornersX(c);
             y_old = cornersY(c);

             new_x = x0 + (C * x_old - S * y_old);
             new_y = y0 + (S * x_old + C * y_old);

             if ((new_x < X_min) || (X_max < new_x))
                 obj_fits = false;
                 break
             end

             if ((new_y < Y_min) || (Y_max < new_y))
                 obj_fits = false;
                 break
             end

             new_cornersX(c) = new_x; 
             new_cornersY(c) = new_y;

         end
     end

     center = [x0, y0];

     ttl_points = round(perimeter * point_density);
     x_vals = zeros(ttl_points, 1);
     y_vals = zeros(ttl_points, 1);

     current_point = 1;
     for side = 1:num_sides

         if side == num_sides % final point is also first point!
             next_side = 1;
         else
             next_side = side + 1;
         end

         x1 = new_cornersX(side);
         y1 = new_cornersY(side);

         x2 = new_cornersX(next_side);
         y2 = new_cornersY(next_side);  

         side_length = side_lengths(side);
         side_points = round(side_length * point_density);

         del_x = (x2 - x1) / side_points;
         del_y = (y2 - y1) / side_points;

         for point = 1:side_points
             r = mu + sigma * randn();
             q = (2 * pi) * rand();
             x_vals(current_point) = x1 + del_x * point + r * cos(q);
             y_vals(current_point) = y1 + del_y * point + r * sin(q);

             current_point = current_point + 1;
         end
     end

     data = [x_vals, y_vals];
     save('object_points.txt','data','-ascii')

     answer = [obj_num, center(1), center(2), theta, is_flipped];
     save('answers.txt','answer','-ascii')


     if (need_debug)
          figure(1)   
          plot([1, 3, 3, 1, 1], [1, 1, 2, 2, 1], '-', ...
               'LineWidth', 2, 'Color', [0.75, 0.75, 0.75])
          hold on
          plot(x_vals, y_vals, '+r')
          plot(center(1), center(2), 'og', ...
               'MarkerFaceColor', 'green', 'MarkerSize', 12)          
          xlim([0.75, 3.25])
          ylim([0.75, 2.25])
          title('Object (red) with Center (green) in Box (gray)')
          hold off          

     end
end

