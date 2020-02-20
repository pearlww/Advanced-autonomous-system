function newpose=kinupdate(robotpar,ts, wheelspeed)
    global sim_step
    global posehist
    global pose
    speed_R=invkin(robotpar, wheelspeed, pose);
    newpose=pose + speed_R*ts;
    posehist =[posehist;newpose];
    sim_step=sim_step+1;
    pose=newpose;
end