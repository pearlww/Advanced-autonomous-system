clear all;
close all;

x = 1:100;
y = -x +3 + randn(1,100);
[alpha,r]=cart2pol(x,y);
estimated_line = lsqline([alpha,r]);

plot(x,y,'.')
figure
polarplot(alpha,r,'.')
polarLine(estimated_line)
