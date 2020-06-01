/***************************************************************************
 *   Copyright (C) 2005 by Christian Andersen and DTU                             *
 *   jca@oersted.dtu.dk                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef UFUNC_MRCOBST_H
#define UFUNC_MRCOBST_H

#include <cstdlib>
#include <ulms4/ufunclaserbase.h>
#include <urob4/uresposehist.h>
#include<vector>
#include<fstream>
#include<iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm> 

using namespace std;

#define PI 3.14159265
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/**
 * Laserscanner function to demonstrate
 * simple laser scanner data handling and analysis
 * @author Christian Andersen
*/
// Extened version of UFunczoneobst
class UFunczoneobst : public UFuncLaserBase
{
  // Basically, the public part is in ufunczoneobst.cpp 
public:
	UFunczoneobst()
  { 
    setCommand("zoneobst", "zoneobstif", "obstacle detect for MRC (Compiled " __DATE__ " " __TIME__ ")");
    createBaseVar();
  }
	virtual bool setResource(UResBase * resource, bool remove);
	virtual bool handleCommand(UServerInMsg * msg, void * extra);

  // Members
protected:
	void createBaseVar();
	UVariable *var_zone;
	UResPoseHist * poseHist;
  
  // Private member functions, Here I just put declaration.
private:
	// member
	vector<vector<double>> selectedPointsInWorld; 
	// functions
	void transform(vector<double>&);
	void transform(const vector<double>&, double&, double&);
};



///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  OBJECT DETECTION CLASS /////////////////////////////////////////

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
	vector<vector<double>> ransac(vector<double> X, vector<double> Y, int itr, float thresh, int m=0);

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


#endif

