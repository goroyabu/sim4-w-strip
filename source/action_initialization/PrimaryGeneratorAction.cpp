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
     particle_gun(nullptr),
     gps(nullptr),
     use_particle_gun(true),
     use_gps(false)
{
    this->particle_gun  = new G4ParticleGun();
    this->gps           = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete this->particle_gun;
    delete this->gps;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    if ( use_particle_gun )
	this->particle_gun->GeneratePrimaryVertex( event );
    else if ( use_gps )
	this->gps->GeneratePrimaryVertex( event );
}
