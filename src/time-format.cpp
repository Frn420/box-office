#include <utility>
#include "time-format.h"

using namespace std;

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