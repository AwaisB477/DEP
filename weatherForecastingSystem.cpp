#include "weatherForecastingSystem.h"
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "HistoricalWeatherSystem.h"
string weatherForecastingSystem::urlGenerator(const string& location) {
    //geocodeLocation(location, "05680f708fc343819a90d6cdb3bb1249");
    string url = "https://api.openweathermap.org/data/2.5/weather?q=" + location + "&APPID=b1b9900e4fe16c97bed323032c4a79eb";
    return url;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    
    return size * nmemb;
}

