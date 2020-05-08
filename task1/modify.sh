#!/bin/bash


#--------------------
#help display

if [ $1 == -h ] || [ $1 == --help ] || [ $1 == ]	#display help when -h|--help or when nothing is written after "./modify.sh"
then
	echo -e "\n  modify [-r] [-l|-u] <dir/file names...>"
	echo -e "  modify [-r] <sed pattern> <dir/file names...>"
	echo -e "  modify [-h]\n\n"
	echo -e "  -l dedicated to lowerizing file names"
	echo -e "  -u dedicated to uppercasing file names"
	echo -e "  -r apply lowerizing|uppercasing recursively"
	echo -e "  -h, --help display this help and exit\n"
	exit
fi

#------------------
#number of arg + name=last arg

argNum=$#	#count number of args inserted
name=$(eval echo \$$argNum)	#define "name" as the last argument inserted
argumentSed=""	#basic argument for sed

#------------------
#check if file|directory exist

if [ ! -f $name ] && [ ! -d $name ]
then
	echo "modify: cannot modify '$name': No such file or directory"
	exit
fi


#-------------------
#Function

lowerizing()
{
	_name=$1	#first arg of the function
	l_name=${_name,,}	#lowerrized char
	if [ $l_name != $_name ]	#check if we are not already in lowercasing
	then
		mv $_name $l_name
	else
		if [ "$2" != -r ] #"$2" and not just $2 because if $2 is empty the test is like : if [ != -r ]
                then
                        echo "modify: invalid use -- '$1'"
                        echo "Try 'modify --help' for more information."
                        exit
                fi
	fi
}

uppercasing()
{
	_name=$1	#first arg of the function
	u_name=${_name^^}	#uppercased char
	if [ $u_name != $_name ]	#check if we are not already in uppercasing
	then
		mv $_name $u_name
	else
		if [ "$2" != -r ]
                then
                 	echo "modify: invalid use -- '$1'"
                       	echo "Try 'modify --help' for more information."
			exit
                fi
	fi
}

sedPath()
{
	sed_name=$(echo "$name" | sed $argumentSed)	#sed _name take the sed path
	mv $name $sed_name
}


#--------------------
#Case where -r is not used

if [ "$1" != -r ]	#check if recursiv is not called
then
	case $1 in

		-l) 	lowerizing $name	#call lowerizing function
			exit
			;;

		-u) 	uppercasing $name	#call uppercasing function
			exit
			;;

		*) 	if [ $name != $1 ]
		   	then
				sedPath $name	#call sedPath Function
		   	else
				 echo -e "modify: invalid option -- '$1'\nTry 'modify --help' for more information."
		   	fi
			exit
			;;
	esac
fi

#--------------------
#Case where -r is used

if [ "$1" == -r ] && [ -d $name ]	#check if recursiv is called and the directory selected is a directory
then
	dir=$3
	cd $dir		#focus on the concerned directory

	ls -R1 > ../fileList.txt 	#create a tempfile (remove at the end) to stock all the files contained in the folder "name"

	find < ../fileList.txt > ../path_fileList.txt	#arrange formatting to display acces path and create an other tempfile (remove at the end)

	while read line
	do
		case $2 in

			-l) 	lowerizing $line -r	#call lowerizing function
				if [ -d $line ]		#check if the current line of "path_fileList.txt" is a directory
				then
					sed -e -i 's/'$line'/\L&/' ../path_fileList.txt #renames the folder for the next file in this folder example: the directory "TEST" is lowerizing so it becomes "test" but the acces path for (for example "IN_TEST") the files in "test" are wrong in "path_fileList.txt". So here we rename so that it works
        			fi
				;;

			-u) 	uppercasing $line -r	#call uppercasing function
				if [ -d $line ]
				then
                                        sed -e -i 's/'$line'/\U&/' ../path_fileList.txt #same as above but in uppercasing case
	                        fi

				;;

			*) 	echo -e "modify: invalid option -- '$2'\nTry 'modify --help$"
         			exit
				;;
		esac

	done < ../path_fileList.txt #to use all the line of the file "path_fileList.txt

	rm ../fileList.txt #remove  temp file as said previously
	rm ../path_fileList.txt	#remove temp file as said previoulsy

	cd ..
	case $2 in #change the name of the directory selected at the beginning

		-l)	lowerizing $name
			;;

		-u)	uppercasing $name
			;;
	esac
else

	echo "modify: cannot modify '$name': Because '$name' is not a directory"
fi
