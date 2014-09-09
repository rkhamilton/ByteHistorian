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
	ByteHistorian temperature;
	
	temperature.setMinValue(minValue);
	temperature.setMaxValue(maxValue);

	// run tests
	temperature.logValue(now(),minValue + (maxValue - minValue)/10);
	temperature.logValue(now()+1,minValue + (maxValue - minValue)/2);
	Serial << "Todays low: " << (temperature.todaysLow() == minValue + (maxValue - minValue)/10) << endl;
	Serial << "Todays high: " << (temperature.todaysLow() == minValue + (maxValue - minValue)/2) << endl;
}


void testDayLogging()
{
	ByteHistorian temperature;
	
	temperature.setMinValue(minValue);
	temperature.setMaxValue(maxValue);

	// run tests
	temperature.logValue(now(),55);
	
}