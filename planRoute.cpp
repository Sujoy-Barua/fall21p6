// Name: Sujoy Barua
// Lab Section: 020
// Date Submitted: 12/02/2021

//add all the libraries and namespace std

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;


//creating necesary structs

struct Location {
    int gridRow;
    int gridCol;
    int sRow;
    int sCol;
    int eRow;
    int eCol;
        
};

struct Planet {
    int Row;
    int Col;
    char Sym;
    int Id;
    string Name;
};


int main() {
    
    //prompting user for input file names and storing them to variables
    string locFile;
    string namesFile;
    cout << "Enter Locations Filename: ";
    cin >> locFile;
    cout << "Enter Names Filename: ";
    cin >> namesFile;

    //opening streams for inputted ( by user ) files
    ifstream locIn(locFile);
    if (!locIn.is_open()) {
        cout << "Input file could not be opened" <<endl;
        return 1;
    }
    ifstream nameIn(namesFile);
    if (!nameIn.is_open()) {
        cout << "Input file could not be opened" <<endl;
        return 1;
    }
    
    //reading in the locaton file
    Location location;
    locIn >> location.gridRow >> location.gridCol >> location.sRow >> location.sCol >> location.eRow >> location.eCol;
    
    Planet planet;
    vector<Planet> planetVec;
    while (locIn >> planet.Row >> planet.Col >> planet.Sym >> planet.Id) {
        if (planet.Row <= location.gridRow && planet.Col <= location.gridCol && planet.Row > 0 && planet.Col > 0) {
            planetVec.push_back(planet);
        } else {
            cout << planet.Id << " out of range - ignoring" << endl;
        }
        
       
    }

    //reading in planet names to Planet struct where id is matching
    int planetID;
    string planetName;
    while (nameIn >> planetID >> planetName) {
        for (size_t i = 0; i < planetVec.size(); ++i) {
            if (planetVec.at(i).Id == planetID) {
                planetVec.at(i).Name = planetName;
            }
        }
    }

    //closing input streams
    locIn.close();
    nameIn.close();

    //cleaning the names by removing XX and replacing _ with blank space
    for (size_t i = 0; i < planetVec.size(); ++i) {
        
        while (planetVec.at(i).Name.find("XX") != string::npos) {
            int index = planetVec.at(i).Name.find("XX");
            planetVec.at(i).Name.erase(planetVec.at(i).Name.begin() + index, planetVec.at(i).Name.begin() + index + 2);
        }

        while (planetVec.at(i).Name.find("_") != string::npos) {
            int index = planetVec.at(i).Name.find("_");
            planetVec.at(i).Name.replace(index, 1, " ");
        }
    }

    //create a map filled with number of periods equal to the number of rows and columns within the driver's range
    vector<string> periods(location.gridCol, ".");
    vector<vector<string> > map(location.gridRow, periods);

    //replace periods with appropriate symbols
    map.at(location.sRow-1).at(location.sCol-1) = "S";
    map.at(location.eRow-1).at(location.eCol-1) = "E";

    for (size_t i = 0; i < planetVec.size(); ++i) {
        map.at((planetVec.at(i).Row)-1).at((planetVec.at(i).Col)-1) = planetVec.at(i).Sym;
    }
        



    //create output stream
    ofstream fout("journey.txt");

    //output the map to output stream
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map.at(i).size(); ++j) {
            fout << map.at(i).at(j);
        }
        fout << endl;
    }
    
    //plan the route to take
    int currentLocRow = location.sRow;
    int currentLocCol = location.sCol;
    int potentialLocRow;
    int potentialLocCol;
    int actualLocRow;
    int actualLocCol;
    double nDist;
    double oDist = 10000;
    int minIndex = 0;
    vector<string> visited (planetVec.size(), "false");

    //output start location
    fout << "Start at " << currentLocRow << " " << currentLocCol << endl;
    
    //loop that calculates distance and determines which planet to go next
    //runs until all planets have been visited
    for (size_t i = 0; i < planetVec.size(); ++i) {
        oDist = 10000;
            for (size_t j = 0; j < visited.size(); ++j) {
                if (visited.at(j) == "false") {
                    potentialLocRow = planetVec.at(j).Row;
                    potentialLocCol = planetVec.at(j).Col;
                    nDist = sqrt(pow((currentLocRow - potentialLocRow),2) + pow((currentLocCol - potentialLocCol),2));
                    //runs if new distance is less than old distance
                    if (nDist < oDist) {
                        oDist = nDist;
                        minIndex = j;
                        actualLocRow = planetVec.at(minIndex).Row;
                        actualLocCol = planetVec.at(minIndex).Col;
                    }
                    //runs if new distance is equal to old distance
                    else if (nDist == oDist) {
                            if (planetVec.at(j).Id < planetVec.at(minIndex).Id) {
                            minIndex = j;
                            }
                        actualLocRow = planetVec.at(minIndex).Row;
                        actualLocCol = planetVec.at(minIndex).Col;
                    }
                    
                
                }
            }
    //going to the new location and setting the planet as visited
    currentLocRow = actualLocRow;
    currentLocCol = actualLocCol;
    visited.at(minIndex) = "true";

    //outputting where to go next
    fout << "Go to " << planetVec.at(minIndex).Name << " at " << actualLocRow << " " << actualLocCol << endl;
    }

    //outputting end location
    fout << "End at " << location.eRow << " " << location.eCol << endl;

    //closing output stream
    fout.close();
}
