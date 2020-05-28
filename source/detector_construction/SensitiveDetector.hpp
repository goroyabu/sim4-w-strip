/**
   @file    SensitiveDetector.hpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#ifndef SensitiveDetector_hpp
#define SensitiveDetector_hpp

#include "HitsCollection.hpp"

#include <utility>

#include <G4VSensitiveDetector.hh>

#include <TString.h>

class TObject;
class TH1D;

class SensitiveDetector : public G4VSensitiveDetector
{
    
public:
    
    SensitiveDetector( G4String name );
    ~SensitiveDetector();
    
    virtual void Initialize( G4HCofThisEvent* hce ) override;
    virtual G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;
    virtual void EndOfEvent( G4HCofThisEvent* ) override;

    G4int SetDetectorID( G4int id );
    G4int GetDetectorID();
    
    std::pair<G4int, G4int> GetStripID( const G4ThreeVector& pos );
    G4int SetGridXaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridYaxis( G4int nbins, G4double min, G4double max );
    G4int SetGridZaxis( G4int nbins, G4double min, G4double max );

    std::pair<G4double, G4double> GetCenterOfPixel( G4int strip_x, G4int strip_y );
    
private:

    HitsCollection * hits_collection;
    
    G4int detector_id;
    TString grid_xaxis_name;
    TString grid_yaxis_name;
    TString grid_zaxis_name;
    TH1D * grid_xaxis;
    TH1D * grid_yaxis;
    TH1D * grid_zaxis;

    TString GetUniqueObjectName(const TString& name);
    void DeleteObject(TObject* obj);
    
};

#endif
