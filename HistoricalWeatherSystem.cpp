#include "HistoricalWeatherSystem.h"
#include <curl/curl.h>
#include <fstream>
#include<nlohmann/json.hpp>
#include"weatherForecastingSystem.h"
#include"weatherVariables.h"
#include<iostream>
using namespace std;
using json = nlohmann::json;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* userdata) {
    string* data = static_cast<string*>(userdata);
    data->append(buf, size * nmemb);
    return size * nmemb;
}

HistoricalWeatherSystem::HistoricalWeatherSystem() {}



bool HistoricalWeatherSystem::fetchHistoricalData(const std::string& location, const std::string& date) {
    //string url = "https://api.open-meteo.com/v1/forecast?location=" + location + "&start_date=" + date + "&end_date=" + date + "&hourly=temperature_2m";
    geocodeLocation(location);
    /*cout << Latitude << endl;
    cout << Longitude << endl;
    cout << date << endl;*/
    string url = "https://archive-api.open-meteo.com/v1/archive?latitude=" + Latitude + "&longitude=" + Longitude + "&start_date=" + date + "&end_date=" + date + "&hourly=temperature_2m,wind_speed_10m";// , precipitation, weather_code";

    cout<< url;
    CURL* curl = curl_easy_init();
    if (curl) {
        string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            //cout << response;
            return parseWeatherData(response);
        }
        else {
            cerr << "Error fetching weather data: " << curl_easy_strerror(res) << endl;
            return false;
        }
    }
    else {
        cerr << "Failed to initialize curl" << endl;
        return false;
    }
}

bool HistoricalWeatherSystem::parseWeatherData(const std::string& data) {
    try {
        auto jsonData = json::parse(data);
        //cout << jsonData << endl;
        if (jsonData.contains("hourly")) {
            const auto& hourly = jsonData["hourly"];
            size_t dataSize = hourly["time"].size();

            for (size_t i = 0; i < dataSize; ++i) {
                WeatherData weatherData;
                weatherData.date = hourly["time"][i];
                weatherData.temperature = hourly["temperature_2m"][i];
                weatherData.weather = hourly.contains("weather") ? hourly["weather"][i].get<string>() : "";
                weatherData.windSpeed = hourly.contains("wind_speed_10m") ? hourly["wind_speed_10m"][i].get<double>() : 0.0;
               
                historicalData.push_back(weatherData);
            }
            return true;
        }
        else {
            throw std::runtime_error("JSON structure does not contain expected keys.");
        }
    }
    catch (const json::exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return false;
    }
    catch (const std::runtime_error& e) {
        cerr << e.what() << endl;
        return false;
    }
}

void HistoricalWeatherSystem::displayHistoricalData() const {
    for (const auto& data : historicalData) {
        cout << "Date: " << data.date << endl;
        cout << "Temperature: " << data.temperature << " K" << endl;
       // cout << "Weather: " << data.weather << endl;
        cout << "Wind Speed: " << data.windSpeed << " m/s" << endl;
       //cout << "Humidity: " << data.humidity << "%" << endl;
      // cout << "Precipitation: " << data.precipitation << endl;
        cout << endl;
    }
}

bool HistoricalWeatherSystem::exportToCSV(const std::string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return false;
    }

    file << "Date,Temperature,Weather,Wind Speed,Humidity\n";
    for (const auto& data : historicalData) {
        file << data.date << "," << data.temperature << "," << data.weather << ","
            << data.windSpeed << "\n"; //<< ","<<data.humidity<<","<<data.precipitation<<"\n";
    }

    file.close();
    return true;
}

bool HistoricalWeatherSystem::exportToJSON(const std::string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return false;
    }

    json jsonData;
    for (const auto& data : historicalData) {
        json j;
        j["date"] = data.date;
        j["temperature"] = data.temperature;
        j["weather"] = data.weather;
        j["windSpeed"] = data.windSpeed;
       // j["humidity"] = data.humidity;
      //  j["precipitation"] = data.precipitation;
        jsonData.push_back(j);
    }

    file << jsonData.dump(4);
    file.close();
    return true;
}
pair<string, string> HistoricalWeatherSystem::geocodeLocation(const std::string& location)
{
    string url = "https://api.openweathermap.org/data/2.5/weather?q=" + location + "&APPID=b1b9900e4fe16c97bed323032c4a79eb";
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return std::make_pair("0.0", " 0.0"); // Return invalid coordinates
        }
        curl_easy_cleanup(curl);
    }

    try {
       //cout << readBuffer;
        auto json = nlohmann::json::parse(readBuffer);
       // cout << endl << json;
        if (json.contains("main") && json.contains("weather"))
        {
            latitude = json["coord"]["lat"];
            //cout <<endl<< latitude<<endl;
            Latitude = to_string(latitude);

            longitude = json["coord"]["lon"];
            // cout << endl << longitude << endl;
            Longitude = to_string(longitude);
            return std::make_pair(Latitude, Longitude);
        }
        else {
            std::cerr << "Error: No results found for location '" << location << "'" << std::endl;
            return std::make_pair("0.0", " 0.0"); // Return invalid coordinates
        }
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return std::make_pair("0.0", " 0.0"); // Return invalid coordinates
    }
}


string HistoricalWeatherSystem::historicalDataURLGenerator(const string& location, const string& date)
{
  // geocodeLocation(location);
    string url = "https://api.open-meteo.com/v1/archive?latitude=" + Latitude + "&longitude=" + Longitude + "& hourly = temperature_2m, relative_humidity_2m, wind_speed_10m"
        "&start_date =" + date + "&end_date =" + date;
    //string url = "https://api.open-meteo.com/v1/forecast?latitude=" + Latitude + "&longitude=" + Longitude + "&hourly=temperature_2m";
    return url;
}
