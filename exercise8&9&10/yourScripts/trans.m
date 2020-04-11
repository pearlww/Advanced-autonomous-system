function odoTargetPose = trans(transform,targetPose)
% odoTargetPose = trans(transform,targetPose)
% Transform a given point in world coordinates (targetPose) to odometry
% coordinates, using the origo of the odometry coordinates in world
% coordinates (transform).
    theta_t=transform(3);
    R=rotz(theta_t);
    odoTargetPose = R* targetPose+transform;
%     odoTargetPose = [0;0;0];
end