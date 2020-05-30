#!/usr/bin/python3

import os

filename = "constSMRCL.txt"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

PNOs = []
Xs = []
Ys = []

PNO1s = []
PNO2s = []

new_text = ""
lines = original_text.split('\n')
for line in lines:
	addpoint_found = line.find('addpoint')
	if(addpoint_found != -1):
		line_parts = line.split()

		pno_parts = line_parts[2].split('=')
		PNOs.append(pno_parts[1].strip())
		
		x_parts = line_parts[3].split('=')
		Xs.append(x_parts[1].strip())
		
		y_parts = line_parts[4].split('=')
		y_subparts = y_parts[1].split('"')
		Ys.append(y_subparts[0].strip())

	addcon_found = line.find('addcon')
	if(addcon_found != -1):
		line_parts = line.split()

		pno1_parts = line_parts[2].split('=')
		PNO1s.append(pno1_parts[1].strip())
		
		pno2_parts = line_parts[3].split('=')
		pno2_subparts = pno2_parts[1].split('"')		
		PNO2s.append(pno2_subparts[0].strip())

new_text = ""
for pno1, pno2 in zip(PNO1s, PNO2s):
	x1 = Xs[int(pno1) - 1]
	y1 = Ys[int(pno1) - 1]
	x2 = Xs[int(pno2) - 1]
	y2 = Ys[int(pno2) - 1]	
	new_text += x1 + '\t' + y1 + '\t' + x2 + '\t' + y2 + '\n'

filename = "auxPlannerVisualizer.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(new_text)
f.close()