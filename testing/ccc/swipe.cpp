#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define INF 1e9

int n = 0;
std::vector<int> Aline;
std::vector<int> Bline;

int findDesired(int desiredInt, int currPosition)
{
    int position = -1;

    bool found = false;

    int l = currPosition, r = currPosition;
    while(!found)
    {

        // if found, go to its end?
        if(Aline[l] == desiredInt)
        {
            found = true;
            position = l;
            break;
        }

        if(Aline[r] == desiredInt)
        {
            found = true;
            position = r;
            break;
        }

        if(l != -1)
            l--;
        if(r != n)
            r++;
        if((l == -1) && (r == (n)))
            found = true;
    }

    // everything inbetween gets set to that number in arrayA
    if(position != -1)
    {
        for(int i = std::min(currPosition, position); i < std::max(currPosition, position); i++)
        {
                Aline[i] = desiredInt;
        }
    }

    return position;
}

int main()
{
    std::cin >> n;

    for(int i = 0; i < n; i++)
    {
        int tempInt = 0;
        std::cin >> tempInt;
        Aline.push_back(tempInt);
    }

    for(int i = 0; i < n; i++)
    {
        int tempInt = 0;
        std::cin >> tempInt;
        Bline.push_back(tempInt);
    }


    std::vector<std::pair<char, std::pair<int, int>>> output;
    bool outputStuff = true;

    // ok now we just itterate through uhh, A, yes
    for(int i = 0; i < n; i++)
    {
        // now we search from our current position to nearest desired int
        int desiredInt = Bline[i];
        // if(findDesired(desiredInt, i) == -1)
        // {
        //     std::cout << "NO" << std::endl;
        //     break;
        // }

        int positionFound = findDesired(desiredInt, i);
        if(positionFound == -1)
        {
            std::cout << "NO" << '\n';
            outputStuff = false;
            break;
        }

        // right if position > i
        // left is position < i
        // nothing if positon == i
        // R/L i position
        else if(positionFound == i)
        {
        } // nothing LMAO

        else if(positionFound < i)
        {
            std::pair<char, std::pair<int, int>> temp;
            temp.first = 'R';
            temp.second.first = std::min(positionFound, i);;
            temp.second.second = std::max(positionFound, i);
            output.push_back(temp);

        }
        else if(positionFound > i)
        {
            std::pair<char, std::pair<int, int>> temp;
            temp.first = 'L';
            temp.second.first = std::min(positionFound, i);;
            temp.second.second = std::max(positionFound, i);
            output.push_back(temp);

        }
    }




    // cleaning up output
    // bandage solution,, one i wanted to use did not work :(
    int currCheck = 0;
    while((currCheck+1) < output.size())
    {
        if(output[currCheck].first == output[currCheck+1].first)
        {
            // std::cout << "found" << output[currCheck].second.first << ' ' <<  output[currCheck+1].second.first << std::endl;
            if(output[currCheck].second.first == output[currCheck+1].second.first)
            {
                output[currCheck].second.second = output[currCheck+1].second.second;
                output.erase(output.begin()+currCheck);
            }
            else
            {
                currCheck++;
            }
        }
        else
        {
            currCheck++;
        }
    }

    if((outputStuff == true) && (output.size() >= n))
    {
        outputStuff = false;
        std::cout << "NO";
    }


    if(outputStuff == true)
    {
        std::cout << "YES" << '\n';
        std::cout << output.size() << '\n';
        for(int i = 0; i < output.size(); i++)
        {
            std::cout << output[i].first << ' ' << output[i].second.first << ' ' << output[i].second.second << '\n';

        }
    }
}
