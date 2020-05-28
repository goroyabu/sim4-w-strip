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

G4ThreadLocal G4Allocator<DsdHit>* DsdHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DsdHit::DsdHit()
    : G4VHit()
      //      fEdep(0.),
      //fTrackLength(0.)
{
    detid = -1;
    strip_x = -1;
    strip_y = -1;
    pos = G4ThreeVector(0,0,0);
    edep = 0.0;
    total_energy = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DsdHit::~DsdHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DsdHit::DsdHit(const DsdHit& right)
    : G4VHit()
{
    // fEdep        = right.fEdep;
    // fTrackLength = right.fTrackLength;
    
    detid = right.detid;
    strip_x = right.strip_x;
    strip_y = right.strip_y;
    pos = right.pos;
    edep = right.edep;
    total_energy = right.total_energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const DsdHit& DsdHit::operator=(const DsdHit& right)
{
    // fEdep        = right.fEdep;
    // fTrackLength = right.fTrackLength;

    detid = right.detid;
    strip_x = right.strip_x;
    strip_y = right.strip_y;
    pos = right.pos;
    edep = right.edep;    
    total_energy = right.total_energy;
    
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DsdHit::operator==(const DsdHit& right) const
{
    return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DsdHit::Print()
{
    // std::cout
    // 	<< "DETID: " << std::setw(3) << detid;
    // 	<< "Edep: " 
    // 	<< std::setw(7) << G4BestUnit(fEdep,"Energy")
    // 	<< " track length: " 
    // 	<< std::setw(7) << G4BestUnit( fTrackLength,"Length")
    // 	<< std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DsdHit* DsdHit::SetDetectorID( G4int id )
{
    this->detid = id;
    return this;
}
DsdHit* DsdHit::SetStripID( G4int x, G4int y )
{
    this->strip_x = x; this->strip_y = y;
    return this;
}
DsdHit* DsdHit::SetPosition( const G4ThreeVector& p )
{
    this->pos = p;
    return this;
}
DsdHit* DsdHit::SetEnergy( G4double energy )
{
    this->edep = energy;
    return this;
}
DsdHit* DsdHit::SetProcessName( const G4String& name )
{
    this->proc_name = name;
    return this;
}

bool DsdHit::IsSamePixel( const DsdHit& other )
{
    return detid   == other.detid
	&& strip_x == other.strip_x
	&& strip_y == other.strip_y;
}
bool DsdHit::IsAdjacentPixel( const DsdHit& other )
{
    if( detid != other.detid ) return false;
    return IsSamePixel( other ) == true 
	|| abs( strip_x - other.strip_x ) <= 1
	|| abs( strip_y - other.strip_y ) <= 1;
}
DsdHit* DsdHit::MergeSamePixel( const DsdHit& other )
{
    if ( !IsSamePixel(other) ) return this;

    G4ThreeVector newpos(0,0,0);
    if ( this->edep > other.edep ) newpos = this->pos;
    else newpos = other.pos;

    SetPosition( newpos );
    SetEnergy( this->edep + other.edep );
    
    return this;
}
DsdHit* DsdHit::MergeAdjacentPixel( const DsdHit& other , G4int mode)
{
    if ( !IsAdjacentPixel( other ) ) return this;

    G4ThreeVector newpos(0,0,0);

    if ( mode==0 ) {
	if ( this->edep > other.edep ) newpos = this->pos;
	else newpos = other.pos;
    }
    // else {
    // 	newpos = this->pos*this->edep + other->
    // }

    SetPosition( newpos );
    SetEnergy( this->edep + other.edep );
    
    return this;    
}
