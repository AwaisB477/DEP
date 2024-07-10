#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
class HistoricalWeatherSystem {
public:
    struct WeatherData {
        std::string date;
        double temperature;
        std::string weather;
        double windSpeed;
       // int humidity;
       // string precipitation;
    };

    HistoricalWeatherSystem();
    pair<string, string> geocodeLocation(const std::string& location);
    bool fetchHistoricalData(const std::string& location, const std::string& date);
    void displayHistoricalData() const;
    bool exportToCSV(const std::string& filename) const;
    bool exportToJSON(const std::string& filename) const;
    string historicalDataURLGenerator(const string& location, const string& date);
private:
    std::vector<WeatherData> historicalData;
    bool parseWeatherData(const std::string& data);
    float longitude, latitude;
    string Latitude, Longitude;
};
