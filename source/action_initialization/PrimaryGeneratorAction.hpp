/**
   @date 2019/12/22
**/
#ifndef PrimaryGeneratorAction_hpp
#define PrimaryGeneratorAction_hpp

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>
#include <globals.hh>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();    
    virtual void GeneratePrimaries(G4Event* event) override;

    void EnableParticleGun()
    {
	use_particle_gun = true;
	use_gps = false;
    }
    void EnableGPS()
    {
	use_gps = true;
	use_particle_gun = false; 
    }
    
private:
    G4ParticleGun * particle_gun;
    G4GeneralParticleSource * gps;

    G4bool use_particle_gun;
    G4bool use_gps;
    
};

#endif
