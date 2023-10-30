#!/bin/bash
echo 'hai'
for i in {10..100}
do
	g++ -Wall -DLEADER_AMT=$i -g -c leaderPathFinding.cpp
	g++ leaderPathFinding.o -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw2
	./draw2 >> data/aStarData.txt
	echo -n ", ";
	echo $i;
done
