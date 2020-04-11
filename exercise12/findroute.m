function route = findroute(goal,start,map)
cp=start;
route=cp;
min_dist=99999;
[r,c]=size(map);

while (true)
    if cp==goal % goal
        break
    end
    % loop all neighbours and find the closest one
    for i=(-1:1:1)
        for j=(-1:1:1)
            if i==0 && j==0 % point itself
                continue
            end
            if cp(1)+i>0 && cp(1)+i<r && cp(2)+j>0 && cp(2)+j<c % inside the map
                nb=[cp(1)+i,cp(2)+j];
                if map(nb(1),nb(2))~= 1 % not walls                   
                    dist=map(nb(1),nb(2));
                    if dist<min_dist
                        min_dist= dist;
                        selected_nb=nb;
                    end
                end
            end
        end
    end
    
    cp=selected_nb;
    route=[route;cp];
end

route=[route;goal];
end