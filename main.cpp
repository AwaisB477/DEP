#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include "weatherVariables.h"
#include "weatherForecastingSystem.h"
#include "Location.h"
#include "HistoricalWeatherSystem.h"

using namespace std;
weatherVariables variables;

static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    string data((const char*)buf, size * nmemb);
   // cout << endl << data << endl;
    variables.fetchData(data);
    return size * nmemb;
}

int main() {
    string apiKey = "05680f708fc343819a90d6cdb3bb1249";
    HistoricalWeatherSystem historicalSystem;
    weatherForecastingSystem forecastingSystem;

    while (true) {
        cout << "Weather Data Management System" << endl;
        cout << "1. Fetch Real-Time Weather Data" << endl;
        cout << "2. Fetch Historical Weather Data" << endl;
        cout << "3. Display Historical Weather Data" << endl;
        cout << "4. Export Data to CSV" << endl;
        cout << "5. Export Data to JSON" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            string location;
            cout << "Enter Your Location: ";
            cin >> location;
            string url = forecastingSystem.urlGenerator(location);

            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

                res = curl_easy_perform(curl);

                if (res != CURLE_OK) {
                    cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
                }
                else {
                    variables.dataHandle();
                    variables.displayData();
                }

                curl_easy_cleanup(curl);
            }
            else {
                cerr << "Failed to initialize curl" << endl;
            }

            curl_global_cleanup();
            break;
        }
        case 2: {
            string location, date;
            cout << "Enter Location: ";
            cin >> location;
            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> date;

            if (historicalSystem.fetchHistoricalData(location, date)) {
                cout << "Historical weather data fetched successfully." << endl;
            }
            else {
                cout << "Failed to fetch historical weather data." << endl;
            }
            break;
        }
        case 3:
            historicalSystem.displayHistoricalData();
            break;
        case 4: {
            string filename;
            cout << "Enter filename to export CSV: ";
            cin >> filename;

            if (historicalSystem.exportToCSV(filename)) {
                cout << "Data exported to CSV successfully." << endl;
            }
            else {
                cout << "Failed to export data to CSV." << endl;
            }
            break;
        }
        case 5: {
            string filename;
            cout << "Enter filename to export JSON: ";
            cin >> filename;

            if (historicalSystem.exportToJSON(filename)) {
                cout << "Data exported to JSON successfully." << endl;
            }
            else {
                cout << "Failed to export data to JSON." << endl;
            }
            break;
        }
        case 6:
            cout << "Exiting the program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}
