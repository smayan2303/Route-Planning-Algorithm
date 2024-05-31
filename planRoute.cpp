
//import everything I need
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>
#include <cmath>

//use namespace std
using namespace std;

//make a struct to store all the planet's data when we load it in
struct Planet {
  //initialize all variables
    int Row;
    int Column;
    string Symbol;
    int ID;
    string Name;
};

//make the function that loads the data in from the file
void loadFromLoc(vector<Planet> &fleet, istream &input){

  //make an object from struct Planet
  Planet planet; 

  // Continue reading data until the end of the file is reached
  while (input >> planet.Row >> planet.Column >> planet.Symbol >> planet.ID) {
    //add the new planet data to fleet
    fleet.push_back(planet);
  }
}


int main() {
  //make a string to collect user input for locations and names filenames
  string locations;
  cout << "Enter Locations Filename: ";
  cin >> locations;
  string names;
  cout << "Enter Names Filename: ";
  cin >> names;

  //make the input file streams for locations and names files
  ifstream loc(locations);
  ifstream name(names);

  //check if loc opened correctly
  if(!loc.is_open()) {
    cout << "Input file could not be opened" << endl;
    return 1;
  }
//check if name opened correctly
  if(!name.is_open()) {
    cout << "Input file could not be opened" << endl;
    return 1;
  }

  //make variables to hold the starting data from the locations file
  int numRows,numColumns,startRow,startCol,endRow,endCol;
  //read in the starting data
  loc >> numRows >> numColumns >> startRow >> startCol >> endRow >> endCol;
  //make a vector of structs to store planet data in
  vector<Planet> planetList;
  //call the loadFromLoc function to read in the planet data into the vector of structs
  loadFromLoc(planetList, loc);
  //make variables for Ids and Names
  int Ids;
  string Names;
  //read in data for Ids and Names from name
    while(name >> Ids >> Names) {
      //loop through every planet
      for(size_t i = 0; i < planetList.size(); ++i){
        //check if the Ids match
        if(Ids == planetList.at(i).ID) {
          //if they do set the Name of that planet to the variable Name
          planetList.at(i).Name = Names;
        }
      }
    }

  //loop through the planets to find out if any of them are outside the given row and column range
  //if they are, erase them from the list and print a message
  for(size_t i = 0; i < planetList.size(); i++) {
  
    if (planetList.at(i).Column < 1 || planetList.at(i).Row > numRows || planetList.at(i).Row < 1 || planetList.at(i).Column > numColumns){
      cout << planetList.at(i).ID << " out of range - ignoring" << endl;
      planetList.erase(planetList.begin() + i);
      i--;
      }
    
    }

  //make variables for what we are looking for and their replacements
  string looking1 = "_";
  string replace1 = " ";
  string looking2 = "XX";
  string replace2 = "";

  int size = planetList.size();
  //code for replacing the underscores with spaces and deleting the "XX"
  for(int i = 0; i < size; ++i) {
    string string = planetList.at(i).Name;
    while (string.find(looking1) != string.npos) {
      int index = string.find(looking1);
      planetList.at(i).Name = string.replace(index,looking1.size(),replace1);
    }
    while (string.find(looking2) != string.npos) {
      int index = string.find(looking2);
      planetList.at(i).Name = string.replace(index,looking2.size(),replace2);
    }
  }


  //code for creating the map with all periods and replacing the start and end with S and E
  vector <string> row(numColumns, ".");

  vector <vector<string>> map(numRows,row);
  map.at(startRow - 1).at(startCol - 1) = "S";
  map.at(endRow - 1).at(endCol - 1) = "E";

  //add in the planets to the map
  for(size_t i = 0; i < planetList.size(); ++i) {
    int aRow = planetList.at(i).Row - 1;
    int aCol = planetList.at(i).Column - 1;
    map.at(aRow).at(aCol) = planetList.at(i).Symbol;
  }

  //make the output stream for journey
  ofstream jin("journey.txt");

  //add the map to the output file
  for (size_t i = 0; i < map.size();i++) {
    for (size_t j = 0; j < map[i].size(); j++) {
      jin << map.at(i).at(j);
    }
    jin << endl;
  }

  //set the current location and print the starting line to the output file
  vector<int> currentLoc = {startRow, startCol};
  jin << "Start at " << startRow << " " << startCol << endl;


  //loop while planet size is greater than 0
  while (planetList.size() > 0) {
  //set the min equal to the distance from the first planet
  double min = sqrt((pow(currentLoc.at(0) - planetList.at(0).Row, 2)) + (pow(currentLoc.at(1) - planetList.at(0).Column, 2)));
  //new location is the location of the first planet
  vector<int> newLoc = {planetList.at(0).Row, planetList.at(0).Column};
  //set index and ID to that of the first planet
  int index = 0;
  int Ids = planetList.at(0).ID;

  //loop through all available planets
  for(size_t i = 0; i < planetList.size(); ++i) {
    //if the distance is less than the current min
    if(sqrt((pow(currentLoc.at(0) - planetList.at(i).Row, 2)) + (pow(currentLoc.at(1) - planetList.at(i).Column, 2))) < min) {
      //set the new min and all associated values to that of the new closest planet
      min = sqrt((pow(currentLoc.at(0) - planetList.at(i).Row, 2)) + (pow(currentLoc.at(1) - planetList.at(i).Column, 2)));
      newLoc = {planetList.at(i).Row, planetList.at(i).Column};
      index = i;
      Ids = planetList.at(i).ID;
    }

    //if the distance is equal to the current min
    if(sqrt((pow(currentLoc.at(0) - planetList.at(i).Row, 2)) + (pow(currentLoc.at(1) - planetList.at(i).Column, 2))) == min) {
      //check which planet has the lower ID and save the info of that planet
      if(planetList.at(i).ID < Ids) {
        min = sqrt((pow(currentLoc.at(0) - planetList.at(i).Row, 2)) + (pow(currentLoc.at(1) - planetList.at(i).Column, 2)));
      newLoc = {planetList.at(i).Row, planetList.at(i).Column};
      index = i;
      Ids = planetList.at(i).ID;
      }
    }
  }
  //set the current loc to the planet we just went to
  currentLoc = newLoc;
  //add this information to the journey file
  jin << "Go to " << planetList.at(index).Name << " at " << planetList.at(index).Row << " " << planetList.at(index).Column << endl;
  //erase that planet so we dont go there again
  planetList.erase(planetList.begin() + index);


  }
  //add in the line for the ending location
  jin << "End at " << endRow << " " << endCol << endl;

}


