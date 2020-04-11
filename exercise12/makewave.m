function [map,status] = makewave(goal,start,map)
    queue= Queue();
    queue.push(goal);
    status=false;
    
    while ~queue.empty() 
        cp=queue.pop();
        if cp==start % reach start point
            status=true;
            break
        end
            
        nb_points=find_neighbours(cp,map);
        
        for i=1:size(nb_points,1)
            nbi=nb_points(i,1:2);
            dist=nb_points(i,3);
            
            d=map(cp(1),cp(2))+dist;
            if map(nbi(1),nbi(2))==0 % a new frontier
                map(nbi(1),nbi(2))=d;
                queue.push(nbi)
            else
                if map(nbi(1),nbi(2))>d % the nearest route
                    map(nbi(1),nbi(2))=d;
                end
            end
        end
        
    end
        
end