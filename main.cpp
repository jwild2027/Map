#include <iostream>
#include <string>
#include <limits>
#include "index.hpp"


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: spatial_demo <path-to-csv>\n";
        return 2;
    }
    const std::string csv = argv[1];

    SpatialIndex index;
    try {
        index.loadCSV(csv, true);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }

    auto states = index.getStates();
    if (states.empty()) {
        std::cerr << "No states found in CSV.\n";
        return 1;
    }
    auto st = states.front();

    auto cities = index.getCities(st);
    if (cities.empty()) {
        std::cerr << "No cities found for state: " << st << "\n";
        return 1;
    }
    auto ci = cities.front();

    auto center = index.getCenter(st, ci);
    if (!center) {
        std::cerr << "No center for " << ci << ", " << st << "\n";
        return 1;
    }

    std::cout << "Selected: " << ci << ", " << st
              << " (center: " << center->lat0 << ", " << center->lon0 << ")\n";

    std::cout << "Type in distance from city center (km): ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cerr << "Failed to read input.\n";
        return 1;
    }

    double radius_km = 0.0;
    try {
        size_t pos = 0;
        radius_km = std::stod(line, &pos);
        if (pos != line.size()) {
            std::cerr << "Invalid number format.\n";
            return 1;
        }
        if (radius_km < 0.0) {
            std::cerr << "Radius must be non-negative.\n";
            return 1;
        }
    } 

    auto pts = index.queryKm(st, ci, radius_km);
    std::cout << "Houses within " << radius_km << " km of " << ci << ", " << st
              << ": " << pts.size() << "\n";

    return 0;
}
