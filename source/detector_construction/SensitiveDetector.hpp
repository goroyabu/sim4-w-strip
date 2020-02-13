/**
   @file    SensitiveDetector.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef SensitiveDetector_hpp
#define SensitiveDetector_hpp

#include "HitsCollection.hpp"

#include <G4VSensitiveDetector.hh>

class SensitiveDetector : public G4VSensitiveDetector
{
    
public:
    
    SensitiveDetector( G4String name );
    ~SensitiveDetector();
    
    virtual void Initialize( G4HCofThisEvent* hce ) override;
    virtual G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;
    virtual void EndOfEvent( G4HCofThisEvent* ) override;
    
private:

    HitsCollection * hits_collection;
    
};

#endif
