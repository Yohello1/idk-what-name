#include <bits/stdc++.h>

int main()
{
    std::vector< std::pair< int, std::pair<int, std::string> > > input; // stores -grade, height, name

    int N, M;
    std::cin >> N >> M;

    for(int i = 0; i < N; i++){
        int grade, height; std::string name;
        std::cin >> name >> grade >> height;

        input.push_back({-grade, {height, name}});
    }
    std::sort(input.begin(), input.end());

    std::vector<std::string> out;
    for(int i = 0; i < M; i++) out.push_back(input[i].second.second);
    std::sort(out.begin(), out.end());

    for(auto name : out) std::cout << name << "\n";

    return 0;
}
