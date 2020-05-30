#!/usr/bin/python3

import os
import math
from random import random
from random import randint

X_min = 1
X_max = 3
Y_min = 1
Y_max = 2

obj_num = randint(1, 4)

if (obj_num < 3):
	obj_type = 'rectangle'
	num_sides = 4

	if (obj_num == 1):
		side_length1 = 0.15
		side_length2 = 0.40        
	else:
		side_length1 = 0.20
		side_length2 = 0.30   

	L1 = 0.5 * side_length1
	L2 = 0.5 * side_length2

	cornersX = [-L2, -L2,  L2,  L2]
	cornersY = [ L1, -L1, -L1,  L1]
	side_lengths = [side_length1, side_length2, side_length1, side_length2] 
	perimeter = 2 * (side_length1 + side_length2)

else:
	obj_type = 'triangle'
	num_sides = 3

	if (obj_num == 3):
		side_length1 = 0.10
		side_length2 = 0.40       
	else:
		side_length1 = 0.15
		side_length2 = 0.30      

	cornersX = [0, 0, side_length2]
	cornersY = [side_length1, 0, 0]           

	hypotenuse = math.sqrt(side_length1 ** 2 + side_length2 ** 2)
	side_lengths = [side_length1, side_length2, hypotenuse] 
	perimeter = side_length1 + side_length2 + hypotenuse

new_cornersX = cornersX.copy()
new_cornersY = cornersY.copy()

obj_fits = False
while (not obj_fits):
	obj_fits = True 
	x0 = X_min + (X_max - X_min) * random()
	y0 = Y_min + (Y_max - Y_min) * random()

	# rectangles are symmetric mod 180 deg; triangles are not
	q0 = math.pi * random()
	if(obj_type == 'triangle'):
		q0 = 2 * q0

	C = math.cos(q0)
	S = math.sin(q0)

	for c in range(0, num_sides):

		x_old = cornersX[c]
		y_old = cornersY[c]

		new_x = x0 + (C * x_old - S * y_old) 
		new_y = y0 + (S * x_old + C * y_old)

		if ((new_x < X_min) or (X_max < new_x)):
			obj_fits = False

		if ((new_y < Y_min) or (Y_max < new_y)):
			obj_fits = False

		new_cornersX[c] = new_x 
		new_cornersY[c] = new_y		

summary_text = "Type: " + obj_type + '\n'
summary_text += "ID:   " + str(obj_num) + '\n'
summary_text += "Pose: (" + str(x0) + ', ' + str(y0) + ', ' + str(q0) + ')'


filename = "simSummaryObject.txt"
path = "../simSummary/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(summary_text)
f.close()

new_cornersX.append(new_cornersX[0])
new_cornersY.append(new_cornersY[0])

obj_wall_text = ""
for c in range(num_sides):
	x1 = str(new_cornersX[c])
	y1 = str(new_cornersY[c])
	x2 = str(new_cornersX[c+1])
	y2 = str(new_cornersY[c+1])
	obj_wall_text += x1 + '\t' + y1 + '\t' + x2 + '\t' + y2 + '\n'

filename = "auxObjectWalls.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(obj_wall_text)
f.close()

filename = "constWalls.txt"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
const_wall_text = f.read()
f.close()

rando_wall_text = const_wall_text + '\n\n' + obj_wall_text
filename = "randoWalls.txt"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(rando_wall_text)
f.close()