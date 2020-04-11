clear all;
close all;

scannerpose1=[0,0,0];
scannerpose2=[1,0,0];
scannerpose3=[1.5,0,pi/6];
scannerpose=[scannerpose1;scannerpose2;scannerpose3];

maxDistance=4;
resol=0.36;
field_of_view=180;

line1=[0;1;2.5;1];
line2=[2.5;1;2.5;5];
line3=[0;-1;3.5;-1];
line4=[3.5;-1;3.5;5];
%lines=[3;-4;3;1];
lines=[line1,line2,line3,line4];

for i = 1:3
    
    % polar coordinate
    scan_l=laserscanHighresol(scannerpose(i,1),scannerpose(i,2),scannerpose(i,3), lines, maxDistance, resol,field_of_view);
    scan_l=scan_l';
    %     figure(1)
    %     polarplot(scan_l(:,1),scan_l(:,2),'.')
    
    % carthesian coordinate
    scan_l_cart=polar2carth(scan_l);
    %     figure(2)
    %     plot(scan_l_cart(:,1),scan_l_cart(:,2),'.')
    
    figure(3)
    hold on
    pose_w=transform(scannerpose(i,:),scan_l_cart);
    plot(pose_w(:,1),pose_w(:,2),'.')
end

%lsq estimation in scanner frame
estimated_line = lsqline([1,-1;2,-1;3,-1]);%line3
line_points = polarLine(estimated_line);
figure
polarplot(line_points(:,1),line_points(:,2));

%transform the estimated line to world frame
line_points_cart=polar2carth(line_points);
estimated_line_w=transform(scannerpose(1,:),line_points_cart);
figure
plot(estimated_line_w(:,1),estimated_line_w(:,2))



