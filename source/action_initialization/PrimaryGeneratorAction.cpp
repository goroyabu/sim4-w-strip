/**
   @date 2019/12/22
**/

#include "PrimaryGeneratorAction.hpp"

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    :G4VUserPrimaryGeneratorAction(),
     particle_gun(nullptr)
{
    this->particle_gun  = new G4ParticleGun();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete this->particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    std::cout << "PrimaryGeneratorAction::GeneratePrimaries()" << std::endl;

    /** 1. Macro **/
    // /gun/particle gamma
    // /gun/energy 100 keV
    // /gun/direction 1. 0. 0.
    // /gun/position -5. 0. 0.0. cm

    /** 2. Hard Coding **/
    // auto particle_definition
    // 	= G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    // this->particle_gun->SetParticleDefinition(particle_definition);
    // particle_gun->SetParticleEnergy            ( 100*keV );
    // particle_gun->SetParticleMomentumDirection ( G4ThreeVector( 1.0, 0.0, 0.0) );
    // particle_gun->SetParticlePosition          ( G4ThreeVector( -5.0*cm,  0.0*cm,  0.0*cm) );
    
    this->particle_gun->GeneratePrimaryVertex( event );
}
