//Group : Omar Buzdar & Nicholas Wenstad
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <math.h>
#include "demogData.h"
#include "comboDemogData.h"
#include "comboHospitalData.h"
#include "countyDemogData.h"
#include "comboHospitalData.h"
#include "hospitalData.h"
#include "cityHospitalData.h"
#include "parse.h"

#include "visitorReport.h"
#include "visitorCombineState.h"
#include "visitorCombineCounty.h"
#include "stats.h"
#include "statTool.h"
#include "statGatherer.h"

using namespace std;


int main() {

    std::vector<shared_ptr<placeData>> pileOfData;

    //set up outpout format
    cout << std::setprecision(2) << std::fixed;

    //read in the hospital data
    read_csv(pileOfData, "hospitals.csv", HOSPITAL);
    //read in the demographic data
    read_csv(pileOfData, "county_demographics.csv", DEMOG); 

    //create a visitor to combine the state data
    visitorCombineState theStates;

    //create the state demographic data
    statTool::createStateData(pileOfData, theStates);
    //theStates.printAllStates();
   

    //create a visitor to combine the county data (need aux map city-> county)
    visitorCombineCounty theCounties("simple_uscities.csv");
    statTool::createCountyData(pileOfData, theCounties);
    //theCounties.printAllCounties();
    //cout<< "Size of county hosp data:" << theCounties.countyHmap().size() << endl;
    //create statGathers to help make the data one for states one for counties
    stateGather fillStates;
    countyGather fillCounties;

    //Do stats work here these are examples...

/*
    //mixed data use 'sample'
    cout << "**State data over 65  and number Hospitals: " << endl;
    statTool::computeStatsMixRegionData(&theStates, &fillStates, &demogData::getpopOver65, &comboHospitalData::getNumH);
    cout << "County data over 65 and number Hospitals: " << endl;
    statTool::computeStatsMixRegionData(&theCounties, &fillCounties, &demogData::getpopOver65, &comboHospitalData::getNumH);
    
    cout<< "Size of county hosp data:" << theCounties.countyHmap().size() << endl;

    //demogxdemog sample
    cout << "State data Pop under 5 and BA up: " << endl;
    statTool::computeStatsDemogRegionData(&theStates, &fillStates, &demogData::getpopUnder5, &demogData::getBAup,
        &demogData::getpopUnder5Count, &demogData::getBAupCount);
    cout << "County data Pop under 5 and BA up: " << endl;
    statTool::computeStatsDemogRegionData(&theCounties, &fillCounties, &demogData::getpopUnder5, &demogData::getBAup,
        &demogData::getpopUnder5Count, &demogData::getBAupCount);

    cout<< "Size of county hosp data:" << theCounties.countyHmap().size() << endl;
*/

    cout << "\t\t WORKSHEET QUESTIONS\n";

    vector<double> stateOveralls;
    vector<double> countyOveralls;
    vector<double> stateNumHosp;
    vector<double> countyNumHosp;
    vector<double> countyHSup;
    vector<double> countyPovBelow;
    vector<double> statePopOver65;
    vector<double> countyBAup;


    for(auto obj : theStates.stateHmap()){
        if(!isnan((obj.second)->getOverallRate())){
            stateOveralls.push_back((obj.second)->getOverallRate());
        }
        if(!isnan((obj.second)->getNumH())){
            stateNumHosp.push_back((obj.second)->getNumH());
        }
    }
    for(auto obj : theCounties.countyHmap()){
        if(!isnan((obj.second)->getOverallRate())){
            countyOveralls.push_back((obj.second)->getOverallRate());
        }
        if(!isnan((obj.second)->getNumH())){
            countyNumHosp.push_back((obj.second)->getNumH());
        }
    }
    for(auto obj : theCounties.countyDmap()){
        if(!isnan((obj.second)->getHSup())){
            countyHSup.push_back((obj.second)->getHSup());
        }
        if(!isnan((obj.second)->getBelowPoverty())){
            countyPovBelow.push_back((obj.second)->getBelowPoverty());
        }
        if(!isnan((obj.second)->getBAup())){
            countyBAup.push_back((obj.second)->getBAup());
        }
    }
    for(auto obj : theStates.stateDmap()){
        if(!isnan((obj.second)->getpopOver65())){
            statePopOver65.push_back((obj.second)->getpopOver65());
        }
    }
    

    double stateMean = stats::computePopMean(stateOveralls, stateOveralls.size());
    double stateStdDev = stats::computeStdDevSample(stateOveralls);

    double countyMean = stats::computePopMean(countyOveralls, countyOveralls.size());
    double countyStdDev = stats::computeStdDevSample(countyOveralls);

    double numHospMean = stats::computePopMean(stateNumHosp, stateNumHosp.size());
    double numHospStdDev = stats::computeStdDevSample(stateNumHosp);

    double ovrHSup_mean = stats::computePopMean(countyHSup, countyHSup.size());
    double ovrHSup_SD = stats::computeStdDevSample(countyHSup);

    double ovrPovBelow_mean = stats::computePopMean(countyPovBelow, countyPovBelow.size());
    double ovrPovBelow_SD = stats::computeStdDevSample(countyPovBelow);

    double correlation1 = stats::computeCorCoeff(stateOveralls, statePopOver65);
    double correlation2 = stats::computeCorCoeff(countyNumHosp, countyBAup);

    cout << "\t Overall Hospital Rating (state): "<< endl;
    cout << "State Mean: " << stateMean << endl;
    cout << "State SD: " << stateStdDev << endl << endl;

    cout << "\t Overall Hospital Rating (county): "<< endl;
    cout << "County Mean: " << countyMean << endl;
    cout << "County SD: " << countyStdDev << endl << endl;

    cout << "\t Number of Hospitals: "<< endl;
    cout << "Number of Hospitals Mean: " << numHospMean << endl;
    cout << "Number of Hospitals SD: " << numHospStdDev << endl << endl;

    cout << "\t Overall HS up: " << endl;
    cout << "HS up Mean: " << ovrHSup_mean << endl;
    cout << "HS up SD: " << ovrHSup_SD << endl << endl;
    
    cout << "\t Overall Poverty Below: " << endl;
    cout << "Pov Below Mean: " << ovrPovBelow_mean << endl;
    cout << "Pov Below SD: " << ovrPovBelow_SD << endl << endl;


    cout << "\nCorrelation1: " << endl;
    statTool::computeStatsMixRegionData(&theStates, &fillStates, &demogData::getpopOver65, &comboHospitalData::getOverallRate);

    cout << "\nCorrelation2: " << endl;
    statTool::computeStatsMixRegionData(&theCounties, &fillCounties, &demogData::getBAup, &comboHospitalData::getNumH);
    
    cout << "\nCorrelation3: " << endl;
    statTool::computeStatsMixRegionData(&theCounties, &fillCounties, &demogData::getBelowPoverty, &comboHospitalData::getOverallRate);

    cout << "\nCorrelation4: " << endl;
    statTool::computeStatsDemogRegionData(&theCounties, &fillCounties, &demogData::getBAup, &demogData::getBelowPoverty, &demogData::getBAupCount, &demogData::getBelowPovertyCount);

    cout << "\nCorrelation5: " << endl;
    statTool::computeStatsDemogRegionData(&theCounties, &fillCounties, &demogData::getHSup, &demogData::getBelowPoverty, &demogData::getHSupCount, &demogData::getBelowPovertyCount);

    cout << "\nCorrelation6: " << endl;
    statTool::computeStatsDemogRegionData(&theCounties, &fillCounties, &demogData::getpopUnder18, &demogData::getpopUnder5, &demogData::getpopUnder18Count, &demogData::getpopUnder5Count);

    cout << "\nCorrelation7: " << endl;
    statTool::computeStatsDemogRegionData(&theCounties, &fillCounties, &demogData::getpopUnder18, &demogData::getpopOver65, &demogData::getpopUnder18Count, &demogData::getpopOver65Count);

    for(auto obj : statePopOver65){
        cout << obj << ", ";
    }
    
    cout << "\nOveralls: " << endl;
    for(auto obj : stateOveralls){
        cout << obj << ", ";
    }
    return 0;
}



