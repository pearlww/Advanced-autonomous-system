function [ projectedLine, linecov ] = projectToLaser( worldLine,poseIn, covIn)
%[projectedLine, lineCov] = PROJECTTOLASER(worldLine,poseIn,covIn) 
%Project a word line to the laser scanner frame given the
%world line, the robot pose and robot pose covariance. Note that the laser
%scanner pose in the robot frame is read globally
%   worldLine: The line in world coordinates
%   poseIn: The robot pose
%   covIn: The robot pose covariance
%
%   projectedLine: The line parameters in the laser scanner frame
%   lineCov: The covariance of the line parameters

%% Constants
global lsrRelPose % The laser scanner pose in the robot frame is read globally

%% Calculation

% measurenent prediction
% z=H*x
% z=[alpha2;rho2]
% x=[x,y,theta]=poseIn
%% method 1
% % The laser scanner pose in the world frame
% theta=poseIn(3); 
% R=[cos(theta),-sin(theta),0;
%     sin(theta),cos(theta),0;
%     0,0,1];
% t=poseIn;
% laser_w = R*lsrRelPose'+t;
% 
% % the line in world frame to laser frame
% alpha2=worldLine(1)-laser_w(3);
% rho2=worldLine(2)-laser_w(1)*cos(worldLine(1))-laser_w(2)*sin(worldLine(1));

%% method 2
alpha2=worldLine(1)-poseIn(3)-lsrRelPose(3);
rho2=worldLine(2)...
     -poseIn(1)*cos(worldLine(1)) - poseIn(2)*sin(worldLine(1))...
     -lsrRelPose(1)*cos(worldLine(1)-poseIn(3)) - lsrRelPose(2)*sin(worldLine(1)-poseIn(3));   
%% why this is not correct?
% % the line in world frame to laser frame
% [x,y]=pol2cart(worldLine(1),worldLine(2));
% theta=laser_w(3); 
% R=[cos(theta),-sin(theta);sin(theta),cos(theta)]';
% t=-laser_w(1:2);   
% line_l_cart=R*[x,y]'+t;

%%
% thansfer the domain to -pi~pi
if alpha2>pi
    alpha2=alpha2-2*pi;
elseif alpha2<-pi
    alpha2=alpha2+2*pi;
end

projectedLine = [alpha2;rho2];
[linecov,~] = lineCov(worldLine,poseIn,covIn);

end
