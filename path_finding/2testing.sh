#!/bin/bash
echo 'hai'
for i in {6000..10000..3}
do
        g++ -Wall -DLEADER_AMT=$i -g -c 2leaderMazeAStar.cpp
        g++ 2leaderMazeAStar.o -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -o draw4
        printf $i >> data/aStarData2.txt;
        printf %s ", " >> data/aStarData2.txt;
        ./draw4 >> data/aStarData2.txt;
        echo $i;
done
