function wheelspeed = kin(robotpar,robotspeed,pose)
%The differential drive kinematic
     % three wheels
    syms alpha1 beta1 l1 alpha2 beta2 l2 
    
    alpha_1 = pi/2;
    beta_1=0;
    l_1= robotpar(1)/2;
    para1 = [alpha_1 beta_1 l_1];
    
    alpha_2=pi/2 ;
    beta_2=0;
    l_2= -robotpar(1)/2;
    para2 = [alpha_2 beta_2 l_2];
    
    % constraints of left standard wheel
    v_l1=rol_cons_vec(para1);
    v_l2=sli_cons_vec(para1);
    
    % constraints of right standard wheel
    v_r1=rol_cons_vec(para2);
    v_r2=sli_cons_vec(para2);
    
    % for two wheel differential drive robot, 
    m1=[v_l1;v_r1;v_l2];
    
    % for two wheel differential drive robot, 
    m2=m1*Rot(pose(3))*robotspeed;
    wheelspeed=[m2(1)/robotpar(2);m2(2)/robotpar(3)];
    wheelspeed=wheelspeed';
end