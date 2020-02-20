function sta =MotionControl2(currentpose, targetpose)
    global robotpar
    global ts
    global sim_step
    
    K_rho=0.3;
    K_alpha=0.8;
    K_beta=-0.15;
    
    max_sim_step=300;
    sim_step=0;
    
    while(1)
        inputpose = targetpose-currentpose;
        rho=sqrt(inputpose(1)^2+inputpose(2)^2);
        alpha= atan2(inputpose(2),inputpose(1)) - currentpose(3);
        beta= - (currentpose(3)+alpha);

        v=K_rho*rho;
        w=K_alpha*alpha+K_beta*beta;
        robotspeed=[v*cos(currentpose(3));v*sin(currentpose(3));w];

        wheelspeed = kin(robotpar,robotspeed,currentpose);
        currentpose=kinupdate(robotpar,ts, wheelspeed);
        
        sta = targetpose- currentpose;
        d_error=sqrt(sta(1)^2+sta(2)^2);
        a_error=sta(3);
        
        if(d_error<0.05 && a_error<0.05)
            break
        end 
        if sim_step>max_sim_step
            break
        end    
    end
end