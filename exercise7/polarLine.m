function points = polarLine(line)
    alpha=line(1);
    r=line(2);
    delta_alpha = pi/4;
%     X = r*cos(alpha) - [-1000,1000]*sin(alpha);
%     Y = r*sin(alpha) + [-1000,1000]*cos(alpha);
%     [A,R]=cart2pol(X,Y);

    A=[alpha; alpha+delta_alpha;alpha-delta_alpha];
    R=[r;r/cos(delta_alpha);r/cos(-delta_alpha)];
    points=[A,R];
end