YearByteHistorian
=================

This Arduino library implements a simple data historian which records:

- The current value of the historized input
- N measurements per day for the past one day
- The current day's min/max value
- Min and max values seen on each day for the past one year

All values are recorded with one byte of precision in order to reduce memory usage. When creating a new historian instance one must specify the min and max values which you would like to record. Anything outside of that range will be clipped, and all values inside that range will be quantized to 1/255 of the specified range. Some examples:

- Storing temperature in the range -6 to 45 Celsius (covering reasonable weather conditions for my location) with steps of (45 C+ 6 C) / 255 = 0.2 Celsius.
- Storing humidity values in the range 0 to 100% with steps of 100% / 255 = 0.4 %

#Usage


#Acknowledgments
* 