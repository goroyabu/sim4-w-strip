/**
   @file    EventAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "EventAction.hpp"
#include "AnalysisManager.hpp"

#include <iostream>

EventAction::EventAction()
    : G4UserEventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event*)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->ClearNtupleVector();    
    std::cout << "EventAction::BeginOfEventAction()" << std::endl;
}
void EventAction::EndOfEventAction(const G4Event*)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->AddNtupleRow();    
    std::cout << "EventAction::EndOfEventAction()" << std::endl;
}
