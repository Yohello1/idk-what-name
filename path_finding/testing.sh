#!/bin/bash
echo 'hai'
for i in {10..10000..1}
do
	g++ -Wall -DLEADER_AMT=$i -g -c leaderMazeAStar.cpp
	g++ leaderMazeAStar.o -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw2
	printf $i >> data/aStarData.txt;
	printf %s ", " >> data/aStarData.txt;
	./draw2 >> data/aStarData.txt;
	echo $i;
done

