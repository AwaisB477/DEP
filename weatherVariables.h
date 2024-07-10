#pragma once
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using namespace std;

class weatherVariables {
private:
    float temperature,temperature_Min,temperature_Max,feelsLike;
    int pressure, humidity;
    string DataFetched, city;

public:
    float getTemperature();
    float getTemperature_Min();
    float getTemperature_Max();
    float getFeelsLike();
    int getPressure();
    int getHumidity();

    weatherVariables() {};
    void displayData();
    void dataHandle();
    void fetchData(string);
    ~weatherVariables() {};
};
