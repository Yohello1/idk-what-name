#include <iostream>
#include <string>
#include <array>
#include <cmath>
#include <array>
#include <queue>
#include <set>
#include <algorithm>
#include <map>
#include <list>
#include <sstream>

#ifndef BOARD_SIZE
  #define BOARD_SIZE 50
#endif

#ifndef SEED
  #define SEED 200
#endif

std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE> board;


int countFalsePositionsFromTopLeft() {

    int rows = board.size();
    int cols = board[0].size();
    int falseCount = 0;

    std::vector<int> dirX = {0, 0, -1, 1};
    std::vector<int> dirY = {-1, 1, 0, 0};

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    std::queue<std::pair<int, int>> q;

    if (!board[0][0]) {
        q.push({0, 0});
        visited[0][0] = true;
    }

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        falseCount++;
        for (int d = 0; d < 4; ++d) {
            int newX = x + dirX[d];
            int newY = y + dirY[d];
            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !visited[newX][newY] && !board[newX][newY]) {
                q.push({newX, newY});
                visited[newX][newY] = true;
            }
        }
    }

    return falseCount;
}


std::string runLengthEncode(const std::vector<bool>& arr) {
    if (arr.empty()) {
        return "";
    }

    std::ostringstream encodedStream;

    char currentChar = arr[0] ? 'T' : 'F';
    int count = 1;

    for (size_t i = 1; i < arr.size(); ++i) {
        char newChar = arr[i] ? 'T' : 'F';
        if (newChar == currentChar) {
            count++;
        } else {
            encodedStream << currentChar << count;
            currentChar = newChar;
            count = 1;
        }
    }
    encodedStream << currentChar << count; // Don't forget the last run

    encodedStream << ';';

    return encodedStream.str();
}


int main()
{
    // create board
    // solve board
    // compress board
    srand(SEED);


    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            int temp = rand() % 5;
            if(temp < 1)
                board[i][j] = true;
            else
                board[i][j] = false;
        }
    }




    int score = countFalsePositionsFromTopLeft();

    std::string outputMap = "";

    for(int i = 0; i < BOARD_SIZE; i ++)
    {
        std::vector<bool> tempVec;
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            tempVec.push_back(board[i][j]);
        }
        outputMap += runLengthEncode(tempVec);
    }

    std::cout << score << std::endl;
    std::cout << outputMap << std::endl;
}
