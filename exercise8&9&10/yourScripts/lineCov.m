function [covPre,H_jacob]=lineCov(worldline,pose,poseCov)

    global lsrRelPose
    
    w1=worldline(1);
    theta=pose(3);
    xlr=lsrRelPose(1);
    ylr=lsrRelPose(2);
    H_jacob=[       0,       0,                                  -1;
             -cos(w1), -sin(w1), xlr*sin(theta-w1)+ylr*cos(theta-w1)];
   
    covPre= H_jacob*poseCov*H_jacob';
end