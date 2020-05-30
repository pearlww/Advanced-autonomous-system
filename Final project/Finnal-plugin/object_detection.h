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

struct side{
	vector<double> line;
	vector<double> p1;
	vector<double> p2;
	double length;
};

#define PI 3.14159265

class ObjectDetection{
public:
	//ObjectDetection();
	vector<double> transform(vector<double> poseW, vector<double> cart);
	vector<double> getLine(double x1, double y1, double x2, double y2);
	double pointLineDis(double x, double y, vector<double> line);
	vector<double> lsqLine(vector<double> X, vector<double> Y);
	vector<vector<double>> ransac(vector<double> X, vector<double> Y, int itr, float thresh);

	vector<double> getIntersectedPoint(vector<double> line1, vector<double> line2);
	double getPointsDis(vector<double> point1, vector<double> point2);
	bool checkParallel(vector<double> line1, vector<double> line2);
	void objectPose(vector<vector<double>> Lines);


protected:
	// variable to store good line fits
	vector<vector<double>> points;


};


#endif /*OBJECT_DETECTION_H*/