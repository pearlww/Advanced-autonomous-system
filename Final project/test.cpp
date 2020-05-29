#include "object_detection.h"

using namespace std;

vector<vector<double>> createDataset(vector<double> p1, vector<double> p2, vector<double> p3) {
	vector<vector<double>> lines;
	ObjectDetection od;
	vector<double> line1 = od.getLine(p1[0], p1[1], p2[0], p2[1]);
	vector<double> line2 = od.getLine(p1[0], p1[1], p3[0], p3[1]);
	vector<double> line3 = od.getLine(p2[0], p2[1], p3[0], p3[1]);
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;

	return lines;
}


int main(void){
	vector<double> X;
	vector<double> Y;
	vector<vector<double>> Lines;
	ObjectDetection od;

	for (int i = 0; i <= 10; i++){
		X.push_back(i);
		Y.push_back(i);
	}
	for (int i = 0; i <= 10; i++){
		X.push_back(10);
		Y.push_back(i);
	}
	for (int i = 0; i <= 10; i++){
		X.push_back(i);
		Y.push_back(0);
	}

	Lines = od.ransac(X,Y,10,0.01);
	od.objectPose(Lines);
	//ObjectDetection od;
	//vector<double> p1;
	//vector<double> p2;
	//vector<double> p3;
	//p1.push_back(0);
	//p1.push_back(0);
	//p2.push_back(0.4);
	//p2.push_back(0);
	//p3.push_back(0);
	//p3.push_back(0.1);
	//vector<vector<double>> Lines;
	//Lines = createDataset(p1,p2,p3);
	//od.objectPose(Lines);

	system("pause");
	return 0;

}