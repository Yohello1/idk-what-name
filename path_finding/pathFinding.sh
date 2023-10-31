#!/bin/bash
echo 'hai'
for i in {5..5000..2}
do
	g++ -Wall -DLEADER_AMT=$i -g -c leaderPathFinding.cpp
	g++ leaderPathFinding.cpp -DLEADER_AMT=$i -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw2
	printf $i >> data/leaderPath.txt;
	printf %s ", " >> data/leaderPath.txt;
	./draw2 >> data/leaderPath.txt;
done
