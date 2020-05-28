/**
   @file    PhysicsConstructor.cpp
   @author  Goro Yabu  
   @date    2020/05/21
**/

#ifndef PhysicsConstructor_hpp
#define PhysicsConstructor_hpp

#include <G4VPhysicsConstructor.hh>
#include <G4EmParticleList.hh>
#include <globals.hh>

class PhysicsConstructor : public G4VPhysicsConstructor
{
    
public:

    explicit PhysicsConstructor(G4int ver=1, const G4String& name="");
    
    virtual ~PhysicsConstructor();
    
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
    
private:
    
    G4int  verbose;
    G4EmParticleList partList;

    G4String compton_model;
    G4bool   is_enabled_livermore_compton;

public:
    
    int SetParameter(const G4String& key, const G4String& value);
    int SetParameter(const G4String& key, G4bool value);
    
};

#endif





