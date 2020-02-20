% function forward(distance,speed)
%     global robotpar
%     global pose
%     global ts 
%     
%     wheelspeed = [speed/robotpar(2),speed/robotpar(3)];
%     if distance<0
%         wheelspeed=-wheelspeed;
%     end    
%     
%     move_distance=0;
%         while abs(move_distance)<abs(distance)      
%             move_distance = move_distance+speed*ts;
%             newpose=kinupdate(pose,robotpar,ts, wheelspeed);
%             pose=newpose;
%         end
% end

function forward(distance,speed)
    global robotpar
    global pose
    global ts 
    
    wheelspeed = [speed/robotpar(2),speed/robotpar(3)];
    if distance<0
        wheelspeed=-wheelspeed;
    end    
    
    move_distance=0;
    lastpose=pose;
        while abs(move_distance)<abs(distance)
            newpose=kinupdate(robotpar,ts, wheelspeed);
            newmove=newpose-lastpose;
            move_distance = sqrt(newmove(1)^2+newmove(2)^2);        
        end
end