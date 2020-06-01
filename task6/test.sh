#!/bin/bash

touch sourceFile
echo -e "This is a sentence.\n" >> sourceFile

clear
echo -e "1)---------Tests\n"
ls
echo -e "\ncat sourceFile:"
cat sourceFile
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "2)---------copy (without any argument)\n"
./copy
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "3)---------copy -h\n"
./copy -h
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "4)---------copy --help\n"
./copy --help
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "5)---------copy -t (random argument)\n"
./copy -t
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "6)---------copy sourceFile (no target file)\n"
./copy sourceFile
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "7)---------copy srcFile target (unexisting file)\n"
./copy srcFile target
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "8)---------copy -m srcFile target (unexisting file + -m)\n"
./copy -m srcFile target
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "9)---------copy -m sourceFile (-m without right number of arguments)\n"
./copy -m srcFile
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "10)---------copy -m srcFile target test_ (too much arguments)\n"
./copy -m srcFile target test_
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

echo -e "11)---------copy sourceFile target\n"
./copy sourceFile target
ls
echo -e "\ncat target:"
cat target
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear


echo -e "12)---------copy -m traget m_target\n"
./copy -m target m_target
ls
echo -e "\ncat m_target:"
cat m_target
echo -e "\n__________________________________"
echo -e "Press any key to see the next step\n"
read pause
clear

rm sourceFile target m_target