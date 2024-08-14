#include <bits/stdc++.h>

using namespace std;
int n, k;

void degreeMatrix(const vector<vector<int>>& graph, vector<vector<int>>& degree )
{
    for(int i = 0; i < n; i++)
    {
        int q = 0;
        for(int j = 0; j < n; j++)
        {
            q += graph[j][i];
        }
        degree[i][i] = q;
    }
}

int main()
{
    std::cout << "nodes, edges\n";

    cin >> n >> k;

    vector<vector<int>> graph(n, vector<int>(n, 0));
    vector<vector<int>> degree(n, vector<int>(n, 0));


    cout << "u v\n";

    for(int i = 0; i < k; i++)
    {
        int l,m;
        cin >> l >> m;
        graph[l][m] = graph[m][l] += 1;
    }


    degreeMatrix(graph, degree);

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << degree[i][j];
        }
        cout << "\n";
    }
}
