#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        //Constrector:
        NaiveSelection();

        // Ruls Of 3:
        // Copy-Constrector:
        NaiveSelection(const NaiveSelection &other) = default;

        //Destructor:
        ~NaiveSelection() override = default;
        
        //Methods:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
    private:
        int lastSelectedIndex ;
};

class BalancedSelection: public SelectionPolicy {
    public:
        //Constrector:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        BalancedSelection();

        // Ruls Of 3:
        // Copy-Constrector:
        BalancedSelection(const BalancedSelection &other) = default;

        //Destructor:
        ~BalancedSelection() override = default;

        //Methods:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        //Constrector:
        EconomySelection();

        // Ruls Of 3:
        // Copy-Constrector:
        EconomySelection(const EconomySelection &other) = default;

        //Destructor:
        ~EconomySelection() override = default;

        //Methods:
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;

    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        //Constrector:
        SustainabilitySelection();

        // Ruls Of 3:
        // Copy-Constrector:
        SustainabilitySelection(const SustainabilitySelection &other) = default;

        //Destructor:
        ~SustainabilitySelection() override = default;

        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
    private:
        int lastSelectedIndex;
};