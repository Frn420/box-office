#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

typedef pair<int, int> Time;
typedef vector<pair<Time, string>> Route;
typedef pair<double, int> TicketInfo;
typedef pair<string, TicketInfo> Ticket;

bool isItTime(Time time) {
    bool check = true;

    if (get<1>(time) < 0 || get<1>(time) > 59)
        check = false;

    if (get<0>(time) < 5 || get<0>(time) > 21)
        check = false;

    if (get<0>(time) == 5 && get<1>(time) < 55)
        check = false;

    if (get<0>(time) == 21 && get<1>(time) > 21)
        check = false;

    return check;
}

int whichTimeIsEarlier(Time time1, Time time2) {
    if (get<0>(time1) < get<0>(time2)) {
        return 1;
    }
    else if (get<0>(time1) > get<0>(time2)) {
        return 2;
    }
    else {
        if (get<1>(time1) < get<1>(time2))
            return 1;
        else if (get<1>(time1) > get<1>(time2))
            return 2;
        else
            return 0;
    }
}

int howManyMinutes(Time time1, Time time2) {
    return (get<0>(time2) - get<0>(time1)) * 60 + get<1>(time2) - get<1>(time1);
}

static bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

static bool isCharFromStop(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           c == '^';
}

static bool checkShuttleFormat(string line) {
    bool result = true;
    int index = 0, limit = line.size(), hour, minute;
    pair<int, int> time1 = make_pair(5, 54), time2;

    while (index < limit && isNumber(line[index]))
        index++;

    if (index + 1 >= limit || line[index] != ' ')
        result = false;

    index++;

    while (result && index < limit) {
        if (isNumber(line[index])) {
            if (line[index] == '0' &&
                (line[index + 1] == '0' || line[index + 1] == ':'))
                result = false;

            if (result && line[index + 1] == ':')
                if (index + 4 >= limit || !isNumber(line[index + 2]) ||
                    !isNumber(line[index + 3]) || line[index + 4] != ' ')
                    result = false;

            if (result && isNumber(line[index + 1]) && line[index + 2] == ':')
                if (index + 5 >= limit || !isNumber(line[index + 3]) ||
                    !isNumber(line[index + 4]) || line[index + 5] != ' ')
                    result = false;

            if (result) {
                stringstream(line.substr(index, limit - index))
                >> hour >> minute;
                time2 = make_pair(hour, minute);

                if (!isItTime(time2) || whichTimeIsEarlier(time1, time2) != 1)
                    result = false;

                if (get<0>(time2) < 10)
                    index += 5;
                else
                    index += 6;
            }
        }
        else {
            result = false;
        }

        while (result && index < limit && line[index] != ' ') {
            if (!isCharFromStop(line[index]))
                result = false;

            index++;
        }

        if (index < limit)
            index++;

        time1 = time2;
    }

    return result;
}

static bool addShuttle(string line, map<int, Route> &shuttles) {
    int shuttleId, hour, minute;
    long unsigned int index = 0;
    string stopName;
    Route stopInfo;
    if (checkShuttleFormat(line)) {
        stringstream(line) >> shuttleId;

        while (line[index] != ' ')
            index++;

        index++;
        line = line.substr(index, line.size() - index);

        while (!line.empty()) {
            stringstream(line) >> hour >> minute;

            index = 0;
            while (line[index] != ' ')
                index++;

            index++;
            line = line.substr(index, line.size() - index);
            stringstream(line) >> stopName;

            stopInfo.emplace_back(make_pair(hour, minute), stopName);

            index = 0;
            while (index < line.size() && line[index] != ' ')
                index++;

            if (index < line.size())
                index++;

            line = line.substr(index, line.size() - index);
        }

        shuttles.insert(pair<int, vector<pair<pair<int, int>, string>>>
                        (shuttleId, stopInfo));

        return true;
    }
    else {
        return false;
    }
}

static bool checkText(string text) {
    bool result = true;

    for (char c: text)
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' '))
            result = false;

    if (text[text.size() - 1] == ' ')
        result = false;

    return result;
}

static bool checkIfItsTicketFormat(string line) {
    bool result = true;
    int index = 1, limit = line.size();

    while (index < limit) {
        if (line[index] == '.') {
            if (index + 3 < limit && isNumber(line[index + 1]) &&
                isNumber(line[index + 2]) && line[index + 3] == ' ') {
                index += 3;
                result = true;
                break;
            }
        }

        result = false;
        index++;
    }

    if (index == limit)
        result = false;

    index++;

    while (index < limit) {
        if (!isNumber(line[index]))
            result = false;
        index++;
    }

    return result;
}

static bool addTicket(string line, set<Ticket> &tickets) {
    int index = 0, timeOfValidity;
    double ticketPrice;
    string ticketName;
    TicketInfo ticketInfo;

    while (line[index] != ' ' ||
           line[index + 1] < '0' || line[index + 1] > '9') {
        ticketName += line[index];
        index++;
    }

    if (checkText(ticketName) &&
        checkIfItsTicketFormat(line.substr(index, line.size() - index))) {
        stringstream(line.substr(index, line.size() - index))
        >> ticketPrice >> timeOfValidity;

        ticketInfo = make_pair(ticketPrice, timeOfValidity);
        tickets.insert(make_pair(ticketName, ticketInfo));

        return true;
    }
    else {
        return false;
    }
}

static bool checkAskingFormat(string line) {
    bool result = true;
    int index = 1, limit = line.size();

    if (line[index] != ' ')
        result = false;

    index++;

    while (result && index < limit && line[index] != ' ') {
        if (!isCharFromStop(line[index]))
            result = false;

        index++;
    }

    while (result && index < limit) {
        if (index + 1 >= limit)
            result = false;
        index++;

        while (result && index < limit && line[index] != ' ') {
            if (!isNumber(line[index]))
                result = false;

            index++;
        }

        if (line[index] != ' ')
            result = false;
        index++;

        while (result && index < limit && line[index] != ' ') {
            if (!isCharFromStop(line[index]))
                result = false;

            index++;
        }
    }

    return result;
}

static bool checkShuttleTime(map<int, Route> shuttles, int shuttleId,
                             string &stop1, string &stop2, Time &arrival,
                             int &time) {
    bool result = true, stop1check = false, stop2check = false;
    Route route;
    Time departure;

    if (shuttles.count(shuttleId) == 0) {
        result = false;
    }
    else {
        route = shuttles[shuttleId];
        for (pair<Time, string> p: route) {
            if (stop1 == get<1>(p)) {
                stop1check = true;
                departure = get<0>(p);
            }

            if (stop1check && stop2 == get<1>(p)) {
                stop2check = true;
                arrival = get<0>(p);
            }
        }

        if (stop1check && stop2check)
            time += howManyMinutes(departure, arrival);
        else
            result = false;
    }

    return result;
}

static int findOptimalTickets(int time, set<Ticket> &tickets) {
    int currentTime;
    double price = numeric_limits<double>::max(), currentPrice;
    string ticket1 = "_", ticket2 = "_", ticket3 = "_";

    for (Ticket t: tickets) {
        if (get<1>(get<1>(t)) >= time && get<0>(get<1>(t)) < price) {
            ticket1 = get<0>(t);
            price = get<0>(get<1>(t));
        }
    }

    for (Ticket t1: tickets) {
        for (Ticket t2: tickets) {
            currentTime = get<1>(get<1>(t1)) + get<1>(get<1>(t2));
            currentPrice = get<0>(get<1>(t1)) + get<0>(get<1>(t2));
            if (currentTime >= time && currentPrice < price) {
                ticket1 = get<0>(t1);
                ticket2 = get<0>(t2);
                price = currentPrice;
            }
        }
    }

    for (Ticket t1:tickets) {
        for (Ticket t2:tickets) {
            for (Ticket t3:tickets) {
                currentTime = get<1>(get<1>(t1)) + get<1>(get<1>(t2)) +
                              get<1>(get<1>(t3));
                currentPrice = get<0>(get<1>(t1)) + get<0>(get<1>(t2)) +
                               get<0>(get<1>(t3));
                if (currentTime >= time && currentPrice < price) {
                    ticket1 = get<0>(t1);
                    ticket2 = get<0>(t2);
                    ticket3 = get<0>(t3);
                    price = currentPrice;
                }
            }
        }
    }

    if (ticket1 == "_") {
        cout << ":-|" << endl;
        return 0;
    }
    else if (ticket2 == "_") {
        cout << "! " << ticket1 << endl;
        return 1;
    }
    else if (ticket3 == "_") {
        cout << "! " << ticket1 << "; " << ticket2 << endl;
        return 2;
    }
    else {
        cout << "! " << ticket1 << "; " << ticket2 << "; " << ticket3 << endl;
        return 3;
    }
}

static bool askForTickets(string line, map<int, Route> &shuttles,
                          set<Ticket> &tickets, int &numberOfTickets) {
    bool result = true;
    int shuttleId, totalTime = 0, time2 = 0;
    unsigned long int index = 2;
    string stop1, stop2;
    Time arrivalTime, departureTime;

    if (checkAskingFormat(line)) {
        line = line.substr(index, line.size() - index);
        index = 0;

        stringstream(line) >> stop1;
        while (line[index] != ' ')
            index++;

        index++;
        line = line.substr(index, line.size() - index);
        index = 0;

        while (result && !line.empty()) {
            stringstream(line) >> shuttleId;
            while (line[index] != ' ')
                index++;

            index++;
            line = line.substr(index, line.size() - index);
            index = 0;

            stringstream(line) >> stop2;
            while (index < line.size() && line[index] != ' ')
                index++;

            if (index < line.size()) {
                index++;
            }

            line = line.substr(index, line.size() - index);
            index = 0;

            result = checkShuttleTime(shuttles, shuttleId, stop1, stop2,
                                      arrivalTime, totalTime);

            if (result && time2 != 0) {
                if (totalTime - time2 !=
                    howManyMinutes(departureTime, arrivalTime)) {
                    cout << ":-( " << stop1 << endl;
                    break;
                }
            }

            time2 = totalTime;
            stop1 = stop2;
            departureTime = arrivalTime;
        }

        if (totalTime - time2 == howManyMinutes(departureTime, arrivalTime)) {
            numberOfTickets += findOptimalTickets(totalTime, tickets);
        }
    }
    else {
        result = false;
    }

    return result;
}

void parse(string line, map<int, Route> &shuttles, set<Ticket> &tickets,
           int lineCounter, int &numberOfTickets) {
    bool check = true;

    if (line[0] >= '0' && line[0] <= '9') {
        check = addShuttle(line, shuttles);
    }
    else if ((line[0] >= 'a' && line[0] <= 'z') ||
             (line[0] >= 'A' && line[0] <= 'Z')) {
        check = addTicket(line, tickets);
    }
    else if (line[0] == '?') {
        check = askForTickets(line, shuttles, tickets, numberOfTickets);
    }
    else if (!line.empty()) {
        check = false;
    }

    if (!check)
        cerr << "Error in line " << lineCounter << ": " << line << endl;
}

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
