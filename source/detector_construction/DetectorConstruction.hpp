/**
   @file    DetectorConstruction.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef DetectorConstruction_hpp
#define DetectorConstruction_hpp

#include <map>
#include <vector>

#include <G4VUserDetectorConstruction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    DetectorConstruction();
    ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct() override;

    int SetNumberOf(const G4String& key, G4int number);
    int SetLengthOf(const G4String& key, G4double value);
    int SetPositionOf(const G4String& key, const G4ThreeVector& vector);
    int SetNameOf(const G4String& key, const G4String& name);

    int AddDetectorLayers(const G4String& material, G4double thickness);
    
private:

    std::map<G4String, G4String>       map_key2str;
    std::map<G4String, G4double>       map_key2val;
    std::map<G4String, G4ThreeVector>  map_key2vec;
    std::map<G4String, G4int>          map_key2int;
    std::vector<G4String>              vec_detmate;
    std::vector<G4double>              vec_detthic;
    
};
#endif

