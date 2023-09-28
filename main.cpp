#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "restaurant_constructor.h"

using namespace std;
using namespace std::chrono;
//project 3: the food search
//Dorian DeJesus and Ashley Peters

void printMainMenu() {
    //print options menu
    cout << "The Food Search: Main Menu " << endl;
    cout << "-------------------------- " << endl;

    //1. print all options satisfying filters (with details)
    cout << "1. Print all restaurant options fitting current filters" << endl;

    //2. search by name
    cout << "2. Search for a restaurant by name (regardless of applied filters)" << endl;

    //3. filter by location
    cout << "3. Filter current restaurants by location" << endl;

    //4. filter by dietary options
    cout << "4. Filter current restaurants by dietary restrictions" << endl;

    //5. filter by price
    cout << "5. Filter current restaurants by price" << endl;

    //6. filter by cuisine
    cout << "6. Filter current restaurants by cuisine type" << endl;

    //7. review and be able to change favorited restaurants
    cout << "7. Review all favorite restaurants" << endl;

    //8. review and change search filters
    cout << "8. Review applied filters" << endl;

    //9. exit The Food Search
    cout << "9. Exit The Food Search" << endl;

    cout << "Please select an option from above: ";
}


void filterSuccessful(mapTree map, vector<pair<string, string>> &filters) {
    map.updateFitFilters(map.root, filters);
    cout << endl << "Filter Applied Successfully!" << endl;
    cout << "Would you like to print all restaurants fitting your current filters?" << endl;
    cout << "Type yes or no: ";
    string response = "";
    cin >> response;
    if (response == "yes" || response == "Yes" || response == "y" || response == "Y") {
        //print all restaurants matching current filters
        int count = 0;
        map.filteredInorderPrint(map.root, count);
    }
    else {
        cout << endl;
    }
}

void setFilterSuccessful(setTree set, vector<pair<string, string>> &filters) {
    set.updateFitFilters(set.root, filters);
    cout << endl << "Filter Applied Successfully!" << endl;
    cout << "Would you like to print all restaurants fitting your current filters?" << endl;
    cout << "Type yes or no: ";
    string response = "";
    cin >> response;
    if (response == "yes" || response == "Yes" || response == "y" || response == "Y") {
        //print all restaurants matching current filters
        int count = 0;
        set.filteredInorderPrint(set.root, count);
    }
    else {
        cout << endl;
    }
}

int main() {
    //print welcome message
    cout << "Welcome to The Food Search! " << endl;
    cout << "Press enter to begin!" ;
    cin.ignore();
    cout << endl;

    cout << "Would you like to use a map or a set for your data?" << endl;
    cout << "Please type map or set: ";
    string choice;
    cin >> choice;
    bool useMap = false;

    if (choice == "set" || choice == "Set") {
        useMap = false;
    }
    else {
        useMap = true;
    }

    auto start = high_resolution_clock::now(); // start time

    RestaurantData Data;
    mapTree map;
    setTree set;

    ifstream input;
    input.open("TheBigOne.txt");

    string line;
    string word;
    string name;
    string state;
    string type;
    string county;
    string diet;
    string price;

    int rating;
    int num = 0;

    //Referenced this to get the number of words
    //In the restaurant's name, the food types, and the county
    //Since the number of words describing those
    //Could be one or >3 words
    //https://stackoverflow.com/questions/34664964/c-counting-how-many-words-in-line
    while (getline(input, line)) {

        num++;

        bool got_name = false;
        bool got_type = false;
        bool got_county = false;

        struct restStruct newRestaurant;

        vector<string> words;
        vector<string>::iterator it;
        name = "";
        type = "";
        county = "";
        newRestaurant.fitFilters = true;
        newRestaurant.favorited = false;
        stringstream in(line);
        //While there are words in the line to be read
        while (in >> word) {
            //If a state hasn't been encountered yet and the name hasn't
            //been found, then the words belong to the title of the restaurant
            if (!Data.findState(word) && got_name == false) {
                words.push_back(word);
            }
                //If a state abbreviation has been found, stick all the words
                //that belong to the name into one string.
            else if (Data.findState(word)) {
                for (it = words.begin(); it != words.end(); ++it) {
                    name = name + *it;
                    if (it != words.end() - 1) {
                        name = name + " ";
                    }
                }
                //Assign the name to the currently new restaurant structure
                //and we've found the name so set that to true.
                //Additionally, since we've also found the state, assign
                //the state to the restaurant and clear the words vector
                newRestaurant.title = name;
                got_name = true;
                words.clear();
                newRestaurant.state = word;
            }
                //If we've found the name but not the dietary options, then we're reading in
                //the types of food this place has to offer. Same logic applies from above.
            else if (!Data.findDiet(word) && got_name == true && got_type == false) {
                words.push_back(word);
            }
            else if (Data.findDiet(word) && got_name == true && got_type == false) {
                for (it = words.begin(); it != words.end(); ++it) {
                    type = type + *it;
                    if (it != words.end() - 1) {
                        type = type + " ";
                    }
                }
                newRestaurant.cuisine = type;
                got_type = true;
                words.clear();
                newRestaurant.diet = word;
            }
                //If we haven't found the county and we haven't encountered the price,
                //then we're reading all the words in the county. Again, same thing as before.
            else if (!Data.findPrice(word) && got_type == true && got_county == false) {
                words.push_back(word);
            }
            else if (Data.findPrice(word) && got_type == true && got_county == false) {
                for (it = words.begin(); it != words.end(); ++it) {
                    county = county + *it;
                    if (it != words.end() - 1) {
                        county = county + " ";
                    }
                }
                newRestaurant.county = county;
                got_county = true;
                words.clear();
                newRestaurant.price = word;
            }
                //And then finally, if we've found all the multi-word values,
                //then we must be reading in the rating.
            else if (got_name == true && got_type == true && got_county == true) {
                newRestaurant.rating = stoi(word);
            }
        }
        //After reading in every word in the line, insert this new restaurant into
        //both the map (ordered alphabetically) and the set (ordered by the number it was added)
        if (useMap) {
            map.mapInsert(name, newRestaurant);
        }
        else {
            set.setInsert(num, newRestaurant);
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    if (useMap) {
        cout << "Time taken to create map: " << duration.count() << " microsecond(s)"<< endl << endl;
    }
    else {
        cout << "Time taken to create set: " << duration.count() << " microsecond(s)"<< endl << endl;
    }

    vector<pair<string, string>> filters; //{attribute, specific} eg. {state, FL} {pricing, E} {cuisine, Sports Bar}

    int option = 0;
    while (option != 9) {
        printMainMenu();
        cin >> option;
        cout << endl;

        if (option == 1) {
            //print all restaurants fitting currently activated filters
            auto start = high_resolution_clock::now(); // start time

            int count = 0;
            if (useMap) {
                map.updateFitFilters(map.root, filters);
                map.filteredInorderPrint(map.root, count);
            }
            else {
                set.updateFitFilters(set.root, filters);
                set.filteredInorderPrint(set.root, count);
            }

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time taken to print all entries: " << duration.count() << " microsecond(s)"<< endl << endl;

            int favorite = -1;
            while (favorite != 0) {
                cout << "If you would like to favorite any restaurant, type its index (type 0 if none): ";
                cin >> favorite;

                if (favorite == 0) {
                    cout << endl;
                }
                else {
                    int count = 0;
                    if (useMap) {
                        map.setFavorite(map.root, favorite, count);
                    }
                    else {
                        set.setFavorite(set.root, favorite, count);
                    }
                }
            }
        }
        else if (option == 2) {
            //search for a restaurant by name (regardless of applied filters)
            cout << "Which restaurant are you searching for? " << endl;
            cout << "Type a restaurant name or key words: ";
            string input;
            cin >> input;

            cout << endl << "Printing matching restaurants:" << endl;
            //using an inputted restaurant name or keyword, search all names to find any matches
            int count = 0;
            if (useMap) {
                map.searchedPrint(input, map.root, count);
            }
            else {
                set.searchedPrint(input, set.root, count);
            }
        }
        else if (option == 3) {
            //filter by location
            //cout << "Which location filter would you like to add to your search?" << endl;
            cout << "Are you looking to filter by state or county: ";   //determine whether location will be state or county
            string attribute = "";
            cin >> attribute;

            if (attribute == "state" || attribute == "State") {
                cout << "Type a state (by its capitalized two letter abbreviation: ";
                string input = "";
                cin >> input;

                //add input to filter vector
                filters.push_back({"State", input});
            }
            else {
                cout << "Type a county name: ";
                string input = "";
                cin >> input;

                filters.push_back({"County", input});
            }
            if (useMap) {
                filterSuccessful(map, filters);
            }
            else {
                setFilterSuccessful(set, filters);
            }
        }
        else if (option == 4) {
            //filter by dietary restriction
            cout << "Which dietary restriction filter would you like to add to your search?" << endl;
            cout << "Type \"Vegan\", \"Keto\", or \"Vegetarian\": ";
            string input = "";
            cin >> input;

            //add to filter vector based on input
            if (input == "vegan" || input == "Vegan") {
                filters.push_back({"Diet", "V"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else if (input == "keto" || input == "Keto") {
                filters.push_back({"Diet", "K"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else if (input == "vegetarian" || input == "Vegetarian") {
                filters.push_back({"Diet", "P"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else {
                cout << "Filter not recognized!" << endl;
            }
        }
        else if (option == 5) {
            //filter by price
            cout << "Which print filter would you like to add to your search?" << endl;
            cout << "Type \"Budget Friendly\", \"Average\", or \"Expensive\": ";
            string input = "";
            cin >> input;

            //add to filter vector based on their input
            if (input == "budget friendly" || input == "Budget Friendly") {
                filters.push_back({"Price", "B"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else if (input == "Average" || input == "average") {
                filters.push_back({"Price", "A"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else if (input == "expensive" || input == "Expensive") {
                filters.push_back({"Price", "E"});
                if (useMap) {
                    filterSuccessful(map, filters);
                }
                else {
                    setFilterSuccessful(set, filters);
                }
            }
            else {
                //unrecognized input
                cout << "Filter not recognized!" << endl;
            }
        }
        else if (option == 6) {
            //filter by cuisine type
            cout << "Which cuisine type filter would you like to add to your search?" << endl;
            cout << "Type a kind of cuisine: ";
            string input = "";
            cin >> input;

            filters.push_back({"Cuisine", input});
            if (useMap) {
                filterSuccessful(map, filters);
            }
            else {
                setFilterSuccessful(set, filters);
            }
        }
        else if (option == 7) {
            //print all restaurants which are currently favorited
            int count = 0;
            if (useMap) {
                map.favoritesInorderPrint(map.root, count);
            }
            else {
                set.favoritesInorderPrint(set.root, count);
            }
            if (count == 0) {
                cout << "You currently have no restaurants favorited" << endl << endl;
            }
        }
        else if (option == 8) {
            if (filters.empty()) {
                cout << "You currently have no filters applied" << endl << endl;
            }
            else {
                for (int i = 1; i <= filters.size(); i++) {
                    cout << i << ". " << filters.at(i - 1).first << ": " << filters.at(i - 1).second << endl;
                }
                cout << endl;

                cout << "Would like to reset all filters?" << endl;
                cout << "Type yes or no: ";
                string response = "";
                cin >> response;
                if (response == "yes" || response == "Yes" || response == "y" || response == "Y") {
                    filters.clear();
                    if (useMap) {
                        map.updateFitFilters(map.root, filters);
                    }
                    else {
                        set.updateFitFilters(set.root, filters);
                    }
                    cout << "All filters reset successfully" << endl;
                } else {
                    cout << endl;
                }
                cout << endl;
            }
        }
        else if (option == 9) {
            //exit while loop
        }
        else {
            //unrecognized input
            cout << "Input not recognized! Please try again." << endl;
        }
    }

    //exit message and ask about survey
    cout << "Thank you for using The Food Search!" << endl;
    cout << "Would you be willing to complete a brief survey on your experience?" << endl;
    cout << "Type yes or no: ";
    string survey = "";
    cin >> survey;

    if (survey == "yes" || survey == "Yes" || survey == "y" || survey == "Y") {
        //ask survey questions if responded yes, saving answers
        int responses[2];
        string bugs = "";
        cout << "On a scale from 1 - 10, how effective do you believe The Food Search was in helping you determine a restaurant to eat at today?: ";
        cin >> responses[0];
        cout << "On a scale from 1 - 10, how likely are you to recommend The Food Search to a friend or family member to use?: ";
        cin >> responses[1];
        cout << "Did you encounter any bugs or errors while using The Food Search (if so, please describe)?: ";
        cin >> bugs;
    }
    // take no or any other input as no and just continue
    cout << endl << "Thank you for your time!";

    return 0;
}
