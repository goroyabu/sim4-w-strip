/**
   @date 2019/12/22
**/
#ifndef PrimaryGeneratorAction_hpp
#define PrimaryGeneratorAction_hpp

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <globals.hh>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event* event) override;
    
private:
    G4ParticleGun * particle_gun;
    
};

#endif
