function transform = findTransform(odoPose, pose)
% transform = FINDTRANSFORM(odoPose,pose)
% Find the transformation from the world coordinates to the odometry
% coordinates given a pose in the odometry coordinates (odoPose) and the
% same point in the world coordinates (pose). The output (transform) is
% simply the origo of the odometry coordinates in the world coordinates
    theta_t=odoPose(3)-pose(3);
    R=rotz(theta_t);
    transform= odoPose - R*pose;
%     transform = [0;0;0];
end