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
    primary_generator = new PrimaryGeneratorAction();
    run_action        = new RunAction();
    event_action      = new EventAction();
    stepping_action   = new SteppingAction();

}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
    std::cout << "ActionInitialization::Build()" << std::endl;


    this->SetUserAction(  primary_generator  );
    this->SetUserAction(  run_action         );
    this->SetUserAction(  event_action       );
    this->SetUserAction(  stepping_action    );
}
