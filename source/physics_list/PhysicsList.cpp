/**
   @file PhysicsList.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "PhysicsList.hpp"

#include <iostream>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

//#include <G4EmLivermorePhysics.hh>
//#include "CustomizedLivermorePhysics.hpp"
#include "PhysicsConstructor.hpp"

PhysicsList::PhysicsList()
    :G4VModularPhysicsList()
{
    // this->RegisterPhysics( new G4EmLivermorePhysics );
    // this->RegisterPhysics( new CustomizedLivermorePhysics );
    this->RegisterPhysics( new PhysicsConstructor );
    std::cout << "PhysicsList::PhysicsList()" << std::endl;
}
PhysicsList::~PhysicsList()
{
}

void PhysicsList::SetCuts()
{
    this->SetDefaultCutValue(0.1*mm);
    std::cout << "PhysicsList::SetCuts()" << std::endl;
}
