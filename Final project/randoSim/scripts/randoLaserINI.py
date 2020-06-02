#!/usr/bin/python3

'''
Author:			Jeremy Coffelt
Date:			Spring 2020
Course:			31388 - Advanced Autonomous Systems
Group:			Number 17
Groupmates:		P. Christensen, P. Wang, Z. Lei, and N. Schieber
Filename:		randoLaserINI.py
Purpose:		Updates the ulmsserver.ini file with new OOTL walls
Dependencies:	auxObjectWalls.txt, which contains (only) the walls of OOTL
				constLaserINI.txt, which is the template file to be updated
Outputs:		randoLaserINI.txt, which will ultimately be copied and
					renamed as ulmsserver.ini
'''
import os

filename = "auxObjectWalls.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
wall_text = f.read()
f.close()

x1s = []
y1s = []
x2s = []
y2s = []

lines = wall_text.split('\n')
for line in lines:
	try:
		line_parts = line.split()

		x1s.append(line_parts[0].strip())
		y1s.append(line_parts[1].strip())
		x2s.append(line_parts[2].strip())
		y2s.append(line_parts[3].strip())
	except:
		pass

inserted_text = ""
line_num = 1
for x1,y1,x2,y2 in zip(x1s, y1s, x2s, y2s):
	inserted_text += "addline startx=" + x1 + '\tstarty=' + y1 + \
			'\tendx=' + x2 + '\tendy=' + y2 + 'name="object line ' \
			+ str(line_num) + '"\n'
	line_num += 1

filename = "constLaserINI.txt"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

new_text = ""
lines = original_text.split('\n')
for line in lines:
	line_found = line.find('INSERTOBJECTHERE')
	if(line_found == -1):
		new_text += line + '\n'
	else:
		new_text += inserted_text

filename = "randoLaserINI.txt"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(new_text)
f.close()