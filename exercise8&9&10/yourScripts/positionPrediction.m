function [ poseOut, covOut ] = positionPrediction( poseIn,covIn,delSr,delSl)
%[poseOut, covOut] = POSITIONPREDICTION(poseIn,covIn,delSr,delSl) perform
%one step of robot pose prediction from a set of wheel displacements
%   poseIn = old robot pose
%   covIn = uncertainty on the old robot pose
%   delSr = right wheel linear displacement
%   delSl = left wheel linear displacement


%% Constants
% The robot parameters are read globally, odoB is the wheel separation, kR
% and kL are the odometry uncertainty parameters
global odoB kR kL 

%% pose update
delta_s = (delSr+delSl)/2;
delta_theta = (delSr-delSl)/odoB;
cosp=cos(poseIn(3)+delta_theta/2);
sinp=sin(poseIn(3)+delta_theta/2);
m=[ delta_s*cosp; 
    delta_s*sinp; 
    delta_theta];
poseOut = poseIn+m;

%% thansfer the theta domain to -pi~pi
% % why this introduce more accumulative error?

% if poseOut(3)>pi
%     poseOut(3)=poseOut(3)-2*pi;
% elseif poseOut(3)<-pi
%     poseOut(3)=poseOut(3)+2*pi;
% end

%% Covariance update
Q = [kR*abs(delSr),0; 
     0, kL*abs(delSl)];
dfdx=[1,0,0];
dfdy=[0,1,0];
dfdt=[-delta_s*sinp, -delta_s*cosp, 1];

Fp = [dfdx; dfdy; dfdt]';
Fu = [1/2*cosp - delta_theta/2*sinp, 1/2*cosp + delta_theta/2*sinp;
      1/2*sinp + delta_theta/2*cosp, 1/2*sinp-delta_theta/2*cosp;
      1/odoB,                        -1/odoB];
covOut = Fp*covIn*Fp' + Fu*Q*Fu';

end
