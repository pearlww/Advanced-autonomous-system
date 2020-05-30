#!/usr/bin/python3

import os

filename = "auxOriginalGuidemarkLocations.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

x = []
y = []
q = []

lines = original_text.split('\n')
for line in lines:
	parts = line.split('=')
	num_parts = len(parts)
	if (num_parts == 2):
		coord_type = parts[0]
		coord_val = parts[1]
		if(coord_type[0:3] == 'gmx'):
			x.append(coord_val)
		elif(coord_type[0:3] == 'gmy'):
			y.append(coord_val)
		elif(coord_type[0:3] == 'gmt'):
			q.append(coord_val)

num_gm = len(x)
simplified_text = "ID\tx\ty\tq\n"
for gm_num in range(num_gm):
	simplified_text += str(gm_num+1) + '\t' + x[gm_num] + '\t' \
							+ y[gm_num] + '\t' + q[gm_num] + '\n'


filename = "auxSimplifiedGuidemarks.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(simplified_text)
f.close()