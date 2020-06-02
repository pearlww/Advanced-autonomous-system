#ifndef OBJECT_DETECTION_H
#define OBJECT_DETECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm> 

using namespace std;


#define PI 3.14159265

class ObjectDetection {
public:
	/* function name: getLine
	** parameters:
	double x1 --> point1's x-coordinate,
	double y1 --> point1's y-coordinate,
	double x2 --> point2's x-coordinate,
	double y2 --> point2's y-coordinate
	return: vector<double> line, the line show as kx - y + b = 0, so line[0] = k, line[1] = -1, line[2] = b.
	*/
	vector<double> getLine(double x1, double y1, double x2, double y2);

	/* function name: pointLineDis
	**
	parameters:  double x --> point's x-coordinate,
	double y --> point's y-coordinate,
	return: double dis, the distance between point(x, y) and line;
	*/
	double pointLineDis(double x, double y, vector<double> line);

	/* function name: lsqLine
	** parameters:  vector<double> X --> x-coordinate set of all points
	vector<double> Y --> y-coordinate set of all points
	return: vector<double> lqsLine, the line show as  bx - y + a = 0(a and b are the coefficients of LSQ line,
	so line[0] = b, line[1] = -1, line[2] = a.
	*/
	vector<double> lsqLine(vector<double> X, vector<double> Y);

	/* function name: ransac
	** parameters:  vector<double> X -->  x-coordinate set of all points
	vector<double> Y --> y-coordinate set of all points
	int itr --> the number of resampleing iterations
	float thresh --> the thresh distance of define a inlier
	int m --> the possible number of outliers, defult value is 0.
	**return:	vector<vector<double>> Lines --> all extracted lines
	*/
	vector<vector<double>> ransac(vector<double> X, vector<double> Y, int itr, float thresh, int m = 0);

	/* function name: getIntersectedPoint
	** parameters:  vector<double> line1 --> the first line,
	vector<double> line2 --> the other line,
	**return:	vector<double> point --> the intersecter point of the two line
	*/
	vector<double> getIntersectedPoint(vector<double> line1, vector<double> line2);

	/* function name: getPointsDis
	** parameters:  vector<double> point1 --> the first point,
	vector<double> point2 --> the other point,
	**return:	double dis --> the distance between 2 points
	*/
	double getPointsDis(vector<double> point1, vector<double> point2);

	/* function name: checkParallel
	** parameters:  vector<double> line1 --> the first line,
	vector<double> line2 --> the other line,
	**return:	double parallelFlag --> when 2 lines are parallel, return true.
	*/
	bool checkParallel(vector<double> line1, vector<double> line2);

	/* function name: objectPose
	** parameters:  vector<vector<double>> Lines --> a lines set which include 3 or 4 lines,
	** return:	vector<double> pose --> the pose[0] is the x-coordinary, the pose[1] is the y-coordinary, the pose[2] is the theta.
	*/
	vector<double> objectPose(vector<vector<double>> Lines);


};

// self-defined structure, used to save the edges' information of shapes
struct side {
	vector<double> line;
	vector<double> p1;
	vector<double> p2;
	double length;
};


#endif /*OBJECT_DETECTION_H*/