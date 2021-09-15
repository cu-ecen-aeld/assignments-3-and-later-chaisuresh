#!/bin/sh
# Author: Chaithra Suresh

FILESDIR=$1
searchstr=$2
#echo "hello"

if [ $# -lt 2 ]
then
	echo "please input two command line arguments"
	echo "first- path to a directory  second- text string to find"
	exit 1
fi

#if [ -d "$FILESDIR" ]
#then
	#exit 0
#else
	#echo "File path not found"
	#exit 1
#fi


numfiles=$(grep -Rl $searchstr $FILESDIR |  wc -l)
matchfiles=$(grep -r $searchstr $FILESDIR | wc -l)
echo "The number of files are ${numfiles} and the number of matching lines are ${matchfiles}"

