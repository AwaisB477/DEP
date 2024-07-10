#include "weatherVariables.h"
#include <iomanip>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void weatherVariables::fetchData(string data) {
    DataFetched = data;
}

void weatherVariables::dataHandle() {
    try {
        auto jsonData = json::parse(DataFetched);
        //cout << DataFetched;
        if (jsonData.contains("main")) 
        {
            temperature = jsonData["main"]["temp"];
            feelsLike = jsonData["main"]["feels_like"];
            temperature_Min = jsonData["main"]["temp_min"];
            temperature_Max = jsonData["main"]["temp_max"];
            pressure = jsonData["main"]["pressure"];
            humidity = jsonData["main"]["humidity"];
            city = jsonData["name"];
        }
        else {
            throw std::runtime_error("JSON structure does not contain expected keys.");
        }
    }
    catch (const json::exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
    }
    catch (const std::runtime_error& e) {
        cerr << e.what() << endl;
    }
}

float weatherVariables::getTemperature()
{
    return temperature;
}

float weatherVariables::getTemperature_Min()
{
    return temperature_Min;
}

float weatherVariables::getTemperature_Max()
{
    return temperature_Max;
}

float weatherVariables::getFeelsLike()
{
    return feelsLike;
}

int weatherVariables::getPressure()
{
    return pressure;
}

int weatherVariables::getHumidity()
{
    return humidity;
}

void weatherVariables::displayData() {
    cout << fixed << setprecision(2);
    cout << "Weather in " << city << ":\n";
   //cout << "Description: " << description << " (" << main_weather << ")\n";
    cout << "Temperature: " << temperature << " K\n";
    cout << "Feels Like: " << feelsLike << " K\n";
    cout << "Min Temperature: " << temperature_Min << " K\n";
    cout << "Max Temperature: " << temperature_Max << " K\n";
    cout << "Pressure: " << pressure << " hPa\n";
    cout << "Humidity: " << humidity << "%\n";
}
