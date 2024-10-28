#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

struct City {
    int id;
    double x, y;
};

// Function to calculate Euclidean distance between two cities
double euclideanDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to calculate total cost of a tour
double calculateTourCost(const vector<City>& cities, const vector<int>& tour) {
    double totalCost = 0.0;
    for (size_t i = 0; i < tour.size(); ++i) {
        totalCost += euclideanDistance(cities[tour[i]], cities[tour[(i + 1) % tour.size()]]);
    }
    return totalCost;
}

// Function to perform Simulated Annealing for TSP
void simulatedAnnealing(const vector<City>& cities, vector<int>& bestTour, double& bestCost) {
    int numCities = cities.size();
    if (numCities == 0) return; // No cities to process

    vector<int> currentTour(numCities);
    
    // Initialize current tour
    for (int i = 0; i < numCities; ++i) {
        currentTour[i] = i;
    }
    
    // Randomly shuffle the initial tour
    random_device rd;
    mt19937 g(rd());
    shuffle(currentTour.begin(), currentTour.end(), g);
    
    bestTour = currentTour;
    bestCost = calculateTourCost(cities, bestTour);

    double temperature = 10000.0;
    double coolingRate = 0.995;

    for (int iteration = 0; iteration < 10000; ++iteration) {
        // Generate a neighbor solution by swapping two cities
        vector<int> newTour = currentTour;
        int city1 = rand() % numCities;
        int city2 = rand() % numCities;
        
        // Ensure city1 and city2 are different
        while (city1 == city2) {
            city2 = rand() % numCities;
        }
        
        swap(newTour[city1], newTour[city2]);

        double newCost = calculateTourCost(cities, newTour);
        
        // Accept new solution based on cost and temperature
        if (newCost < bestCost || exp((calculateTourCost(cities, currentTour) - newCost) / temperature) > ((double) rand() / RAND_MAX)) {
            currentTour = newTour; // Accept new tour
            if (newCost < bestCost) {
                bestTour = newTour; // Update best tour
                bestCost = newCost; // Update best cost
            }
        }

        // Cool down the temperature
        temperature *= coolingRate;
    }
}

// Function to read city data from file
bool readCityData(const string& filename, vector<City>& cities) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    City city;
    while (inputFile >> city.id >> city.x >> city.y) {
        cities.push_back(city);
    }

    inputFile.close();
    return true;
}

int main() {
    vector<City> cities;

    // Read city data from file (replace "cities.txt" with your dataset path)
    if (!readCityData("cities.txt", cities)) {
        return 1; // Exit if file reading fails
    }

    // Check if cities were loaded
    if (cities.empty()) {
        cerr << "No cities were loaded from the file." << endl;
        return 1;
    }

    vector<int> bestTour;
    double bestCost;

    simulatedAnnealing(cities, bestTour, bestCost);

    // Output final cost and solution
    cout << "Final achieved cost: " << bestCost << endl;
    cout << "Solution (sequence of cities): ";
    
    for (int city : bestTour) {
        cout << city << " ";
    }
    
    cout << endl;

    return 0;
}
