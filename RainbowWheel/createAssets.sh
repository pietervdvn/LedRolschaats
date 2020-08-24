#! /bin/bash

# Note: the order of inclusion is sorted by Ascii of the filename.
# Assets should be imported first
# THus: we need an UPPERCASE A to get started
cd assets
echo "// All assets are saved here" > "../Assets.ino"

for file in *
do
	NAME=`echo $file | sed "s/\..*$//"`
	echo "$file --> $NAME"
	echo -n "String $NAME = \"" >> "../Assets.ino"
	cat $file | sed 's/\\/\\\\/g' | sed 's/"/\\"/g' | tr "\n" " " >> "../Assets.ino"
	echo "\";" >> "../Assets.ino"
	echo >> "../Assets.ino"
done 
