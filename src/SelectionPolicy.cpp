#include "SelectionPolicy.h"
#include <climits> 
#include <string>
#include <stdexcept>
using std::string;
using std::vector;
using std::max;
using std::min;


//***********************************************************************************************************

//************************************************  NaiveSelection ************************************************

//Constrector:
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}


// Ruls Of 3: Defult 

//Methods:

// Selects the next facility in a circular manner
const FacilityType& NaiveSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

// Converts the selection policy to a string representation
const string NaiveSelection::toString() const{
    return "nve";
}

// Creates a deep copy of the NaiveSelection object
NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}



//***********************************************************************************************************

//************************************************  BalancedSelection ************************************************

//Constractor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) :
 LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

// Default constructor:
BalancedSelection::BalancedSelection() :
BalancedSelection(0,0,0){}

// Ruls Of 3: Defult

//Methods:

// Selects a facility that minimizes the difference between maximum and minimum scores
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    int minDifference = INT_MAX; 
    int selectedIndex = 0;

    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        const FacilityType& facility = facilitiesOptions[i];

        int newLifeQuality = LifeQualityScore + facility.getLifeQualityScore();
        int newEconomy = EconomyScore + facility.getEconomyScore();
        int newEnvironment = EnvironmentScore + facility.getEnvironmentScore();

        int maxScore = std::max(newLifeQuality, std::max(newEconomy, newEnvironment));
        int minScore = std::min(newLifeQuality, std::min(newEconomy, newEnvironment));
        int difference = maxScore - minScore;

        if (difference < minDifference) {
            minDifference = difference;
            selectedIndex = i;
        }
    }
    //Update scores:
    LifeQualityScore += facilitiesOptions[selectedIndex].getLifeQualityScore();
    EconomyScore += facilitiesOptions[selectedIndex].getEconomyScore();
    EnvironmentScore += facilitiesOptions[selectedIndex].getEnvironmentScore();

    return facilitiesOptions[selectedIndex];
}


// Converts the selection policy to a string representation
const string BalancedSelection::toString() const {
    return "bal";
}

// Creates a deep copy of the BalancedSelection object
BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}



//***********************************************************************************************************
//************************************************  EconomySelection ************************************************

//Constractor
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

// Ruls Of 3: Defult

//Methods:
// Selects the next facility in the ECONOMY category, iterating in a circular manner
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    for (size_t i = 1; i <= facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + i) % facilitiesOptions.size(); 
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = index; 
            return facilitiesOptions[index]; 
        }
    }
    throw std::runtime_error("No suitable facility found in EconomySelection");
}

// Converts the selection policy to a string representation
 const string EconomySelection::toString() const {
    return "eco";
}

// Creates a deep copy of the EconomySelection object
EconomySelection* EconomySelection :: clone() const {
    return new EconomySelection(*this);
}



//***********************************************************************************************************
//************************************************  SustainabilitySelectio ************************************************

    
//Constractor
 SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

// Ruls Of 3: Defult 

//Methods:


// Selects the next facility in the ECONOMY category, iterating in a circular manner
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    for (size_t i = 1; i <= facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + i) % facilitiesOptions.size(); 
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = index; 
            return facilitiesOptions[index]; 
        }
    }
    throw std::runtime_error("No suitable facility found in SustainabilitySelection");
}

// Converts the selection policy to a string representation
const string SustainabilitySelection::toString() const {
    return "env";
}


// Creates a deep copy of the SustainabilitySelection object
SustainabilitySelection* SustainabilitySelection :: clone() const {
    return new SustainabilitySelection(*this);
}













