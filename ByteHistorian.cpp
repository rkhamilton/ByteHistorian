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

    // initialize todays high/low values with ones which will be replaced immediately
	_yearsHighs[_currentDayOfYear] = 0;
	_yearsLows[_currentDayOfYear] = 255;

}

void ByteHistorian::setMinValue(float x)
{
	_minPossibleValue = x;
}

void ByteHistorian::setMaxValue(float x)
{
	_maxPossibleValue = x;
}

byte ByteHistorian::convertRawToByte(float x)
{
	const float stepsize = (_maxPossibleValue - _minPossibleValue) / 255;
	float converted = (x - _minPossibleValue)/stepsize;
	byte output = byte(constrain(converted,0,255));
	return output;
}

float ByteHistorian::convertByteToRaw(byte x)
{
	const float stepsize = (_maxPossibleValue - _minPossibleValue) / 255;
	float output = float(x) * stepsize + _minPossibleValue;
	return output;
}

void ByteHistorian::logValue(time_t t, float val)
{
	_todaysCurrentValue = convertRawToByte(val);

	//if it is now a different day of the year, take appropriate action
    if (_currentDayOfYear != calcDayOfYear(t))
    {
        // push today's values to yesterday and zero the days record for today
        for(int ii = 0; ii < _samplesPerDay; ii++)
        {
            _yesterdaysValues[ii] = _todaysValues[ii];
            _todaysValues[ii] = 0;
        }
        // update the day of the year
        _currentDayOfYear = calcDayOfYear(t);
        // reset the todaysHigh/Low values to the current value
        _yearsHighs[_currentDayOfYear] = _todaysCurrentValue;
        _yearsLows[_currentDayOfYear] = _todaysCurrentValue;

        _currentSampleToday = 0;
    }

	_yearsHighs[_currentDayOfYear] = max(_yearsHighs[_currentDayOfYear],	_todaysCurrentValue);
	_yearsLows[_currentDayOfYear] = max(_yearsLows[_currentDayOfYear],	_todaysCurrentValue);

    _currentSampleToday = max(indexOfTimeToday(t),_currentSampleToday);

	_todaysValues[indexOfTimeToday(t)] = _todaysCurrentValue;
}

void ByteHistorian::logValue(float val)
{
    logValue(now(),val);
}

float ByteHistorian::currentValue()
{
	return convertByteToRaw(_todaysCurrentValue);
}

float ByteHistorian::todaysLow()
{
	return convertByteToRaw(_yearsLows[_currentDayOfYear]);
}

float ByteHistorian::todaysHigh()
{
	return convertByteToRaw(_yearsHighs[_currentDayOfYear]);
}

inline unsigned int ByteHistorian::indexOfTimeToday(time_t t)
{
	return constrain(int(elapsedSecsToday(t) / _samplesPerDay),0,_samplesPerDay-1);

}

inline unsigned int ByteHistorian::calcDayOfYear(time_t t)
{
    int((now() - _previousStartOfYear) / SECS_PER_DAY);
}

void ByteHistorian::initializeHistoryArrays()
{
    // initialize today's array of values
    for(int ii = 0; ii < _samplesPerDay; ii++)
    {
        _todaysValues[ii] = 0;
        _yesterdaysValues[ii] = 0;
    }

    // initialize year's array of values
    for(int ii = 0; ii < HISTORIAN_DAYS_TO_STORE; ii++)
    {
        _yearsLows[ii] = 0;
        _yearsHighs[ii] = 0;
    }
}

byte ByteHistorian::getLowForDayAsByte(unsigned int idx)
{
    // if a value is requested that's out of bounds, return the extrema
    return _yearsLows[constrain(idx,0,_daysToStore)];
}

float ByteHistorian::getLowForDay(unsigned int idx)
{
    return convertByteToRaw(getLowForDayAsByte(idx));
}

byte ByteHistorian::getHighForDayAsByte(unsigned int idx)
{
    // if a value is requested that's out of bounds, return the extrema
    return _yearsHighs[constrain(idx,0,_daysToStore)];
}

float ByteHistorian::getHighForDay(unsigned int idx)
{
    return convertByteToRaw(getHighForDayAsByte(idx));
}

byte ByteHistorian::setHighForDayAsByte(unsigned int dayOfYear, byte val)
{
    _yearsHighs[constrain(dayOfYear,0,_daysToStore)] = val;
}

byte ByteHistorian::setLowForDayAsByte(unsigned int dayOfYear, byte val)
{
    _yearsLows[constrain(dayOfYear,0,_daysToStore)] = val;

}

byte ByteHistorian::getTodaysValueAsByte(unsigned int idx)
{
    return _todaysValues[constrain(idx,0,_samplesPerDay)];
}

float ByteHistorian::getTodaysValue(unsigned int idx)
{
    return convertByteToRaw(getTodaysValue(idx));
}

byte ByteHistorian::getYesterdaysValueAsByte(unsigned int idx)
{
    return _todaysValues[constrain(idx,0,_samplesPerDay)];
}

float ByteHistorian::getYesterdaysValue(unsigned int idx)
{
    return convertByteToRaw(getYesterdaysValue(idx));
}

ByteHistorian byteHistorian;

