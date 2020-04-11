clear all

% initial the map
map=zeros(10,10);
map(1:7,3)=ones(7,1);
map(3:10)=ones(8,1);

% give goal and start
goal=[2,2];
start=[9,9];
map(goal(1),goal(2))=2;

% make the map
[map,status] = makewave(goal,start,map);

if status==true
    % find the route
    route = findroute(goal,start,map);
else
    error("Can't find the route");
end