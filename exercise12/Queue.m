classdef Queue < handle  
    properties ( Access = public )
        elems = [];
        first = 0;
        last = 0;
    end
    
    methods
        function this = Queue()
        end 
        
        function push(this,new_elem)
            this.elems = [this.elems;new_elem];
            this.last = this.last+1;
        end 
        
        function ret = empty(this)
            ret = (this.last-this.first == 0);
        end 
        
        function elem = pop(this)
            if this.empty()
                error('Empty Queue');
            end         
            elem = this.elems(this.first+1,:);
            this.elems(this.first+1,:)=[];
            this.last = this.last-1;
        end
        
    end
    
end