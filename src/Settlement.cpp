#include "Settlement.h"
#include <iostream>

//Constrector:
Settlement:: Settlement(const string &name, SettlementType type)
: name(name), type(type) {}


//Rule of 3: default

//Methods:

// Returns the name of the settlement
const string &Settlement:: getName() const{
    return name;
}

// Returns the type of the settlement (VILLAGE, CITY, METROPOLIS)
SettlementType Settlement:: getType() const
{
    return type;
}

// Calculates the capacity of the settlement based on its type
int Settlement:: getCapacity() const{
    int capacity = 0 ;
    switch (type) {
        case SettlementType::VILLAGE:
            capacity =1; // Villages can support 1 facility at a time
            break;
        case SettlementType::CITY:
            capacity = 2; // Cities can support 2 facilities at a time
            break;
        case SettlementType::METROPOLIS:
            capacity = 3; // Metropolises can support 3 facilities at a time
            break;
        default:
            break;
    }
    return capacity;
}

// Converts the settlement's details to a string representation
const string Settlement:: toString() const
{
    string result = "Settlement Name: " + name + ", Type: ";
    switch (type) {
        case SettlementType::VILLAGE:
            result += "VILLAGE";
            break;
        case SettlementType::CITY:
            result += "CITY";
            break;
        case SettlementType::METROPOLIS:
            result += "METROPOLIS";
            break;
    }
    return result;
}

