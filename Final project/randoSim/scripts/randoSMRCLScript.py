#!/usr/bin/python3
'''
Author:			Jeremy Coffelt
Date:			Spring 2020
Course:			31388 - Advanced Autonomous Systems
Group:			Number 17
Groupmates:		P. Christensen, P. Wang, Z. Lei, and N. Schieber
Filename:		randoSMRCLScript.py
Purpose:		Updates SMR-CL script to start at first guidemark of tour
Dependencies:	constSMRCL.txt, which contains the constant part of SMR-CL script
				simSummaryGuidemarks.txt, which summarizes the guidemark tour
Outputs:		randoGuidemarks.txt, which contains the randomized SMR-CL script
'''
import os

filename = "simSummaryGuidemarks.txt"
path = "../simSummary/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
gm_text = f.read()
f.close()

gm_lines = gm_text.split('\n')
gm_line = gm_lines[1]
gm_text_parts = gm_line.split(':')
gm_text_part = gm_text_parts[1]
gm_part = gm_text_part.split('-')
first_gm = gm_part[0].strip()

filename = "constSMRCL.txt"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

new_text = ""
lines = original_text.split('\n')
first_gmkno_line_found = False
for line in lines:
	line_found = line.find('gmkno')
	if(line_found == -1):
		new_text += line + '\n'
	elif (not first_gmkno_line_found):
		new_text += "gmkno = " + first_gm + '\n'
		first_gmkno_line_found = True

filename = "randoSMRCL.txt"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(new_text)
f.close()