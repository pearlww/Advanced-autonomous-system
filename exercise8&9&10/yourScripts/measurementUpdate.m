% error at the third corner

function [ poseOut, poseCovOut ] = measurementUpdate( poseIn, poseCovIn, matchResult )
%[ poseOut, poseCovOut ] =MEASUREMENTUPDATE ( poseIn, poseCovIn,
%matchResult ) perform extended Kalman filter measurement update on the
%estimated robot pose poseIn with covariance poseCovIn using a set of
%matched predicted and extracted laser scanner lines given in matchResult.
%The arguments are defined as:
%       poseIn: The estimated robot pose given as [x,y,theta]
%       poseCovIn: The estimated covariance matrix of the robot pose
%       matchResult: A (5xnoOfWorldLines) matrix whose columns are 
%       individual pairs of line matches. It is structured as follows:
%       matchResult = [ worldLine(1,1) , worldLine(1,2) ...  ]
%                     [ worldLine(2,1) , worldLine(2,2)      ]
%                     [ innovation1(1) , innovation2(1)      ]
%                     [ innovation1(2) , innovation2(2)      ]
%                     [ matchIndex1    , matchIndex2    ...  ]
%           Note that the worldLines are in the world coordinates!
%       
%       poseOut: The updated robot pose estimate
%       poseCovOut: The updated estimate of the robot pose covariance 
%       matrix 

    % Constants
    % The laser scanner pose in the robot frame is read globally(lsrRelpose)
    % The varAlpha and varR are the assumed variances of the parameters of
    % the extracted lines, they are also read globally
    global  varAlpha varR
    
    noOfMatchResult=size(matchResult,2);
    compInnov=[];
    compH=[];
    compR=[];
    covObs = diag([varAlpha varR]);
    
    for i=1:noOfMatchResult
        if matchResult(5,i)~=0
            compInnov= [compInnov; matchResult(3:4,i)];
            [~,H_jacob]=lineCov(matchResult(1:2,i),poseIn,poseCovIn);
            compH=[compH; H_jacob];
            compR=blkdiag(compR,covObs);
        end
    end
    
%     compH
    if size(compH,1) ~= 0
        CovIN= compH*poseCovIn*compH' + compR;
        K=poseCovIn*compH'*inv(CovIN);

        poseOut = poseIn + K*compInnov;
        poseCovOut = poseCovIn - K*CovIN*K';
    else % doesn't observe angthing
        poseOut = poseIn;
        poseCovOut = poseCovIn;
    end

    
%         poseOut = poseIn;
%         poseCovOut = poseCovIn;    
end
