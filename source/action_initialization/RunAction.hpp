/**
   @file    RunAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef RunAction_hpp
#define RunAction_hpp

#include <G4UserRunAction.hh>
#include <globals.hh>

#include <vector>

class RunAction : public G4UserRunAction
{

public:

    RunAction();
    ~RunAction();

    G4Run* GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;
    
    std::vector<G4double> vec;

};

#endif
