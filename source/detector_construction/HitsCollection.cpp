/**
   @author  Goro Yabu
   @date    2020/01/31
**/

#include "HitsCollection.hpp"

#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <iostream>
#include <iomanip>

G4ThreadLocal G4Allocator<Hit>* HitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Hit::Hit()
    : G4VHit(),
      fEdep(0.),
      fTrackLength(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Hit::~Hit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Hit::Hit(const Hit& right)
    : G4VHit()
{
    fEdep        = right.fEdep;
    fTrackLength = right.fTrackLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const Hit& Hit::operator=(const Hit& right)
{
    fEdep        = right.fEdep;
    fTrackLength = right.fTrackLength;
    
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool Hit::operator==(const Hit& right) const
{
    return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Hit::Print()
{
    std::cout
	<< "Edep: " 
	<< std::setw(7) << G4BestUnit(fEdep,"Energy")
	<< " track length: " 
	<< std::setw(7) << G4BestUnit( fTrackLength,"Length")
	<< std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
