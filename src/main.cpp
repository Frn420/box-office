#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "parser.h"

using namespace std;

int main() {
    int lineCounter = 1, numberOfTickets = 0;
    string line;
    map<int, Route> shuttles;
    set<Ticket> tickets;

    while (getline(cin, line)) {
        parse(line, shuttles, tickets, lineCounter, numberOfTickets);
        lineCounter++;
    }

    cout << numberOfTickets << endl;

    return 0;
}