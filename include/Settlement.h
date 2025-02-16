#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
    //Constructor
        Settlement(const string &name, SettlementType type);
        Settlement() = default;

    //Rule Of 3:
        // Copy-Constructor
        Settlement(const Settlement& other) = default;

        // Copy Assignment Operator
        Settlement& operator=(const Settlement& other) = default;

        // Destructor
        ~Settlement() = default;
    
    //metohds:
        //geters: 
        const string &getName() const;
        SettlementType getType() const;
        int getCapacity() const;


        const string toString() const;

        private:
            const string name;
            SettlementType type;
};