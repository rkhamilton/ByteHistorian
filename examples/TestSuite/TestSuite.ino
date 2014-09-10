#include <Time.h>
#include <Streaming.h>
#include <ByteHistorian.h>
/*
 * TestSuite.ino
 *
 * Created: 9/7/2014 12:48:23 PM
 * Author: Ryan
 */

const float minValue = -6;
const float maxValue = 45;

void setup()
{
	Serial.begin(9600);

	testCurrentValues();
	testMinMax();


}

void loop()
{

}

void testCurrentValues()
{
        Serial << "Test: logging individual values" << endl;
		ByteHistorian temperature;
		float x;

		temperature.setMinValue(minValue);
		temperature.setMaxValue(maxValue);

		// run tests
		temperature.logValue(now(),minValue);
		x = temperature.currentValue();
		Serial << "Storing min value: " << (x == minValue) << endl;

		temperature.logValue(now(),minValue-1);
		x = temperature.currentValue();
		Serial << "Storing less than min value: " << (x == minValue) << endl;

		temperature.logValue(now(),maxValue);
		x = temperature.currentValue();
		Serial << "Storing max value: " << (x == maxValue) << endl;

		temperature.logValue(now(),minValue+1);
		x = temperature.currentValue();
		Serial << "Storing more than max value: " << (x == maxValue) << endl;

		Serial << "Todays high and low == min/max: " << (temperature.todaysHigh() == maxValue &&
		temperature.todaysLow() == minValue) << endl;


}

void testMinMax()
{
    Serial << "Test: confirm min/max works within a day" << endl;
	ByteHistorian temperature;

	temperature.setMinValue(minValue);
	temperature.setMaxValue(maxValue);

	// run tests
	temperature.logValue(now(),minValue + (maxValue - minValue)/10);
	temperature.logValue(now()+1,minValue + (maxValue - minValue)/2);
	Serial << "Todays low: " << (temperature.todaysLow() == minValue + (maxValue - minValue)/10) << endl;
	Serial << "Todays high: " << (temperature.todaysLow() == minValue + (maxValue - minValue)/2) << endl;
}


void testYearLogging()
{
    Serial << "Test: Simulate logging a year of data" << endl;
	ByteHistorian temperature;

	temperature.setMinValue(minValue);
	temperature.setMaxValue(maxValue);

	// log a years worth of data
    TimeElements tm;
    tm.Year = 2014;
    tm.Month = 9;
    tm.Day = 9;
    tm.Hour = 13;
    tm.Minute = 5;
    tm.Second = 23;
    time_t logTime = makeTime(tm);

    float valueToLog = 0;

    while (logTime <= logTime + SECS_PER_YEAR)
    {
        // every 12 hours ramp the value from 0 to 50
        valueToLog = (logTime % (SECS_PER_HOUR * 12)) / (SECS_PER_HOUR * 12) * 50;
    	temperature.logValue(logTime,valueToLog);
        logTime = logTime + 300; // try to log a value every five minutes
    }
	Serial << "Todays low: " << (temperature.todaysLow() == 0) << endl;
	Serial << "Todays high: " << (temperature.todaysLow() == 50) << endl;

    Serial << "Low from day 1: " << (temperature.getLowForDay(0) == 0) << endl;
    Serial << "High from day 1: " << (temperature.getHighForDay(0) == 50) << endl;
    Serial << "Low from day 365: " << (temperature.getLowForDay(365) == 0) << endl;
    Serial << "High from day 365: " << (temperature.getHighForDay(365) == 50) << endl;
    Serial << "Low from day 367: " << (temperature.getLowForDay(367) == 0) << endl;
    Serial << "High from day 367: " << (temperature.getHighForDay(367) == 50) << endl;



}