#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Action.h"
#include "Auxiliary.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        //Consractor:
        Simulation(const string &configFilePath);
        //Simulation(); Do we need one? 
    //Rule Of 3:
        // Copy-Constructor:
        Simulation(const Simulation &other);


        // Copy Assignment Operator
        Simulation &operator=(const Simulation &other);
        // Destructor:
        ~Simulation();
        //Move-Constructor:
        Simulation(Simulation &&other);
        //Move Assignment Operator:
        Simulation &operator=(Simulation &&other);
        //clear:
        void clear();

        void start();  
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement *getSettlement(const string &settlementName);
        void setSelectionPolicy  (const string &selectionPolicy, int planID);
        SelectionPolicy *getSelectionPolicy (const string &selectionPolicy); // Get new SelectionPolicy according to "nve"/"bal"/"eco"/"env"
        SelectionPolicy *getSelectionPolicy (const string &selectionPolicy, int LifeQualityScore, int EconomyScore, int EnvironmentScore);// Get new SelectionPolicy according to "nav"/"bal"/"eco"/"env"
        int getPlanCounter() const;
        Plan &getPlan(const int planID);
        vector<BaseAction*> getActionsLog();
        void step();
        void close();
        void open();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};