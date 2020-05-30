#!/usr/bin/python3

import os
import math
from random import random

is_friendly = True

if(is_friendly):
	mu = 0
	point_density = 300
	sigma = 0.0025
else:
	mu = 0
	point_density = 100
	sigma = 0.01

filename = "auxObjectWalls.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
obj_wall_text = f.read()
f.close()

num_corners = 0
cornersX = []
cornersY = []

lines = obj_wall_text.split('\n')
for line in lines:
	coords = line.split('\t')
	coord_num = 1
	for coord in coords:
		try:
			coord = float(coord)	
			if (coord_num == 1):
				num_corners += 1
				cornersX.append(coord)
			elif (coord_num == 2):
				cornersY.append(coord)
			coord_num += 1

		except:
			pass

x_vals = []
y_vals = []
x_text = ""
y_text = ""
xy_text = ""

for c in range(num_corners):
	x1 = cornersX[c]
	y1 = cornersY[c]

	if (c  == num_corners-1):
		x2 = cornersX[0]
		y2 = cornersY[0]
	else:
		x2 = cornersX[c+1]
		y2 = cornersY[c+1]

	side_length = round(math.sqrt((x2 - x1)**2 + (y2 - y1)**2), 2)
	side_points = int(side_length * point_density)

	del_x = (x2 - x1) / side_points
	del_y = (y2 - y1) / side_points

	for point in range(side_points):
		r = mu + sigma * random()
		q = (2 * math.pi) * random()
		x_val = x1 + del_x * point + r * math.cos(q)
		y_val = y1 + del_y * point + r * math.sin(q)
		# x_vals.append(x_val)
		# y_vals.append(y_val)
		x_val = str(x_val)
		y_val = str(y_val)
		x_text += x_val + '\n'
		y_text += y_val + '\n'
		xy_text	+= x_val + '\t' + y_val + '\n'

filename = "testObjectXcoords.txt"
path = "../test/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(x_text)
f.close()

filename = "testObjectYcoords.txt"
path = "../test/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(y_text)
f.close()

filename = "testObjectXYcoords.txt"
path = "../test/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(xy_text)
f.close()