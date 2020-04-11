function nb_points = find_neighbours(cp,map)
%     return the neighbour of cp using 8-point connectivity
%     Output contain both the position and distance
%%
[r,c]=size(map);
nb_points=[];

for i=(-1:1)
    for j=(-1:1)
        if i==0 && j==0 % point itself
            continue
        end
        if cp(1)+i>0 && cp(1)+i<=r && cp(2)+j>0 && cp(2)+j<=c % inside the map
            if map(cp(1)+i,cp(2)+j)~= 1 % not walls
                if i~=0 && j~=0 % diagnal neighbour
                    dist=1.41;
                else
                    dist=1;
                end
                p=[cp(1)+i,cp(2)+j,dist];
                nb_points=[nb_points;p];
            end
                        
        end
    end
end

end