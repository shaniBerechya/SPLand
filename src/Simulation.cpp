#include "Simulation.h"
#include "Auxiliary.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

 /********************************************************************************************************* */
//Consractor
// Initializes the simulation by reading a configuration file
Simulation::Simulation(const string &configFilePath)
    : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()// Default initialization
{
    ifstream configFile(configFilePath);
    
    string line;
    while (getline(configFile, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
         // Parse the line into tokens
        vector<string> tokens = Auxiliary::parseArguments(line);
        // Process based on the type of the line
        if (tokens[0] == "settlement") {
            addSettlement(new Settlement(tokens[1], static_cast<SettlementType>(stoi(tokens[2]))));
        } 
        else if (tokens[0] == "facility") {
            addFacility(FacilityType(tokens[1], static_cast<FacilityCategory>(stoi(tokens[2])),
                stoi(tokens[3]), stoi(tokens[4]), stoi(tokens[5]), stoi(tokens[6])));
        } 
        else if (tokens[0] == "plan") {
            addPlan(*getSettlement(tokens[1]), getSelectionPolicy(tokens[2])); //"getSelectionPolicy" cerate a new SelectionPolic - it's going to be deletad by the destrector of the plan
        }
    }
    configFile.close();
}


//Rule Of 5:

// Copy-Constructor:
Simulation::Simulation(const Simulation &other):
isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions()
{
    // Deep copy of actionsLog
    for(BaseAction *action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    // Deep copy of settlements
    for(Settlement *settlement : other.settlements){
        settlements.push_back(new Settlement(*settlement)); 
    }

    //Plan (it's may seems like we don't need deep copy, but Plan has a pointer field)
    //Make sure to update the refrence of settlement in the plan
    for(Plan plan : other.plans){
        //Make sure to update the refrence of settlement in the plan
        string settlName = plan.getSettlement().getName();
        Settlement *newSettelemnet = getSettlement(settlName); 
        Plan newPlan = Plan(*newSettelemnet,plan);
        plans.push_back(newPlan);
    }

    facilitiesOptions = other.facilitiesOptions; //// No deep copy needed
    
    isRunning = other.isRunning;
    planCounter = other.planCounter;

}

// Copy Assignment Operator:
Simulation &Simulation::operator=(const Simulation &other){
    if (this != &other){
        //actionsLog:
        //delete
        for(BaseAction *action : actionsLog){
            delete action;
        }
        actionsLog.clear();

        // Deep copy of actionsLog
        for(BaseAction *action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }

        //settlements:
        //delete:
        for(Settlement *settlement : settlements){
            delete settlement;
        }
        settlements.clear();

        // Deep copy of settlements
        for(Settlement *settlement : other.settlements){
            settlements.push_back(new Settlement(*settlement));
        }

        //Plan: (it's may seems like we don't need deep copy, but Plan has a pointer field)
        plans.clear();
        for(Plan plan : other.plans){
        //Make sure to update the refrence of settlement in the plan
        string settlName = plan.getSettlement().getName();
        Settlement *newSettelemnet = getSettlement(settlName); 
        Plan newPlan = Plan(*newSettelemnet,plan);
        plans.push_back(newPlan);
        }

        facilitiesOptions = other.facilitiesOptions;
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        }
    return *this; 
}

// Destructor:
Simulation::~Simulation(){
    // Delete all the elements in the actionsLog, Plan, settlements vectors:
    for(BaseAction *action : actionsLog){
        delete action;
    }

    //plans: needs to check

    //settlements:
    for(Settlement *settlement : settlements){
        delete settlement;
    }
}

//Move-Constructor:
Simulation::Simulation(Simulation &&other)
: isRunning(other.isRunning),planCounter(other.planCounter),actionsLog(other.actionsLog),plans(other.plans), settlements(other.settlements), facilitiesOptions(other.facilitiesOptions)
{
    //Making all the pointers of other to "nullptr" because it's not under other anymore:
    for (BaseAction *&action : actionsLog) {
    action = nullptr;
    }

    //Plans
    for(Plan plan : plans){
        plan.setSelectionPolicy(nullptr);
    }

    //Settlements:
    for(Settlement *&settlement : settlements){
        settlement = nullptr;
    }
    other.plans = {};
    other.facilitiesOptions = {};
    
}

//Move Assignment Operator:
Simulation &Simulation::operator=(Simulation &&other){
    if (this != &other){
        clear();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        // Shallow copy the vector because other is rvalue
        actionsLog = actionsLog;
        plans =other.plans;
        settlements = other.settlements;
        facilitiesOptions = other.facilitiesOptions;

        //Making all the pointers of other to "nullptr" because it's not under other anymore:
        other.plans = {};
        other.facilitiesOptions = {};

        for(BaseAction *&action : actionsLog){
            action = nullptr;
        }

        //Plans
        for(Plan plan : plans){
            plan.setSelectionPolicy(nullptr);
        }

        //Settlements:
        for(Settlement *&settlement : settlements){
            settlement = nullptr;
        }
    }
    return *this;

}

// Clears all resources of the simulation
void Simulation::clear(){
    for(BaseAction *action : actionsLog){
            delete action;
        }
        actionsLog.clear();

        plans.clear();

        //settlements:
        for(Settlement *settlement : settlements){
            delete settlement;
        }
        settlements.clear();

        facilitiesOptions.clear();
}

 /********************************************************************************************************* */
//Mehtods:

//// Starts the simulation and processes user input commands
void Simulation::start(){
    open(); // Indicates that the simulation is running

    while (isRunning) {
        string line;
        getline(cin, line);
        vector<string> tokens = Auxiliary::parseArguments(line);
        if (tokens[0] == "settlement") {
            BaseAction *newSettlement = new AddSettlement(tokens[1], static_cast<SettlementType>(stoi(tokens[2])));
            newSettlement->act(*this);
            addAction(newSettlement);
        } 
        else if (tokens[0] == "facility") {

            BaseAction *newFacility = new AddFacility(tokens[1], static_cast<FacilityCategory>(stoi(tokens[2])),stoi(tokens[3]),stoi(tokens[4]),stoi(tokens[5]), stoi(tokens[6]));
            newFacility->act(*this);
            addAction(newFacility);
        } 
        else if (tokens[0] == "plan") {
            BaseAction *newPlan = new AddPlan(tokens[1], tokens[2]);
            newPlan ->act(*this);
            addAction(newPlan);
        } 
        else if (tokens[0] == "step") {
            const int numOfStep = stoi(tokens[1]);
            BaseAction *newStep = new SimulateStep(numOfStep);
            newStep ->act(*this);
            addAction(newStep);
        }
        else if (tokens[0] == "planStatus") {
            BaseAction *planStatus = new PrintPlanStatus(stoi(tokens[1]));
            planStatus ->act(*this);
            addAction(planStatus);
        } 
        else if (tokens[0] == "changePolicy") {
            BaseAction *changePolicy = new ChangePlanPolicy(stoi(tokens[1]), tokens[2]);

            changePolicy ->act(*this);
            addAction(changePolicy);
        } 
        else if (tokens[0] == "log") {
            BaseAction *printActionsLog = new PrintActionsLog();
            printActionsLog ->act(*this);
            addAction(printActionsLog);
        }
        else if (tokens[0] == "close") {
            BaseAction *close = new Close();
            close ->act(*this);
            addAction(close);
            clear();
        }  
        else if (tokens[0] == "backup") {
            BaseAction *backupSimulation = new BackupSimulation();
            backupSimulation ->act(*this);
            addAction(backupSimulation);
        }  
        else if (tokens[0] == "restore") {
            BaseAction *restoreSimulation = new RestoreSimulation();
            restoreSimulation ->act(*this);
            addAction(restoreSimulation);
        } 
       else {
        cout << "no comment like this" << endl;
       }
    }
}

// Simulates a single step for all plans
void Simulation::step(){
    for(Plan &planStep : plans){
        planStep.step();
    }
}

// Adds a new plan to the simulation
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan = Plan(planCounter,settlement,selectionPolicy, this->facilitiesOptions);
    plans.push_back(newPlan);
    planCounter += 1;
}

// Adds a new action to the actions log
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

// Adds a settlement to the simulation 
bool Simulation::addSettlement(Settlement *settlement){
        if (!isSettlementExists(settlement->getName())){
            settlements.push_back(settlement);
            return true;
        }
        return false;
}
// Adds a facility type to the list of facility options 
bool Simulation::addFacility(FacilityType facility){
    bool isFacilityExist = false;
    for (FacilityType facilityOption : facilitiesOptions){
        isFacilityExist = facilityOption.getName() == facility.getName();
        if (isFacilityExist){
            return false;
        }
    }  
    if (!isFacilityExist){
        facilitiesOptions.push_back(facility);
        return true;
    }
    return false;
}

// Checks if a settlement with the given name exists in the simulation
bool Simulation::isSettlementExists(const string &settlementName){
    for(Settlement *settlement : settlements){
        string otherName = settlement->getName();
        if (otherName == settlementName){
            return true;
        }
    }
    return false;
}

// Retrieves a pointer to a settlement by its name
Settlement *Simulation::getSettlement(const string &settlementName){
    Settlement *sp = nullptr;
    for(Settlement *settlement : settlements){
        if (settlement->getName() == settlementName){
            return settlement;
        }
    }
    return sp;
}

// Retrieves the selection policy object based on its string representation and scores
SelectionPolicy *Simulation::getSelectionPolicy (const string &selectionPolicy, int LifeQualityScore, int EconomyScore, int EnvironmentScore){
    if(selectionPolicy == "nve"){
        return new NaiveSelection();
    }
    else if(selectionPolicy == "eco"){
        return new EconomySelection();
    }
    else if(selectionPolicy == "env"){
        return new SustainabilitySelection();
    }
    else if(selectionPolicy == "bal"){
        return new BalancedSelection(LifeQualityScore,EconomyScore,EnvironmentScore);
    }
    else {
        throw std::invalid_argument("Invalid selection policy: " + selectionPolicy);    
    }
}

// Overloaded version: retrieves the selection policy with default scores
SelectionPolicy *Simulation::getSelectionPolicy (const string &selectionPolicy){
    return getSelectionPolicy(selectionPolicy,0,0,0);
}

// Returns the current plan counter value
int Simulation::getPlanCounter() const{
    return planCounter;
}

// Retrieves a reference to the plan by its ID
Plan &Simulation::getPlan(const int planID){
    return plans[planID];
}

// Closes the simulation, printing all plans' details and setting the running flag to false
void Simulation::close(){
    for(Plan plan : plans){
        cout << plan.toString()  +"\n"<< endl;
    }
    isRunning = false;
}

// Opens the simulation, setting the running flag to true
void Simulation::open(){
    isRunning = true;
    cout << "Simulation started!" << endl;
}

// Updates the selection policy for a specified plan 
void Simulation::setSelectionPolicy(const string &selectionPolicy, int planID) {
    Plan &plan = getPlan(planID); 
    SelectionPolicy *policy = nullptr;

    if (selectionPolicy == "nve") {
        policy = new NaiveSelection();
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelection();
    } else if (selectionPolicy == "env") {
        policy = new SustainabilitySelection();
    }
     plan.setSelectionPolicy(policy); 
    
}

// Returns the log of all actions performed in the simulation
vector<BaseAction*> Simulation::getActionsLog(){
    return actionsLog;
}
