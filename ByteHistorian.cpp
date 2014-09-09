/*
 * ByteHistorian.cpp
 *
 * Created: 9/7/2014 12:49:12 PM
 * Author: Ryan
 */

#include "ByteHistorian.h"


ByteHistorian::ByteHistorian()
{
    initializeHistoryArrays();

    // figure out what day of the year it is
    TimeElements tm;
    tm.Year = year(now());
    tm.Month = 1;
    tm.Day = 0;
    tm.Hour = 0;
    tm.Minute = 0;
    tm.Second = 0;
    _previousStartOfYear = makeTime(tm);
    _currentDayOfYear = calcDayOfYear(now());

}

void ByteHistorian::setMinValue(float x)
{
	_minValue = x;
}

void ByteHistorian::setMaxValue(float x)
{
	_maxValue = x;
}

byte ByteHistorian::convertRawToByte(float x)
{
	const float stepsize = (_maxValue - _minValue) / 255;
	float converted = (x - _minValue)/stepsize;
	byte output = byte(constrain(converted,0,255));
	return output;
}

float ByteHistorian::convertByteToRaw(byte x)
{
	const float stepsize = (_maxValue - _minValue) / 255;
	float output = float(x) * stepsize + _minValue;
	return output;
}

void ByteHistorian::logValue(time_t t, float val)
{
	_todaysCurrentValue = convertRawToByte(val);

	//if it is now a different day of the year, take appropriate action
    if (_currentDayOfYear != calcDayOfYear(t))
    {
        endTheDay();
        // update the day of the year
        _currentDayOfYear = calcDayOfYear(t);
    }

	_todaysHigh = max(_todaysHigh,	_todaysCurrentValue);
	_todaysLow	= min(_todaysLow,	_todaysCurrentValue);

	_todaysValues[indexOfTimeToday(t)] = _todaysCurrentValue;

}

float ByteHistorian::currentValue()
{
	return convertByteToRaw(_todaysCurrentValue);
}

float ByteHistorian::todaysLow()
{
	return convertByteToRaw(_todaysLow);
}

float ByteHistorian::todaysHigh()
{
	return convertByteToRaw(_todaysHigh);
}

inline unsigned int ByteHistorian::indexOfTimeToday(time_t t)
{
	return constrain(int(elapsedSecsToday(t) / _samplesPerDay),0,_samplesPerDay-1);

}

inline unsigned int ByteHistorian::calcDayOfYear(time_t t)
{
    int((now() - _previousStartOfYear) / SECS_PER_DAY);
}

void ByteHistorian::endTheDay()
{
    // TODO log the day's min/max
    // reset the todaysHigh/Low values to the current value
    _todaysHigh = _todaysCurrentValue;
    _todaysLow = _todaysCurrentValue;
    // push today's values to yesterday and zero the days record for today
    for(int ii = 0; ii < _samplesPerDay; ii++)
    {
        _yesterdaysValues[ii] = _todaysValues[ii];
        _todaysValues[ii] = 0;
    }

}

void ByteHistorian::initializeHistoryArrays()
{
    // TODO replace this with reads from flash memory

    // initialize today's array of values
    for(int ii = 0; ii < _samplesPerDay; ii++)
    {
        _todaysValues[ii] = 0;
        _yesterdaysValues[ii] = 0;
    }

    // initialize year's array of values
    for(int ii = 0; ii < LONG_HISTORY_STORAGE; ii++)
    {
        _yearsLows[ii] = 0;
        _yearsHighs[ii] = 0;
    }
}

ByteHistorian byteHistorian;

