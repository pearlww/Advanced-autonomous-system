#include "object_detection.h"

using namespace std;

int main(void){
	vector<double> X;
	vector<double> Y;
	vector<vector<double>> Lines;
	ObjectDetection od;
	/*----------------------------- test1 --------------------------------------*/
	//for (int i = 0; i <= 6; i++){
	//	X.push_back(10);
	//	Y.push_back(i);
	//}
	//for (int i = 0; i <= 6; i++){
	//	X.push_back(0);
	//	Y.push_back(i);
	//}
	//for (int i = 0; i <= 10; i++){
	//	X.push_back(i);
	//	Y.push_back(0);
	//}
	//for (int i = 0; i <= 10; i++){
	//	X.push_back(i);
	//	Y.push_back(5);
	//}
	/*--------------------------------test2---------------------------------------------*/

	//for (int i = 0; i <= 10; i++){
	//	X.push_back(i);
	//	Y.push_back(i);
	//}
	//for (int i = 0; i <= 10; i++){
	//	X.push_back(10);
	//	Y.push_back(i);
	//}
	//for (int i = 0; i <= 10; i++){
	//	X.push_back(i);
	//	Y.push_back(0);
	//}

	/*----------------------------------- test3-------------------------------------------*/
	ifstream myfile("laserData.txt");
	double x;
	double y;
	while ( myfile >> x >> y) {
		X.push_back(x);
		Y.push_back(y);
	}

	for (int i = 0; i < X.size(); i++){
		cout << X[i] << " " << Y[i] << endl;
	}

	/*--------------------------------------------------------------------------*/
	Lines = od.ransac(X, Y, 100, 0.1);
	od.objectPose(Lines);

	system("pause");
	return 0;

}