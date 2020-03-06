/**
   @file    EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "EventAction.hpp"
#include "AnalysisManager.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <G4Event.hh>

using std::cout;
using std::endl;

EventAction::EventAction()
    : G4UserEventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->ClearNtupleVector();

    G4int eventID = event->GetEventID();
    analysis_manager->FillNtupleIColumnName( "eventID", eventID );

    auto ima = std::chrono::system_clock::now();
    auto ima_time = std::chrono::system_clock::to_time_t(ima);
    
    if ( eventID%10000==0 )
	cout << ">> Event " << std::setw(10) << eventID << " at " << std::ctime(&ima_time);
	    
    // std::cout << "EventAction::BeginOfEventAction()" << std::endl;
}
void EventAction::EndOfEventAction(const G4Event*)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->AddNtupleRow();    
    // std::cout << "EventAction::EndOfEventAction()" << std::endl;
}
