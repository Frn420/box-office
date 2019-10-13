#ifndef TIME_FORMAT_H
#define TIME_FORMAT_H

#include <utility>

using namespace std;

typedef pair<int, int> Time;

bool isItTime(Time time);

int whichTimeIsEarlier(Time time1, Time time2);

int howManyMinutes(Time time1, Time time2);

#endif // TIME_FORMAT_H