/**
   @file    ActionInitialization.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef ActionInitialization_hpp
#define ActionInitialization_hpp

#include <G4VUserActionInitialization.hh>
#include <globals.hh>

class ActionInitialization : public G4VUserActionInitialization
{
public:
    
    ActionInitialization();
    virtual ~ActionInitialization();
    
    virtual void Build() const override;
    
};

#endif
