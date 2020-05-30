#!/usr/bin/python3

import os

filename = "constLaserINI.txt"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

x1s = []
y1s = []
x2s = []
y2s = []

lines = original_text.split('\n')
for line in lines:
	addline_found = line.find('INSERTOBJECTHERE')
	if(addline_found != -1):
		line_parts = line.split()

		x1_parts = line_parts[1].split('=')
		y1_parts = line_parts[2].split('=')
		x2_parts = line_parts[3].split('=')
		y2_parts = line_parts[4].split('=')

		x1s.append(x1_parts[1].strip())
		y1s.append(y1_parts[1].strip())
		x2s.append(x2_parts[1].strip())
		y2s.append(y2_parts[1].strip())

new_text = ""
for x1,y1,x2,y2 in zip(x1s, y1s, x2s, y2s):
	new_text += x1 + '\t' + y1 + '\t' + x2 + '\t' + y2 + '\n'

filename = "auxLaserVisualizer.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(new_text)
f.close()