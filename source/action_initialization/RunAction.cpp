/**
   @file    RunAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "RunAction.hpp"
#include "AnalysisManager.hpp"

#include <iostream>
#include <vector>

#include <g4root.hh>

RunAction::RunAction()
    : G4UserRunAction()
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->SetActivation(true);
    analysis_manager->SetVerboseLevel(1);
    analysis_manager->SetFileName("test_lv3");

    G4int id = analysis_manager->CreateH1("spect","Deposited energy",
					  100, 0., 100.);
    analysis_manager->SetH1Activation(id, false);
}

RunAction::~RunAction()
{
}

G4Run* RunAction::GenerateRun()
{
    std::cout << "RunAction::GenerateRun()" << std::endl;
    return G4UserRunAction::GenerateRun();
}
void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysis_manager = AnalysisManager::Instance();
    analysis_manager->OpenFile();

    analysis_manager->CreateNtuple("tree", "tree");
    analysis_manager->CreateNtupleIColumn("eventID");
    analysis_manager->CreateNtupleIColumn("nhits");
    analysis_manager->CreateNtupleIColumn("hit_pattern");
    analysis_manager->CreateNtupleDColumn("etotal");
    analysis_manager->CreateNtupleIColumnV("detid",   128);
    analysis_manager->CreateNtupleIColumnV("strip_x", 128);
    analysis_manager->CreateNtupleIColumnV("strip_y", 128);    
    analysis_manager->CreateNtupleDColumnV("edep",    128);
    analysis_manager->CreateNtupleDColumnV("pos_x",   128);
    analysis_manager->CreateNtupleDColumnV("pos_y",   128);
    analysis_manager->CreateNtupleDColumnV("pos_z",   128);
    analysis_manager->FinishNtuple();
    
    std::cout << "RunAction::BeginOfRunAction()" << std::endl;
}
void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysis_manager = AnalysisManager::Instance();    
    analysis_manager->Write();
    analysis_manager->CloseFile();
    std::cout << "RunAction::EndOfRunAction()" << std::endl;
}
