/**
   @file PhysicsList.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "PhysicsList.hpp"

#include <iostream>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4EmLivermorePhysics.hh>

PhysicsList::PhysicsList()
    :G4VModularPhysicsList()
{
    this->RegisterPhysics( new G4EmLivermorePhysics );
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
