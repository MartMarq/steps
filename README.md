# steps
Watchface for Watchy (SQFMI) - Steps

Simple watchface which calculates the average of todays steps and the number of steps of the last 6 days.

Pushing the right upper button switches the watchface for time to step overview.


![step1](https://github.com/MartMarq/steps/assets/139223739/4b9a880b-d276-4e1a-adae-abe0802dbbe6)
![step2](https://github.com/MartMarq/steps/assets/139223739/962ab6ab-2528-44b3-a8f0-e506441ae793)

Steps of more than 65535 on one day will lead to an error.

there is a time adjustment routine in Uhr.cpp

every day at 0:30 it sets back time by 15 seconds and 500 ms, because my Watchy runs too fast. Just delete it (or set another adjustment)
