#!/bin/bash
echo 'hai'
for i in {4544..6000..3}
do
	g++ -Wall -DLEADER_AMT=$i -g -c leaderMazeAStar.cpp
	g++ leaderMazeAStar.o -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw3
	printf $i >> data/aStarData.txt;
	printf %s ", " >> data/aStarData.txt;
	./draw3 >> data/aStarData.txt;
	echo $i;
done

