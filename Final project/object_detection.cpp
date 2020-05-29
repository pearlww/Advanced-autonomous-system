#include "object_detection.h"


vector<double> ObjectDetection::transform(vector<double> poseW, vector<double> cart){
	vector<double> cart_trans(2);
	double theta = poseW[2];

	cart_trans[0] = cos(theta)*cart[0] + sin(theta)*cart[1] + poseW[0];
	cart_trans[1] = -sin(theta)*cart[0] + cos(theta)*cart[1] + poseW[1];
	return cart_trans;
}

/* function name: getLine
** parameters:
double x1 --> point1's x-coordinate,
double y1 --> point1's y-coordinate,
double x2 --> point2's x-coordinate,
double y2 --> point2's y-coordinate
return: vector<double> line, the line show as kx - y + b = 0, so line[0] = k, line[1] = -1, line[2] = b.
*/
vector<double> ObjectDetection::getLine(double x1, double y1, double x2, double y2) {
	vector<double> line(3, 9999);

	if (x1 == x2 && y1 == y2) {
		cout << "no line error!" << endl;
		return line;
	}
	else if (x1 == x2 && y1 != y2) {
		line[0] = 1;
		line[1] = 0;
		line[2] = -x1;
		return line;
	}
	else if (x1 != x2 && y1 == y2) {
		line[0] = 0;
		line[1] = 1;
		line[2] = -y1;
		return line;
	}
	double k = (y1 - y2) / (x1 - x2);
	double b = y2 - k * x2;

	line[0] = k;
	line[1] = -1;
	line[2] = b;

	return line;
}

/* function name: pointLineDis
**
parameters:  double x --> point's x-coordinate,
double y --> point's y-coordinate,
return: double dis, the distance between point(x, y) and line;
*/
double ObjectDetection::pointLineDis(double x, double y, vector<double> line) {
	double dis = 0;
	double A = line[0];
	double B = line[1];
	double C = line[2];
	double num = abs(A * x + B * y + C);
	double den = sqrt(A * A + B * B);
	dis = num / den;
	return dis;
}

/* function name: lsqLine
** parameters:  vector<double> X --> points' x-coordinate set,
vector<double> Y --> points' y-coordinate set,
return: vector<double> lqsLine, the line show as  bx - y + a = 0(a and b are the coefficients of LSQ line,
so line[0] = b, line[1] = -1, line[2] = a.
*/
vector<double> ObjectDetection::lsqLine(vector<double> X, vector<double> Y) {
	vector<double> lsqLine(3, 0);

	if (X.empty() || Y.empty()) {
		cout << "Error: input vector empty!" << endl;
		return lsqLine;
	}

	int n = X.size();
	double sumX = 0, sumY = 0;
	double meanX = 0;
	double meanY = 0;
	double covXY = 0;
	double varX = 0;
	double varY = 0;
	double b = 0;
	double a = 0;

	for (int i = 0; i < n; i++) {
		sumX += X[i];
		sumY += Y[i];
	}
	meanX = sumX / n;
	meanY = sumY / n;
	for (int i = 0; i < n; i++) {
		covXY += (X[i] - meanX)*(Y[i] - meanY);
		varX += (X[i] - meanX) * (X[i] - meanX);
		varY += (Y[i] - meanY) * (Y[i] - meanY);
	}

	if (varX<0.00001){ //the line is parallel to y axis
		lsqLine[0] = -1;
		lsqLine[1] = 0;
		lsqLine[2] = meanX;
	}
	else if (varY<0.00001){//the line is parallel to x axis
		lsqLine[0] = 0;
		lsqLine[1] = -1;
		lsqLine[2] = meanY;
	}
	else{
		b = covXY / varX;
		a = meanY - b * meanX;
		//a = y_ - bx_ --> bx - y + a = 0
		lsqLine[0] = b;
		lsqLine[1] = -1;
		lsqLine[2] = a;
	}
	return lsqLine;
}

vector<vector<double>> ObjectDetection::ransac(vector<double> X, vector<double> Y, int itr, float thresh){

	vector<double> remain_X = X;
	vector<double> remain_Y = Y;
	vector<vector<double>> Lines;

	while (remain_X.size() != 0){
		vector<double> X_inliers;
		vector<double> Y_inliers;
		int maxNoOfInlier = 0;
		vector<double> line;

		
		for (int i = 0; i < itr; i++){
			// select two random numbers to fit a line
			int n1=-1, n2=-1;
			while (n1==n2){
				n1 = rand() % remain_X.size();
				n2 = rand() % remain_X.size();
			}
			double x1 = remain_X[n1];
			double y1 = remain_Y[n1];
			double x2 = remain_X[n2];
			double y2 = remain_Y[n2];
			line = getLine(x1, y1, x2, y2);

			// calculate the number of inliers of this line
			int noOfInlier = 0;
			vector<double> XX;
			vector<double> YY;
			for (int i = 0; i < remain_X.size(); i++){
				double dis = pointLineDis(remain_X[i], remain_Y[i], line);
				if (dis < thresh){
					XX.push_back(remain_X[i]);
					YY.push_back(remain_Y[i]);
					noOfInlier++;
				}
			}
			if (noOfInlier > maxNoOfInlier){
				maxNoOfInlier = noOfInlier;
				X_inliers = XX;
				Y_inliers = YY;
			}
		}
		// use all the inliers to fit the line
		line = lsqLine(X_inliers, Y_inliers);
		Lines.push_back(line);

		// print result
		for (int i = 0; i < X_inliers.size(); i++){
			printf("Inlier: %lf, %lf \n", X_inliers[i], Y_inliers[i]);
		}
		printf("Line: %lf,%lf,%lf \n", line[0], line[1], line[2]);

		// delete inliers for next detection
		for (int i = 0; i <X_inliers.size(); i++){
			for (int j = 0; j < remain_X.size(); j++){
				if (X_inliers[i] == remain_X[j] && Y_inliers[i] == remain_Y[j]){
					remain_X.erase(remain_X.begin() + j);
					remain_Y.erase(remain_Y.begin() + j);
					break;
				}
			}
		}
		
	}
	printf("%d lines detected!\n",Lines.size());
	return Lines;
}

vector<double> ObjectDetection::getIntersectedPoint(vector<double> line1, vector<double> line2) {
	vector<double> point(2, 9999);
	double a1 = line1[0];
	double b1 = line1[1];
	double c1 = line1[2];
	double a2 = line2[0];
	double b2 = line2[1];
	double c2 = line2[2];

	if (b1 != 0 && b2 != 0) {
		double k1 = -a1 / b1;
		double k2 = -a2 / b2;
		if (k1 == k2) {
			std::cout << "two lines parallel, no intersected point!" << endl;
			return point;
		}
	}
	else if (b1 == 0 && b2 == 0) {
		std::cout << "two lines parallel, no intersected point!" << endl;
		return point;
	}
	double x;
	double y;
	if (a1 == 0) {
		y = -c1 / b1;
		x = -b2 * y / a2 - c2 / a2;
	}
	else if (a2 == 0) {
		y = -c2 / b2;
		x = -b1 * y / a1 - c1 / a1;
	}
	else{
		y = (a1 * c2 - a2 * c1) / (a2 * b1 - a1 * b2);
		x = -b1 * y / a1 - c1 / a1;
	}
	point[0] = x;
	point[1] = y;
	return point;
}
double ObjectDetection::getPointsDis(vector<double> point1, vector<double> point2) {
	double x1 = point1[0];
	double y1 = point1[1];
	double x2 = point2[0];
	double y2 = point2[1];
	double dis = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

	return dis;
}

double ObjectDetection::getMaxSideLength(vector<double> nums) {
	if (nums.empty()) {
		return -1;
	}
	int n = nums.size();
	double maxSideLength = nums[0];
	for (int i = 1; i < n; i++) {
		if (nums[i] > maxSideLength) {
			maxSideLength = nums[i];
		}
	}
	return maxSideLength;
}


bool mySort(side s1, side s2){
	return s1.length < s2.length;
}

void ObjectDetection::objectPose(vector<vector<double>> Lines){

	vector<double> pose(3, 0);
	string object;

	if (Lines.size() == 3){
		side side1, side2, side3;

		side1.line = Lines[0];
		side2.line = Lines[1];
		side3.line = Lines[2];

		vector<vector<double>> Points;
		Points.push_back(getIntersectedPoint(side1.line, side2.line));
		Points.push_back(getIntersectedPoint(side1.line, side3.line));
		Points.push_back(getIntersectedPoint(side2.line, side3.line));
		for (int i = 0; i < 3; i++){
			cout << "point" << i << ":" << Points[i][0] << " " << Points[i][1] << endl;
		}
			
		side1.length = getPointsDis(Points[0], Points[1]);
		side2.length = getPointsDis(Points[0], Points[2]);
		side3.length = getPointsDis(Points[1], Points[2]);

		vector<side> threeSides;
		threeSides.push_back(side1);
		threeSides.push_back(side2);
		threeSides.push_back(side3);
		sort(threeSides.begin(), threeSides.end(), mySort);
		for (int i = 0; i < 3; i++){
			cout << "side" << i << ":" << threeSides[i].length << endl;
		}

		// judge the shape of object
		if (threeSides[0].length>0.09 && threeSides[0].length<0.11){
			cout << "The object is object 3" << endl;
		}
		else if (threeSides[0].length>0.14 && threeSides[0].length<0.16){
			cout << "The object is object 4" << endl;
		}
		else{
			cout << "It is a triangle but not object 3 or 4" << endl;
		}

		// get the position (the world coordinate of the O point)
		vector<double> pointO = getIntersectedPoint(threeSides[0].line, threeSides[1].line);
		vector<double> pointE = getIntersectedPoint(threeSides[1].line, threeSides[2].line);
		// get the oritation (the angle of the vector OE)
		double y = pointE[1] - pointO[1];
		double x = pointE[0] - pointO[0];
		double theta = atan2(y, x) * 180 / PI;
		pose[0] = pointO[0];
		pose[1] = pointO[1];
		pose[2] = theta;
		cout << "The pose of the object is" << pose[0] << "," << pose[1] << "," << pose[2] << endl;
	}
	else if (Lines.size() == 4){
		vector<double> line1 = Lines[0];
		vector<double> line2 = Lines[1];
		vector<double> line3 = Lines[2];
		vector<double> line4 = Lines[3];
		vector<vector<double>> lineSet;
		vector<double> point1 = getIntersectedPoint(line1, line2);
		lineSet.push_back(line1);
		lineSet.push_back(line2);
		lineSet.push_back(line3);
		lineSet.push_back(line4);
		int groupCnt = 0;
		vector<vector<int>> parallelLineHierachy;
		for (int i = 0; i < lineSet.size(); i++) {
			for (int j = i + 1; j < lineSet.size(); j++) {
				double k1 = -lineSet[i][0] / lineSet[i][1];  //k=-a/b
				double k2 = -lineSet[j][0] / lineSet[j][1];
				double diff = k1 - k2;
				if (diff < 0) {
					diff = -diff;
				}
				if (diff < 0.01) {
					parallelLineHierachy[groupCnt][0] = i;
					parallelLineHierachy[groupCnt][1] = j;
					groupCnt++;
				}
			}
		}
		if (groupCnt != 2) {
			cout << "Error: can not find 2 parallel sides!" << endl;
			cout << "only find" << groupCnt << " parallel side" << endl;
		}
		groupCnt = 0;
		vector<vector<double>> pointSet;  //两条非平行线得交点集合
		for (int i = 0; i < parallelLineHierachy[0].size(); i++) {
			int index1 = parallelLineHierachy[0][i];
			for (int j = 0; j < parallelLineHierachy[1].size(); j++) {
				int index2 = parallelLineHierachy[1][j];
				pointSet.push_back(getIntersectedPoint(lineSet[index1], lineSet[index2]));
			}
		}
		//find 2 short side
		vector<vector<int>> shorSidePointHierachy;
		groupCnt = 0;
		for (int i = 0; i < pointSet.size(); i++) {
			for (int j = i + 1; j < pointSet.size(); j++) {
				double disTemp = getPointsDis(pointSet[i], pointSet[j]);
				if (disTemp < 0.25) {
					shorSidePointHierachy[groupCnt].push_back(i);
					shorSidePointHierachy[groupCnt].push_back(j);
					groupCnt++;
				}
			}
		}

		vector<double> middlePoint1;
		vector<double> middlePoint2;
		middlePoint1[0] = (pointSet[shorSidePointHierachy[0][0]][0] + pointSet[shorSidePointHierachy[0][1]][0]) / 2;
		middlePoint1[1] = (pointSet[shorSidePointHierachy[0][0]][1] + pointSet[shorSidePointHierachy[0][1]][1]) / 2;

		middlePoint2[0] = (pointSet[shorSidePointHierachy[1][0]][0] + pointSet[shorSidePointHierachy[1][1]][0]) / 2;
		middlePoint2[1] = (pointSet[shorSidePointHierachy[1][0]][1] + pointSet[shorSidePointHierachy[1][1]][1]) / 2;

		double theta = atan((pointSet[shorSidePointHierachy[0][0]][1] - pointSet[shorSidePointHierachy[0][1]][1]) / (pointSet[shorSidePointHierachy[0][0]][0] - pointSet[shorSidePointHierachy[0][1]][0]));
		if (theta >= 0) {
			theta -= 3.14 / 2;
		}
		else {
			theta += 3.14 / 2;
		}
		double x = (middlePoint1[0] + middlePoint2[0]) / 2;
		double y = (middlePoint1[1] + middlePoint2[1]) / 2;

		pose[0] = x;
		pose[1] = y;
		pose[2] = theta;
	}
	
	else{
		printf("Not triangle or rectangle!\n");
	}

}



