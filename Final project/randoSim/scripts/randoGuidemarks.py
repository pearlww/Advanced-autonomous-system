#!/usr/bin/python3
'''
Author:			Jeremy Coffelt
Date:			Spring 2020
Course:			31388 - Advanced Autonomous Systems
Group:			Number 17
Groupmates:		P. Christensen, P. Wang, Z. Lei, and N. Schieber
Filename:		randoGuidemarks.py
Purpose:		Randomly selects and orders guidemarks
Dependencies:	auxSimplifiedGuidemarks.txt, which contains constant info on gmks
Outputs:		randoGuidemarks.txt, which contains the randomized guidemarks
				simSummaryGuidemarks.txt, which summarizes the guidemark tour
'''
import os
from random import randint
from random import shuffle

filename = "auxSimplifiedGuidemarks.txt"
path = "../aux/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
original_text = f.read()
f.close()

IDs = []
Xs = []
Ys = []
Qs = []

is_header = True
lines = original_text.split('\n')
for line in lines:
	if(is_header):
		is_header = False
	else:
		try:
			parts = line.split('\t')
			IDs.append(parts[0])
			Xs.append(parts[1])
			Ys.append(parts[2])
			Qs.append(parts[3])
		except:
			pass

ttl_gm_used = randint(6,8)

ptr_pool = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
shuffle(ptr_pool)
first_gm = str(ptr_pool[0])

PTRs = []
for i in range(14):
	PTRs.append('-1')

for i in range(14):
	if (i < ttl_gm_used-1):
		PTRs[ptr_pool[i]-1] = str(ptr_pool[i+1])
	elif (i == ttl_gm_used-1):
		PTRs[ptr_pool[i]-1] = '98'
	else:
		PTRs[ptr_pool[i]-1] = first_gm
		
output_text = "ID\tx\ty\tq\tnext\n"
for i in range(14):
	output_text += str(i+1) + '\t' + Xs[i] + '\t' \
							+ Ys[i] + '\t' + Qs[i] + '\t' + PTRs[i]+ '\n'

filename = "randoGuidemarks.txt"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(output_text)
f.close()

output_text = "This simulation has randomly selected <" + str(ttl_gm_used) + "> guidemarks for use.\n"

gm_text = ""
for i in range(ttl_gm_used):
	gm_text += str(ptr_pool[i])
	if(i < ttl_gm_used - 1):
		gm_text += '-->'

output_text += "We will follow guidemarks in the order: " + gm_text + '\n'
output_text += "The final guidemark, <" + str(ptr_pool[ttl_gm_used-1]) + ">, will have value 98, which points home.\n"
output_text += "All unused guidemarks will have value <" + str(ptr_pool[0]) + ">, pointing back to the first guidemark."

filename = "simSummaryGuidemarks.txt"
path = "../simSummary/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(output_text)
f.close()