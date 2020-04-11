function [ matchResult ] = match( pose, poseCov, worldLines, laserLines )
% [matchResult] = MATCH(pose,poseCov,worldLines,laserLines)
%   This function matches the predicted lines to the extracted lines. The
%   criterion for a match is the mahalanobis distance between the (alpha,
%   r) parameters of the predicted and the extracted lines. The arguments
%   are:
%       pose: The estimated robot pose given as [x,y,theta]
%       poseCov: The estimated covariance matrix of the robot pose
%       worldLines: Known world lines in world coordinates, given as
%       [alpha;r] for each line. Number of rows = number of lines
%       laserLines: Lines extracted from the laser scan. Given as [alpha;r]
%       for each line. Number of rows = number of lines
%
%       matchResult: A (5xnoOfWorldLines) matrix whose columns are 
%       individual pairs of line matches. It is structured as follows:
%       matchResult = [ worldLine(1,1) , worldLine(1,2) ...  ]
%                     [ worldLine(2,1) , worldLine(2,2)      ]
%                     [ innovation1(1) , innovation2(1)      ]
%                     [ innovation1(2) , innovation2(2)      ]
%                     [ matchIndex1    , matchIndex2    ...  ]
%           Note that the worldLines are in the world coordinates!

    % The varAlpha and varR are the assumed variances of the parameters of
    % the extracted lines, they are read globally.
    global varAlpha varR
    
    noOfLaserLines=size(laserLines,2);
    noOfWorldLines=size(laserLines,2);
    g=2;
    covObs = diag([varAlpha varR]);
    matchResult=zeros(5,noOfWorldLines);
    
    for i=1:noOfWorldLines
        index=0;
        min_dis=99999;
        min_innov=[999;999];% not sure what to do here
        [predictedLine, covPre ] = projectToLaser( worldLines(:,i),pose,poseCov);
        covIn = covPre+covObs;
        for j=1:noOfLaserLines
            innov = laserLines(:,j)-predictedLine;
            maha_dis = innov'*inv(covIn)*innov;
            if maha_dis<=g^2 && maha_dis<min_dis
                %successful match
                index=j;
                min_dis=maha_dis;
                min_innov=innov;
            end
        end
        matchedLine=[worldLines(1,i);worldLines(2,i);min_innov(1);min_innov(2);index];
        matchResult(:,i)=matchedLine;
    end
    
end
