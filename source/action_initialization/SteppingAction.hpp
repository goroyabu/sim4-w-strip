/**
   @file    SteppingAction.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef SteppingAction_hpp
#define SteppingAction_hpp

#include <G4UserSteppingAction.hh>
#include <globals.hh>

class SteppingAction : public G4UserSteppingAction
{
public:

    SteppingAction();
    virtual ~SteppingAction();

    virtual void UserSteppingAction( const G4Step * step ) override;
    
};

#endif
