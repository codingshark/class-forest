#!/bin/sh

if [ -z $1 ] || [ $1 = "" ]
then
	echo "Usage:\t\t$0 file_name"
	echo "Example:\t$0 dot_file.gv"
	echo "file_name:\tPath of a DOT script file"
else
	dot -Tsvg -o ./$1.svg $1
fi


