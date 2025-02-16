#include "Facility.h"
#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;
using namespace std;
//***********************************************************************************************************

//************************************************  FacilityType ************************************************

//Constrectors:

//Main constractor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: name(name), category(category), price(price), lifeQuality_score(lifeQuality_score),
 economy_score(economy_score), environment_score(environment_score){}

// Default constructor: initializes a FacilityType with default values
FacilityType::FacilityType()
: FacilityType("", FacilityCategory::LIFE_QUALITY, 0, 0, 0, 0){}

//Rule Of 3:

// Copy constructor
FacilityType::FacilityType(const FacilityType &other)
:FacilityType(other.name, other.category, other.price, other.lifeQuality_score,other.economy_score,other.environment_score){}

// Copy assignment operator
FacilityType &FacilityType::operator=(const FacilityType &other){
    return *this;
}

// Destructor: does nothing since FacilityType has no dynamic memory
FacilityType::~FacilityType(){

}

// Equality operator: checks if two FacilityType objects have the same properties
bool FacilityType::operator==(const FacilityType &other) const {
    return (name == other.name) && (category == other.category) && 
           (price == other.price) &&
           (lifeQuality_score == other.lifeQuality_score) &&
           (economy_score == other.economy_score);
}

// Methods:

// Returns the name of the facility
const string &FacilityType::getName() const{
    return name;
}

// Returns the cost (price) of the facility
int FacilityType::getCost() const{
    return price;
}

// Returns the life quality score contribution of the facility
int FacilityType::getLifeQualityScore() const{
    return lifeQuality_score;
}

// Returns the environmental score contribution of the facility
int FacilityType::getEnvironmentScore() const{
    return environment_score;
}

// Returns the economic score contribution of the facility
int FacilityType::getEconomyScore() const{
    return economy_score;
}

// Returns the category of the facility
FacilityCategory FacilityType::getCategory() const{
    return category;
}

//***********************************************************************************************************

//************************************************  Facility ************************************************

// Constructors:


// Constructor 
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
    settlementName(settlementName),
    status(FacilityStatus::UNDER_CONSTRUCTIONS),
    timeLeft(price){}

// Creates a Facility from an existing FacilityType, associating it with a settlement
Facility::Facility(const FacilityType &type, const string &settlementName)
    :Facility(type.FacilityType::getName(), settlementName, type.FacilityType::getCategory(), 
    type.FacilityType::getCost(), type.FacilityType::getLifeQualityScore(), type.FacilityType::getEconomyScore(),
     type.FacilityType::getEnvironmentScore()){}

// Default constructor: creates a Facility with default values
Facility::Facility()
: Facility(FacilityType(), ""){}

//Rule Of 3:

// Copy constructor
Facility::Facility(const Facility &other)
:Facility(other.name, other.settlementName,other.category, other.price, other.lifeQuality_score,other.economy_score,other.environment_score){}

// Copy assignment operator
Facility &Facility::operator=(const Facility &other){
    return *this;
}

// Destructor: does nothing since Facility has no dynamic memory
Facility::~Facility(){}

//Method:

// Returns the name of the settlement associated with the facility
const string &Facility::getSettlementName() const{
    return settlementName;
}

// Returns the time left for the facility to complete construction
const int Facility::getTimeLeft() const{
    return timeLeft;
}

// Simulates one step of construction, reducing timeLeft by one
FacilityStatus Facility::step(){
    if(timeLeft>0){
        timeLeft = timeLeft-1;
        if(timeLeft==0) {
        //cout << "hiiii" << endl;
        setStatus(FacilityStatus::OPERATIONAL);
        }
    }
    return status;
}

// Updates the status of the facility
void Facility::setStatus(FacilityStatus status){
    this->status=status;
}

// Returns the current status of the facility
const FacilityStatus& Facility::getStatus() const{
    return status;
}

// Converts the facility's details to a string representation
const string Facility::toString() const{
    return "facilityName: " + getName() + " , facilityStatus: " + statusToString();
}

// Converts the FacilityStatus enum to a string representation
string Facility::statusToString() const{
    string result;
    switch (status) {
        case FacilityStatus::UNDER_CONSTRUCTIONS :
            result = "UNDER_CONSTRUCTIONS";
            break;
        case FacilityStatus::OPERATIONAL:
            result = "OPERATIONAL";
            break;
    }
    return result;
}