#!/usr/bin/env bash

clear

printf "Making all randomization scripts executable... "
chmod +x /home/smr/k388/sim/randoSim/scripts/*

printf "DONE!\nRandomizing object on loose type, location, and orientation... "
./randoObject.py
printf "DONE!\nGenerating fake laser data for optional debugging and testing... "
./randoFakeObjectPoints.py
printf "DONE!\nRandomizing guidemark selection and ordering... "
./randoGuidemarks.py

printf "DONE!\nUpdating SMR-CL script with randomized first guidemark... "
./randoSMRCLScript.py
printf "DONE!\nUpdating simserver config.xml file with walls of randomized object on loose... "
./randoConfigXML.py
printf "DONE!\nUpdating laser initializaton file with walls of randomized object on loose... "
./randoLaserINI.py

printf "DONE!\nCopying randoWalls.txt to /home/smr/k388/sim/... "
cp ../rando/randoWalls.txt /home/smr/k388/sim/randoWalls
printf "DONE!\nCopying constLines.txt to /home/smr/k388/sim/... "
cp ../const/constLines.txt /home/smr/k388/sim/constLines
printf "DONE!\nCopying randoSMRCL.txt to /home/smr/k388/sim/... "
cp ../rando/randoSMRCL.txt /home/smr/k388/sim/randoSMRCL
printf "DONE!\nCopying randoConfig.xml to /home/smr/k388/sim/... "
cp ../rando/randoConfig.xml /home/smr/k388/sim/randoConfig.xml
printf "DONE!\nCopying randoLaserINI.txt to /home/smr/k388/sim/ as NEW file... "
cp ../rando/randoLaserINI.txt /home/smr/k388/sim/ulmsserver.ini.NEW

printf "DONE!\nChanging directory to /home/smr/k388/sim/... "
cd /home/smr/k388/sim/
printf "DONE!\nCreating backup copy of current ulmsserver.ini file named ulmsserver.ini.bck... "
cp ulmsserver.ini ulmsserver.ini.bck
printf "DONE!\nRenaming ulmsserver.ini.NEW as ulmsserver.ini... "
mv ulmsserver.ini.NEW ulmsserver.ini
printf "DONE!\nYou are looking for...\n\n"
printf "*************************************************\n"
cat /home/smr/k388/sim/randoSim/simSummary/simSummaryObject.txt
printf "\n*************************************************\n"