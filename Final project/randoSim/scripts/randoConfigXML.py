#!/usr/bin/python3

import os

filename = "randoGuidemarks.txt"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
gm_text = f.read()
f.close()

inserted_text = ""

is_header = True
lines = gm_text.split('\n')
for line in lines:
	if(is_header):
		is_header = False
	else:
		try:
			parts = line.split('\t')
			num = parts[0]
			x = parts[1]
			y = parts[2]
			q = parts[3]
			val = parts[4]
			inserted_text += "      " + num + "<guidemark gmno=\"" + val +"\" gmpose=\"" + x + ' ' + y + ' ' + q + "\"/>\n"
		except:
			pass

filename = "constConfig.xml"
path = "../const/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "r")
config_text = f.read()
f.close()

new_text = ""
lines = config_text.split('\n')
for line in lines:
	if(line != "INSERTGUIDEMARKSHERE"):
		new_text += line + '\n'
	else:
		new_text += inserted_text


filename = "randoConfig.xml"
path = "../rando/"
fullpath = os.path.join(path, filename)
f = open(fullpath, "w")
f.write(new_text)
f.close()