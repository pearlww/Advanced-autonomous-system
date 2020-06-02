#include "object_detection.h"

using namespace std;

int main(void){
	vector<double> X;
	vector<double> Y;
	vector<vector<double>> Lines;
	ObjectDetection od;

	//ofstream out("detection_result.txt");
	//streambuf *coutbuf = cout.rdbuf(); //save old buf
	//cout.rdbuf(out.rdbuf()); //redirect cout to detection_result.txt

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
	ifstream myfile("laserData_rect.txt");
	double x;
	double y;
	while ( myfile >> x >> y) {
		X.push_back(x);
		Y.push_back(y);
	}

	/*--------------------------------------------------------------------------*/
	Lines = od.ransac(X, Y, 100, 0.02 );
	od.objectPose(Lines);

	system("pause");
	return 0;

}