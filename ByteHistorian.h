/*
* ByteHistorian.h
*
* Created: 9/7/2014 12:49:12 PM
* Author: Ryan
*/

#ifndef _BYTEHISTORIAN_h
#define _BYTEHISTORIAN_h

#include <Time.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SAMPLES_PER_DAY 144
// 144 samples stores a value every 10 minutes
#define LONG_HISTORY_STORAGE 365
// store one year of data

class ByteHistorian
{
    private:
		const unsigned int _samplesPerDay = SAMPLES_PER_DAY;
		const unsigned int _secondsBetweenSamples = 24*3600/SAMPLES_PER_DAY; // 24*3600 = seconds in a day
		byte _todaysValues[SAMPLES_PER_DAY];
		byte _yesterdaysValues[SAMPLES_PER_DAY];
        byte _yearsHighs[LONG_HISTORY_STORAGE];
        byte _yearsLows[LONG_HISTORY_STORAGE];
		float _minValue;
		float _maxValue;
		byte _todaysHigh = 0;
		byte _todaysLow = 255;
		byte _todaysCurrentValue = 0;
        time_t _previousStartOfYear;
        unsigned int _currentDayOfYear;
		byte convertRawToByte(float x);
		float convertByteToRaw(byte x);
		void setupByteHistorian();
		inline unsigned int indexOfTimeToday(time_t t);
        inline unsigned int calcDayOfYear(time_t t);
        void endTheDay();
        void initializeHistoryArrays();

    public:
        ByteHistorian();
        void setMinValue(float x);
		void setMaxValue(float x);
		void logValue(time_t t, float val);
		float currentValue();
		float todaysLow();
		float todaysHigh();

};

extern ByteHistorian byteHistorian;

#endif

