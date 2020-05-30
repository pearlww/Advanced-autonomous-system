#!/usr/bin/env bash

clear

printf "Randomizing object on loose type, location, and orientation... "
./randoObject.py
printf "DONE!\n\nGenerating fake laser data for optional debugging and testing... "
./randoFakeObjectPoints.py
printf "DONE!\n\nRandomizing guidemark selection and ordering... "
./randoGuidemarks.py

printf "DONE!\n\nUpdating SMR-CL script with randomized first guidemark... "
./randoSMRCLScript.py
printf "DONE!\n\nUpdating simserver config.xml file with walls of randomized object on loose... "
./randoConfigXML.py
printf "DONE!\n\nUpdating laser initializaton file with walls of randomized object on loose... "
./randoLaserINI.py

printf "DONE!\n\nCopying randoWalls.txt to /home/smr/k388/sim/... "
cp ../rando/randoWalls.txt /home/smr/k388/sim/randoWalls
printf "DONE!\n\nCopying constLines.txt to /home/smr/k388/sim/... "
cp ../const/constLines.txt /home/smr/k388/sim/constLines
printf "DONE!\n\nCopying randoSMRCL.txt to /home/smr/k388/sim/... "
cp ../rando/randoSMRCL.txt /home/smr/k388/sim/randoSMRCL
printf "DONE!\n\nCopying randoConfig.xml to /home/smr/k388/sim/... "
cp ../rando/randoConfig.xml /home/smr/k388/sim/randoConfig.xml
printf "DONE!\n\nCopying randoLaserINI.txt to /home/smr/k388/sim/ as NEW file... "
cp ../rando/randoLaserINI.txt /home/smr/k388/sim/ulmsserver.ini.NEW

printf "DONE!\n\nChanging directory to /home/smr/k388/sim/... "
cd /home/smr/k388/sim/
printf "DONE!\n\nCreating backup copy of current ulmsserver.ini file named ulmsserver.ini.bck... "
cp ulmsserver.ini ulmsserver.ini.bck
printf "DONE!\n\nRenaming ulmsserver.ini.NEW as ulmsserver.ini... "
mv ulmsserver.ini.NEW ulmsserver.ini

printf "DONE!\n\nLaunching simulator tabs... "
gnome-terminal --tab -e 'sh -c "cd /home/smr/k388/sim/; simserver1 randoConfig.xml; exec bash"' \
--tab -e 'sh -c "cd /home/smr/k388/sim/; ulmsserver; exec bash"' \
--tab -e 'sh -c "cd /home/smr/k388/sim/; sleep 30; mrc -s8000 randoSMRCL; exec bash"'