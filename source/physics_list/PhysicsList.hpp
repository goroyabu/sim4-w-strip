/**
   @file PhysicsList.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef PhysicsList_hpp
#define PhysicsList_hpp

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class PhysicsList : public G4VModularPhysicsList
{
    
public:
    
    PhysicsList();
    ~PhysicsList();

    void SetCuts() override;
    
};

#endif
