# include "Plan.h"
#include <iostream>
using std::vector;
using std::string;
using namespace std;


// Constructors:

// Main constructor:
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) 
: 
plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),
capacity(settlement.getCapacity()), facilities(),  underConstruction(), facilityOptions(facilityOptions),
life_quality_score(0), economy_score(0), environment_score(0) {} 

//Semi Copy-constrector: creates a new plan for a different settlement but based on an existing plan and settlement 
Plan::Plan(const Settlement &settlement,const Plan &other)
//Initialize the const variables before the default constructor in C++
:Plan(other.plan_id, settlement, other.selectionPolicy->clone(),other.facilityOptions)
{
    status = other.status;
    capacity = other.capacity;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();

    //Deep Copy of the Facility lists:
    for(Facility* facility : other.underConstruction){
       underConstruction.push_back(new Facility(*facility));
    }

    for(Facility* facility : other.facilities){
       facilities.push_back(new Facility(*facility));
    }
}


//Rule Of 3:

// Copy-Constructor
Plan::Plan(const Plan &other)
//Initialize the const variables before the default constructor in C++
:Plan(other.plan_id, other.settlement, other.selectionPolicy->clone(),other.facilityOptions)
{
    status = other.status;
    capacity = other.capacity;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();


    //Deep Copy of the Facility lists:
    for(Facility* facility : other.underConstruction){
       underConstruction.push_back(new Facility(*facility));
    }

    for(Facility* facility : other.facilities){
       facilities.push_back(new Facility(*facility));
    }
}

// Copy Assignment Operator: not implemented because `settlement` is const
Plan& Plan::operator=(const Plan &other){
    return *this; // This operator is trivial since assignment isn't needed
}


//Destructor
Plan::~Plan(){
    delete selectionPolicy;
    for (Facility* facility : facilities) {
        delete facility;
    }
    for (Facility* facility : underConstruction) {
        delete facility;
    }

}

//Move-Constructor:
Plan::Plan(Plan &&other)
//Initialize the const variables before the default constructor in C++
:Plan(other.plan_id, other.settlement,other.selectionPolicy,other.facilityOptions)
{
    status = other.status;
    capacity = other.capacity;
    life_quality_score = other.getlifeQualityScore();
    economy_score = other.getEconomyScore();
    environment_score = other.getEnvironmentScore();

    //Shallow Copy of the Facility lists and Reset the pointers of other to null:
    facilities = other.facilities;
    other.facilities = {};

    underConstruction = other.underConstruction;
    underConstruction = {};

    other.selectionPolicy = nullptr;

}

// Move Assignment Operator: not implemented because `settlement` is const
Plan& Plan::operator=(Plan &&other){
    return *this; // Trivial since assignment isn't needed
}

//Methods:

//Geters:
const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}

// Sets a new selection policy for the plan
void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    if (selectionPolicy != nullptr) { 
        delete selectionPolicy; 
    }  
        selectionPolicy = newPolicy; 

}
// Performs a simulation step for the plan
void Plan::step() {
    // Select facilities according to capacity and selection policy
    while(status == PlanStatus::AVALIABLE){
        FacilityType next = selectionPolicy->selectFacility(facilityOptions);
        Facility* nextFacility = new Facility(next, settlement.getName());// nextFacility is in the heap because we need it after the scope of "step()"
        addFacility(nextFacility);
        capacity = capacity -1;
        if (capacity == 0)
        {
            status = PlanStatus::BUSY;
        }    
    }
    // Build facilities under construction
    if (status == PlanStatus::BUSY)
    {
        for (size_t i = 0; i < underConstruction.size(); ) {
            Facility* facilityToBuild = underConstruction[i];
            FacilityStatus newStatus = facilityToBuild->step();
            if (newStatus == FacilityStatus::OPERATIONAL) {
                addFacility(facilityToBuild);

                // Update the scores:
                life_quality_score += facilityToBuild->getLifeQualityScore();
                economy_score += facilityToBuild->getEconomyScore();
                environment_score += facilityToBuild->getEnvironmentScore();
                capacity += 1; // Now we can build one more facility

                // Erase the element by index
                underConstruction.erase(underConstruction.begin() + i);
            } else {
                i++; // Increment only if no element is erased
            }
        }
    }

        //Update status:
        if (capacity != 0)
        {
            status = PlanStatus::AVALIABLE;
        } 
    }

// Adds a facility to the appropriate list based on its status
void Plan::addFacility(Facility* facility){
    if(facility->getStatus() == FacilityStatus::OPERATIONAL){
            facilities.push_back(facility);
    }
    else{
        underConstruction.push_back(facility);
    }  
}

// Prints the current status of the plan
void Plan::printStatus(){
    string statusString;
    switch (status) {
        case PlanStatus::AVALIABLE:
            statusString =  "AVALIABLE";
            break;
        case PlanStatus::BUSY:
            statusString = "BUSY";
            break;
    }
    cout << statusString << endl;
}

// Converts the plan's details to a string representation
const string Plan::toString() const {
    string output;
    output = "planID: " + std::to_string(plan_id) + "\n" +
             "SettlementName: " + settlement.getName() + "\n" +
             "planStatus: " + statusToString() + "\n" +
             "SelectionPolicy: " + selectionPolicy->toString() + "\n" +
             "LifeQualityScore: " +  std::to_string(life_quality_score) +  "\n" +
             "EconomyScore: " +  std::to_string(economy_score) + "\n" +
             "EnvironmentScore: " +  std::to_string(environment_score) + "\n" +
             facilitiesToString(); 
    return output;
}

// Converts the plan status to a string
string Plan::statusToString() const{
    string statusString;
    switch (status) {
        case PlanStatus::AVALIABLE:
            statusString =  "AVALIABLE";
            break;
        case PlanStatus::BUSY:
            statusString = "BUSY";
            break;
    }
    return statusString;
}

// Returns the settlement associated with the plan
Settlement Plan::getSettlement() const {
    return settlement;
}

// Returns the plan ID
int Plan::getPlanId() const{
    return plan_id;
}

// Converts the facilities list to a string representation
string Plan::facilitiesToString() const {
    //Operational facilities
    string toString = "Operational facilities: ";
    for (Facility* facility : facilities) {
        toString += facility->getName() + ", ";
    }
    //Under construction facilities
    toString += "\nUnder construction facilities: ";
    for (Facility* facility : underConstruction) {
        toString += facility->getName() + ", ";
    }
    return toString;
}

// Returns the selection policy as a string
string Plan::getSelectionPolicy () const{
    return selectionPolicy->toString();
}








