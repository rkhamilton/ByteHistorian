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

#define HISTORIAN_SAMPLES_PER_DAY 144
// 144 samples stores a value every 10 minutes
#define HISTORIAN_DAYS_TO_STORE 365
// store one year of data

class ByteHistorian
{
    private:
		const unsigned int _samplesPerDay = HISTORIAN_SAMPLES_PER_DAY;
		const unsigned int _daysToStore = HISTORIAN_DAYS_TO_STORE;
		const unsigned int _secondsBetweenSamples = 24*3600/HISTORIAN_SAMPLES_PER_DAY; // 24*3600 = seconds in a day
		byte _todaysValues[HISTORIAN_SAMPLES_PER_DAY];
		byte _yesterdaysValues[HISTORIAN_SAMPLES_PER_DAY];
        byte _yearsHighs[HISTORIAN_DAYS_TO_STORE];
        byte _yearsLows[HISTORIAN_DAYS_TO_STORE];
		float _minPossibleValue;
		float _maxPossibleValue;
		byte _todaysCurrentValue = 0;
        time_t _previousStartOfYear;
        unsigned int _currentDayOfYear;
        unsigned int _currentSampleToday;
		inline unsigned int indexOfTimeToday(time_t t);
        inline unsigned int calcDayOfYear(time_t t);
        void initializeHistoryArrays();

    public:
        ByteHistorian();
        void setMinValue(float x);
		void setMaxValue(float x);
		void logValue(time_t t, float val);
		void logValue(float val);
		float currentValue();
		float todaysLow();
		float todaysHigh();
		byte convertRawToByte(float x);
		float convertByteToRaw(byte x);
        float getHighForDay(unsigned int idx);
        float getLowForDay(unsigned int idx);
        byte getHighForDayAsByte(unsigned int idx);
        byte getLowForDayAsByte(unsigned int idx);
        byte setHighForDayAsByte(unsigned int dayOfYear, byte val);
        byte setLowForDayAsByte(unsigned int dayOfYear, byte val);
        byte getTodaysValueAsByte(unsigned int idx);
        float getTodaysValue(unsigned int idx);
        byte getYesterdaysValueAsByte(unsigned int idx);
        float getYesterdaysValue(unsigned int idx);
};

extern ByteHistorian byteHistorian;

#endif

