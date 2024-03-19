#!/bin/bash
echo 'hai'
for i in {1..1000..2}
do
	g++ -Wall -DLEADER_AMT=$i -DBIRD_AMT=10 -g -c leaderPathFinding.cpp
	g++ leaderPathFinding.cpp -DLEADER_AMT=$i -DBIRD_AMT=10 -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw2
	printf $i >> data/leaderPath.txt;
	printf %s ", " >> data/leaderPath.txt;
	./draw2 >> data/leaderPath.txt;
	echo $i;
done
