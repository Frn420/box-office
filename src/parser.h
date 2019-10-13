#ifndef PARSER_H
#define PARSER_H

#include "time-format.h"

using namespace std;

typedef vector<pair<Time, string>> Route;
typedef pair<double, int> TicketInfo;
typedef pair<string, TicketInfo> Ticket;

static bool isNumber(char c);

static bool isCharFromStop(char c);

static bool checkShuttleFormat(string line);

static bool addShuttle(string line, map<int, Route> &shuttles);

static bool checkText(string text);

static bool checkIfItsTicketFormat(string line);

static bool addTicket(string line, set<Ticket> &tickets);

static bool checkAskingFormat(string line);

static bool checkShuttleTime(map<int, Route> shuttles, int shuttleId,
                             string &stop1, string &stop2, Time &arrival,
                             int &time);

static int findOptimalTickets(int time, set<Ticket> &tickets);

static bool askForTickets(string line, map<int, Route> &shuttles,
                          set<Ticket> &tickets, int &numberOfTickets);

void parse(string line, map<int, Route> &shuttles, set<Ticket> &tickets,
           int lineCounter, int &numberOfTickets);

#endif // PARSER_H