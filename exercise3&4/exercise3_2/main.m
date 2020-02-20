clear all;
close all;

%----------------------  parameters ----------------------------%
% parameters of robot
syms w r_1 r_2
w= 0.26 ;
r_1= 0.035 ;
r_2=0.035;

global robotpar
robotpar=[w,r_1,r_2];

syms omega_1 omega_2
omega_1=5.714 ;
omega_2=5.714 ;

global wheelspeed
wheelspeed=[omega_1,omega_2];

% the angle between inersial coordinate and robot coordinate
syms x y theta
x=0;
y=0;
theta=0;

global pose
pose=[x,y,theta];

global posehist
posehist = pose;

%simulation 
global ts 
ts=0.05;
global sim_step
sim_step=0;
global sim_time
sim_time=5;

%---------------------  test ----------------------------------%
%test1
%newpose=kinupdate(pose,robotpar,ts, wheelspeed);

%test 2 draw a rectangle
% forward(0.5,5)
% turn(90,1)
% forward(0.5,5)
% turn(-90,1)
% forward(-0.5,5)
% turn(-90,1)
% forward(0.5,5)
% plot(posehist(:,1),posehist(:,2))

%test 3

targetpose =[0.5,0,0; 0.5,0.5,0; 0.5,-0.5,-pi/2];

for i= 1:3  
    currentpose=pose;
    status = MotionControl2(currentpose, targetpose(i,:));
end

plot(targetpose(:,1),targetpose(:,2),'ro')
hold on
plot(posehist(:,1),posehist(:,2))
xlabel('pose_x','Fontsize',14)
ylabel('pose_y','Fontsize',14)
grid on
