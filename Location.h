#pragma once
#include <string>
using namespace std;

class Location {
private:
    string city;

public:
    Location() {};
    void setCity(string cityName);
    string getCity() const;
    ~Location() {};
};
