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
typedef pair<double, unsigned> TicketInfo;
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

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

bool isCharFromStop(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           c == '^';
}

bool checkShuttleFormat(string line) {
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
                stringstream(line.substr(index, limit - index)) >> hour;

                if (line[index] > '2')
                    index += 2;
                else
                    index += 3;
                stringstream(line.substr(index, limit - index)) >> minute;

                time2 = make_pair(hour, minute);

                if (!isItTime(time2) || whichTimeIsEarlier(time1, time2) != 1)
                    result = false;

                index += 3;
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

bool checkLoop(Route &route, string &stopName) {
    for (pair<Time, string> p: route)
        if (get<1>(p) == stopName)
            return false;

    return true;
}

bool addShuttle(string line, map<unsigned, Route> &shuttles) {
    int hour, minute;
    unsigned shuttleId;
    long unsigned int index = 0;
    string stopName;
    Route stopInfo;
    if (checkShuttleFormat(line)) {
        stringstream(line) >> shuttleId;

        if (shuttles.count(shuttleId) == 1)
            return false;

        while (line[index] != ' ')
            index++;

        index++;
        line = line.substr(index, line.size() - index);

        while (!line.empty()) {
            stringstream(line) >> hour;

            if (line[0] > '2')
                line = line.substr(2, line.size() - 2);
            else
                line = line.substr(3, line.size() - 3);

            stringstream(line) >> minute;

            index = 0;
            while (line[index] != ' ')
                index++;

            index++;
            line = line.substr(index, line.size() - index);
            stringstream(line) >> stopName;

            if (checkLoop(stopInfo, stopName)) {
                stopInfo.emplace_back(make_pair(hour, minute), stopName);
            }

            index = 0;
            while (index < line.size() && line[index] != ' ')
                index++;

            if (index < line.size())
                index++;

            line = line.substr(index, line.size() - index);
        }

        shuttles.insert(pair<int, Route>(shuttleId, stopInfo));

        return true;
    }
    else {
        return false;
    }
}

bool checkText(string &text, set<Ticket> &tickets) {
    bool result = true;

    for (char c: text)
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' '))
            result = false;

    for (Ticket t: tickets)
        if (text == get<0>(t))
            result = false;

    return result;
}

bool checkIfItsTicketFormat(string line) {
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

bool addTicket(string line, set<Ticket> &tickets) {
    int index = 0;
    unsigned length = line.size(), timeOfValidity;
    double ticketPrice;
    string ticketName;
    TicketInfo ticketInfo;

    while (index + 1 < length && (line[index] != ' ' ||
           line[index + 1] < '0' || line[index + 1] > '9')) {
        ticketName += line[index];
        index++;
    }

    if (checkText(ticketName, tickets) &&
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

bool checkAskingFormat(string line) {
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

    if (index >= limit)
        result = false;

    while (result && index < limit) {
        if (index + 1 >= limit)
            result = false;
        index++;

        while (result && index < limit && line[index] != ' ') {
            if (!isNumber(line[index]))
                result = false;

            index++;
        }

        if (index >= limit || line[index] != ' ')
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

bool checkShuttleTime(map<unsigned, Route> shuttles, int shuttleId,
                      string &stop1, string &stop2, Time &arrival, int &time) {
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

int findOptimalTickets(int time, set<Ticket> &tickets) {
    unsigned currentTime;
    double price = numeric_limits<double>::max(), currentPrice;
    string ticket1 = "_", ticket2 = "_", ticket3 = "_";

    for (Ticket t: tickets) {
        if (get<1>(get<1>(t)) > time && get<0>(get<1>(t)) < price) {
            ticket1 = get<0>(t);
            price = get<0>(get<1>(t));
        }
    }

    for (Ticket t1: tickets) {
        for (Ticket t2: tickets) {
            currentTime = get<1>(get<1>(t1)) + get<1>(get<1>(t2));
            currentPrice = get<0>(get<1>(t1)) + get<0>(get<1>(t2));
            if (currentTime > time && currentPrice < price) {
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
                if (currentTime > time && currentPrice < price) {
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

bool askForTickets(string line, map<unsigned, Route> &shuttles,
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

void parse(string line, map<unsigned, Route> &shuttles, set<Ticket> &tickets,
           int lineCounter, int &numberOfTickets) {
    bool check = true;

    if (line[0] >= '0' && line[0] <= '9') {
        check = addShuttle(line, shuttles);
    }
    else if ((line[0] >= 'a' && line[0] <= 'z') ||
             (line[0] >= 'A' && line[0] <= 'Z') || line[0] == ' ') {
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
    map<unsigned, Route> shuttles;
    set<Ticket> tickets;

    while (getline(cin, line)) {
        parse(line, shuttles, tickets, lineCounter, numberOfTickets);
        lineCounter++;
    }

    cout << numberOfTickets << endl;

    return 0;
}