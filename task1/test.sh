#!/bin/bash

mkdir D_1
touch F_1
mkdir D_1/D_1_2
touch D_1/F_1_2

mkdir d_2
touch d_2/F_2
mkdir d_2/D_2_1

clear
echo "1)---------Tests"
echo "Take a look to the files|directory"
ls -R1
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo "2)--------- -h"
./modify.sh -h
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo "3)-------- -l (./modify.sh -l F_1) "
echo -e "\nBefore:\n"
ls
./modify.sh -l F_1
echo -e "\nAfter:\n"
ls
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
./modify.SH -u f_1
clear

echo "4)-------- -u (./modify.sh -u d_2)"
echo -e "\nBefore:\n"
ls
./modify.sh -u d_2
echo -e "\nAfter:\n"
ls
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
./modify.sh -l D_2
clear

echo "5)-------- -r -l (./modify.sh -r -l D_1)"
echo -e "\nBefore:\n"
ls *
./modify.sh -r -l D_1
echo -e "\nAfter:\n"
ls *
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo "6)-------- -r -u (./modify.sh -r -u d_1)"
echo -e "\nBefore:\n"
ls *
./modify.sh -r -u d_1
echo -e "\nAfter:\n"
ls *
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo -e "7)-------- sed pattern (./modify.sh 's/_/-/' x"
echo -e "\nIt's not working I didn't manage to success."
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo -e "8)-------- -u + Nothing (./modify.sh -u)\n"
./modify.sh -u
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo -e "9)-------- -u NotExistingFile (./modify.sh -u NotExistingFile)\n"
./modify.sh -u NotExistingFile
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear

echo -e "10)------- -u D_2 (./modify.sh -u D_2)\n"
./modify.sh -u D_2
echo -e "\n----------------------------------"
echo -e "\nPress any key to see the next step\n"
read pause
clear


rm -r D_1
rm -r d_2
rm f_1

