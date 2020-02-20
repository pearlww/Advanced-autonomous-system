function sta = MotionControl1(currentpose, targetpose)
    global pose
    inputpose = targetpose-currentpose;
    d=sqrt(inputpose(1)^2+inputpose(2)^2);
    a= atan2(inputpose(2),inputpose(1));
    turn(a-currentpose(3),0.1);
    forward(d,0.1);
    turn(targetpose(3)-a,0.1);
    
    sta = targetpose-pose;
end