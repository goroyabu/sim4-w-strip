/**
   @file    EventAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef EventAction_hpp
#define EventAction_hpp

#include <G4UserEventAction.hh>
#include <globals.hh>

class EventAction : public G4UserEventAction
{
    
public:

    EventAction();
    ~EventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    void SetParameter(const G4String& key, int value);


private:

    int save_nhit_min;
    
};

#endif
