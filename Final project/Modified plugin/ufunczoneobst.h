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
#define PI 3.14159265
using namespace std;


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

	void stack(const double&, const double&);

	void lsqline(const vector<double>&, const vector<double>&);
};



#endif

