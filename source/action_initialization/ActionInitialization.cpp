/**
   @file    ActionInitialization.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "ActionInitialization.hpp"
#include "PrimaryGeneratorAction.hpp"
#include "RunAction.hpp"
#include "EventAction.hpp"
#include "SteppingAction.hpp"

#include <iostream>

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
    std::cout << "ActionInitialization::Build()" << std::endl;

    //auto primary_generator = new PrimaryGeneratorAction();
    //SetUserAction(primary_generator);

    this->SetUserAction(  new PrimaryGeneratorAction  );
    this->SetUserAction(  new RunAction               );
    this->SetUserAction(  new EventAction             );
    this->SetUserAction(  new SteppingAction          );
}
