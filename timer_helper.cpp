#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

std::string GetTimeStringFromMilliseconds(int milliseconds, int accuracy)
{
	constexpr int millisecondsInHour = 1000 * 60 * 60;
	constexpr int millisecondsInMinute = 1000 * 60;
	constexpr int millisecondsInSecond = 1000;
	constexpr int maxAccuracy = 3;

	const int hours = milliseconds / millisecondsInHour;
	milliseconds %= millisecondsInHour;
	const int minutes = milliseconds / millisecondsInMinute;
	milliseconds %= millisecondsInMinute;
	const int seconds = milliseconds / millisecondsInSecond;
	milliseconds %= millisecondsInSecond;

	std::stringstream timeString;
	timeString << std::setfill('0');
	if (hours > 0)
	{
		timeString << hours << ':' << std::setw(2);
	}
	timeString << minutes << ':' << std::setw(2) << seconds;
	if (accuracy > 0)
	{
		accuracy = std::min(accuracy, maxAccuracy);
		const int divisor = static_cast<int>(std::pow(10, maxAccuracy - accuracy));
		const int printedDecimals = milliseconds / divisor;
		timeString << '.' << std::setw(accuracy) << printedDecimals;
	}

	return timeString.str();
}
