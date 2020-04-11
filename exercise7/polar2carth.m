function scan=polar2carth(pol)
    [x,y]=pol2cart(pol(:,1),pol(:,2));
    scan=[x,y];
end