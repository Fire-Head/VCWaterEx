#pragma once
#include "..\address.h"

enum eWeather
{
	WEATHER_SUNNY = 0,
	WEATHER_CLOUDY = 1,
	WEATHER_RAINY = 2,
	WEATHER_FOGGY = 3,
	WEATHER_EXTRA_SUNNY = 4,
	WEATHER_HURRICANE = 5,
	WEATHER_EXTRA_COLOURS = 6
};

class CWeather
{
public:
	static float &Wind;
	static float &WindClipped;
	static float &SunGlare;
	static short &NewWeatherType;
};