#include "ufunczoneobst.h"
#ifdef LIBRARY_OPEN_NEEDED
// This part defines the object necessary for plugin to work properly.
UFunctionBase * createFunc()
{ 
  return new UFunczoneobst();
}
#endif

bool UFunczoneobst::setResource(UResBase * resource, bool remove) { // load resource as provided by the server (or other plugins)
	bool result = true;

	if (resource->isA(UResPoseHist::getOdoPoseID())) { // pointer to server the resource that this plugin can provide too
		// but as there might be more plugins that can provide the same resource
		// use the provided
		if (remove)
			// the resource is unloaded, so reference must be removed
			poseHist = NULL;
		else if (poseHist != (UResPoseHist *) resource)
			// resource is new or is moved, save the new reference
			poseHist = (UResPoseHist *) resource;
		else
			// reference is not used
			result = false;
	}

	// other resource types may be needed by base function.
	result = UFunctionBase::setResource(resource, remove);
	return result;
}



// As function name shows, it handles command.
bool UFunczoneobst::handleCommand(UServerInMsg * msg, void * extra)
{  

  // Command to be sent out
  const int MRL = 500;
  char reply[MRL];

  // 2 bollean variables for debugging
  bool doTest = false;
  bool ask4help = false;

  // 2 boolean state variables
  bool detectObject = false;
  bool determineObject = false;
  bool savedata = false;

  // To receive message
  const int MVL = 100; 
  char value[MVL]; 

  // Data
  ULaserData * data;
  double robotwidth = 0.26; 

  // Taking world pose
  double xw = -1;
  double yw = -1;
  double thw = -1;

  // Some parameters 
  int i,j,imax;
  double r,delta;
  double minRange; // min range in meter
  double zone[9];

  // For help or for test
  ask4help = msg->tag.getAttValue("help", value, MVL);
  doTest = msg->tag.getAttValue("test", value, MVL);

  // Switch working modes
  detectObject = msg->tag.getAttValue("detect", value, MVL);
  determineObject = msg->tag.getAttValue("determine", value, MVL);
  savedata = msg->tag.getAttValue("savedata", value, MVL);

  // Get current pose in world
  if (msg->tag.getAttValue("x", value, MVL)) {
	  xw = strtod(value, NULL);
  }
  if (msg->tag.getAttValue("y", value, MVL)) {
	  yw = strtod(value, NULL);
  }
  if (msg->tag.getAttValue("th", value, MVL)) {
	  thw = strtod(value, NULL);
  }

  // Basic test
  if (ask4help) 
  { 
    sendHelpStart(msg, "zoneobst");
    sendText("--- available zoneobst options\n");
    sendText("help            This message\n");
    sendText("fake=F          Fake some data 1=random, 2-4 a fake corridor\n");
    sendText("device=N        Laser device to use (see: SCANGET help)\n");
    sendText("see also: SCANGET and SCANSET\n");
    sendHelpDone();
  } 

  // Simple test
  if (doTest){
	  snprintf(reply, MRL, "<Robot pose in world is: xw=\"%g\" yw=\"%g\" thw=\"%g\" />\n",
		  xw, yw, thw);
	  sendMsg(msg, reply);
  }

  // Detect and save data
  if(detectObject)
  { // Get laser data
    data = getScan(msg, (ULaserData*)extra);

    // If data is valid, obtain them and do some transformation.
    if (data->isValid())
    {
	  // Robot pose in world
		vector<double> posew;
		posew.push_back(xw);
		posew.push_back(yw);
		posew.push_back(thw);

	 // Transformed vector: the laser frame in world
		transform(posew);

	 // For taking data
		vector<double> x_data;
		vector<double> y_data;
	
     // Get the number of data
		int data_num = data->getRangeCnt();

	 // Select data
		for (int i = 0; i < data_num; i++) {

			// Originally, in polar format
			double range = data->getRangeMeter(i);
			double angle = data->getAngleRad(i);

			// Less than 20 units is a flag value for URG scanner.
			if (range <= 0.02) {
				continue;
			}

			// Conver data to Cantesian frame
			double x = range * cos(angle);
			double y = range * sin(angle);

			// Convert into world frame
			transform(posew, x, y);

			// Check if points are within the rectangular area.
			if (x > 1 && x < 3 && y>1 && y < 2) {
				stack(x, y);
			}
		}	
    }
  }

  if (savedata) {
	  using namespace std;
	  ofstream out;
	  const char* filename = "~/k388/sim/output.txt";
	  out.open(filename);
	  for (int i = 0; i < selectedPointsInWorld.size();i++) {
		  vector<double> point = selectedPointsInWorld[i];
		  out << point[0] << ' ' << point[1] << '\n';
	  }
	  out.close();
  }

  if(determineObject){
  }
  return true;
}

void UFunczoneobst::createBaseVar()
{ 
  var_zone = addVarA("zone", "0 0 0 0 0 0 0 0 0", "d", "Value of each laser zone. Updated by zoneobst.");
}

void UFunczoneobst::transform(vector < double >& pose)
{
	/*
	The laser is mounted 26 cms in front of the center of the robot.
	So, in local frame, it will have the coordinates of [0.26; 0; 0].
	Considering the fact that we have known the robot pose in world, or xw, yw, thw,
	we can easily compute laser frame in world by using transformation: 
		[xlw;ylw]= Rotz(thw)*[0.26;0] + [xw;yw]
	     thlw = thw
	In order to speed up the procedure, we use reference in stead of copying data.
	*/
	pose[0] = cos(pose[2])*0.26 + pose[0];
	pose[1] = sin(pose[2])*0.26 + pose[1];
	pose[2] = pose[2];
}

void UFunczoneobst::transform(const vector<double>& pose, double& x, double& y) {
	/*
	Once we got the parameters necessary for representing transform from laser into world frame.
	We can now safely transform Cartesian coordinates in laser from into world frame for futher processing.
		[xw;tw] = Rotz(thlw)*[x;y] + [xlw;ylw]
	*/
	double thl_w = pose[2];
	double x_tmp = cos(thl_w)*x - sin(thl_w)*y + pose[0];
	double y_tmp = sin(thl_w)*x + cos(thl_w)*y + pose[1];
	x = x_tmp;
	y = y_tmp;
}

void UFunczoneobst::stack(const double& x, const double& y){
	/*
	It will save (x,y) in class member. The format is vector containing x and y coordinates in world.
	*/
	vector < double > point_tmp;
	point_tmp.push_back(x);
	point_tmp.push_back(y);
	UFunczoneobst::selectedPointsInWorld.push_back(point_tmp);
}

void UFunczoneobst::lsqline(const vector<double>& x, const vector<double>& y) {
	/*
	It will implement the lsq method described in exercises.
	*/
	int n = x.size();
	double xmean, ymean, sumx, sumy, sumx2, sumy2, sumxy;

	for (int j = 0; j < n; j++)
	{
		sumx += x[j];
		sumy += y[j];
	}

	xmean = sumx / (double)n;
	ymean = sumy / (double)n;

	sumx2 = 0;
	sumy2 = 0;
	sumxy = 0;
	for (int i = 0; i < n; i++)
	{
		sumx2 += x[i] * x[i];
		sumy2 += y[i] * y[i];
		sumxy += x[i] * y[i];
	}

	double a = 0.5 * atan2((2 * sumx * sumy - 2 * (double)n * sumxy), pow(sumx, 2) - pow(sumy, 2) - (double)n * sumx2 + (double)n * sumy2);
	double r = xmean * cos(a) + ymean * sin(a);

	if (r < 0)
	{
		r = abs(r);
		if (a < 0)
		{
			a += PI;
		}
		else
		{
			a -= PI;
		}
	}

	// make sure we are in ]-pi;pi]
	a = atan2(sin(a), cos(a));

	vector<double> line;
	line.push_back(a);
	line.push_back(r);

	return line;
}

