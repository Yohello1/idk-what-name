// C++ program to demonstrate
// the working of STL queue of pairs

#include <bits/stdc++.h>
using namespace std;

// Print the current pair
void printPair(pair<int, int> p)
{
    // Gives first element from queue pair
    int f = p.first;

    // Gives second element from queue pair
    int s = p.second;

    cout << "(" << f << ", " << s << ") ";
}

// Print the Queue of Pairs
void showQueue(queue<pair<int, int> > gq)
{
    // Print element until the
    // queue is not empty
    while (!gq.empty()) {
        printPair(gq.front());
        gq.pop();
    }

    cout << '\n';
}

// Driver code
int main()
{
    queue<pair<int, int> > gq;

    // Pushing elements inside
    // the queue container
    gq.push({ 10, 20 });
    gq.push({ 15, 5 });
    gq.push({ 1, 5 });
    gq.push({ 5, 10 });
    gq.push({ 7, 9 });

    cout << "Queue of Pairs: ";
    showQueue(gq);

    // Prints size of queue
    cout
        << "\nSize of Queue of Pairs: "
        << gq.size();

    // Prints first element
    // of queue container
    cout << "\nFront of Queue of Pairs: ";
    printPair(gq.front());

    // Prints last element
    // of queue container
    cout << "\nBack of Queue of Pairs: ";
    printPair(gq.back());

    cout << "\n\nRemoving the Front pair\n";
    gq.pop();
    cout << "Current Queue of Pairs: ";
    showQueue(gq);

    return 0;
}
