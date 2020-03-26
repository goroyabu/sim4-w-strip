/**
   @file    ActionInitialization.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef ActionInitialization_hpp
#define ActionInitialization_hpp

#include <G4VUserActionInitialization.hh>
#include <globals.hh>

class PrimaryGeneratorAction;
class RunAction;
class EventAction;
class SteppingAction;

class ActionInitialization : public G4VUserActionInitialization
{
public:
    
    ActionInitialization();
    virtual ~ActionInitialization();
    
    virtual void Build() const override;

    PrimaryGeneratorAction* GetPrimaryGeneratorAction()
    {
	return primary_generator;
    }
    RunAction* GetRunAction()
    {
	return run_action;
    }
    EventAction* GetEventAction()
    {
	return event_action;
    }
    SteppingAction* GetSteppingAction()
    {
	return stepping_action;
    }
    
private:
    
    PrimaryGeneratorAction*  primary_generator;
    RunAction*               run_action;
    EventAction*             event_action;
    SteppingAction*          stepping_action;
    
};

#endif
