% function turn(angle,speed)
%     global robotpar
%     global pose
%     global ts 
%     
%     wheelspeed = [-speed/robotpar(2),speed/robotpar(3)];
%     if angle<0
%         wheelspeed = - wheelspeed;
%     end    
%     %a_rad=angle/180*pi;
% 
%     turn_angle=0;
%         while abs(turn_angle)<abs(angle)            
%             turn_angle =turn_angle+ speed/robotpar(2)*ts;
%             newpose = kinupdate(pose,robotpar,ts, wheelspeed);
%             pose=newpose;            
%         end
% end

function turn(angle,speed)
    global robotpar
    global pose
    global ts 
    
    wheelspeed = [-speed/robotpar(2),speed/robotpar(3)];
    if angle<0
        wheelspeed = - wheelspeed;
    end    
    lastpose=pose;
    turn_angle=0;
        while abs(turn_angle)<abs(angle)
            newpose = kinupdate(robotpar,ts, wheelspeed);
            newmove = newpose-lastpose;
            turn_angle = newmove(3);  
        end
end