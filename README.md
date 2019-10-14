# box-office
App helping box office selling tram tickets.

Input format:

1 add new shuttle to the timetable:
shuttle_number time_1 stop_name_1 ... time_n stop_name_n

2 add new ticket to the timetable:
ticket_name price time_of_validity

3 ask for tickets:
? stop_name_1 shuttle_number_1 stop_name_2 shuttle_number_2 ... stop_name_n shuttle_number_n stop_name_n+1

Output format:

! ticket_name; ticket_name_2; ...; ticket_name_n - set of tickets you have to buy

:-( stop_name_where_you_have_to_wait - if you have to wait on a stop

:-| - if there is more than 3 tickets you have to buy for the transfer.


compile file with this command:
g++ -Wall -Wextra -O2 -std=c++17 kasa.cc -o kasa
