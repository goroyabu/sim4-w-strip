/**
   @file    SteppingAction.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "SteppingAction.hpp"

#include <iostream>

SteppingAction::SteppingAction()
    : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction( const G4Step * )
{
    std::cout << "SteppingAction::UserSteppingAction()" << std::endl;
}
