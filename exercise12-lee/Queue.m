classdef Queue < handle
    properties ( Access = private ) % Attribute of class members
        elements
        nextInsert
        nextRemove
    end

    properties ( Dependent = true )
        NumElements
    end

    methods
        function obj = Queue()
            obj.elements = zeros(2,1);
            obj.nextInsert = 1;
            obj.nextRemove = 1;
        end
        function add( obj, el )
            obj.elements(:,obj.nextInsert) = el;
            obj.nextInsert = obj.nextInsert + 1;
        end
        function el = remove( obj )
            if obj.isEmpty()
                error( 'Queue is empty' );
            end
            el = obj.elements(:,obj.nextRemove);
            obj.nextRemove = obj.nextRemove + 1;
        end
        function tf = isEmpty( obj )
            tf = ( obj.nextRemove >= obj.nextInsert );
            % Because element to be removed is always current one
        end
        function n = getNumElements( obj )
            n = obj.nextInsert - obj.nextRemove;
            % FIFO, first in and first out
        end
    end
end