function line = lsqLine(points)
% The function extract a line described in (alpha,r)
    
%input is a set of cartesian coordinate points 
%output is a line described in polar format (alpha,r) 

x=points(1,:);
y=points(2,:);
[~,n]=size(points);

sum_x = sum(x);
sum_y = sum(y);
sum_xs=x*x';
sum_ys=y*y';
sum_xy=x*y';

N=2*sum_x*sum_y-2*n*sum_xy;
D=sum_x^2-sum_y^2-n*sum_xs+n*sum_ys;

alpha = atan2(N,D)/2;
r = mean(x)*cos(alpha)+mean(y)*sin(alpha);

% to keep r is positive and alpha in the interval [-pi,pi].not necessary
if r<0
    r=-r;
    alpha=alpha-pi;
end

line=[alpha,r];
end