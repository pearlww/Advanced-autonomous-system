function scan=polar2carth(pol)
    [~,c]=size(pol);
    scan(1:2,1:c) = 0;
    for i= 1:c
        rho=pol(2,i);
        theta=pol(1,i);
        x=rho*cos(theta);
        y=rho*sin(theta);
        scan(1,i) = x;
        scan(2,i) = y;
    end
end