/**
   @file HitsCollection.hpp
   @author  Goro Yabu
   @date    2020/01/31
**/

#ifndef HitsCollection_hpp
#define HitsCollection_hpp

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <G4Threading.hh>

class DsdHit : public G4VHit
{
    
public:
    
    DsdHit();
    DsdHit(const DsdHit&);
    virtual ~DsdHit();
    
    // operators
    const DsdHit& operator=(const DsdHit&);
    G4bool operator==(const DsdHit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    // methods from base class
    virtual void Draw() override {}
    virtual void Print() override;
    
    // methods to handle data
    // void Add(G4double de, G4double dl);
    
    // get methods
    // G4double GetEdep() const;
    // G4double GetTrackLength() const;
    
private:
    
    // G4double fEdep;        ///< Energy deposit in the sensitive volume
    // G4double fTrackLength; ///< Track length in the  sensitive volume

public:
    G4int detid;
    G4int strip_x, strip_y;
    //G4double pos_x, pos_y, pos_z;
    G4ThreeVector pos;
    G4double edep;
    G4double total_energy;
    G4String proc_name;

    DsdHit* SetDetectorID( G4int id );
    DsdHit* SetStripID( G4int x, G4int y );
    DsdHit* SetPosition( const G4ThreeVector& p );
    DsdHit* SetEnergy( G4double energy );
    DsdHit* AddTotalEnergy( G4double energy );
    DsdHit* SetProcessName( const G4String& name );
    
    bool IsSamePixel( const DsdHit& other );
    bool IsAdjacentPixel( const DsdHit& other );
    DsdHit* MergeSamePixel( const DsdHit& other );
    DsdHit* MergeAdjacentPixel( const DsdHit& other, G4int mode=0 );
};

using HitsCollection = G4THitsCollection<DsdHit>;

extern G4ThreadLocal G4Allocator<DsdHit>* DsdHitAllocator;

inline void* DsdHit::operator new(size_t)
{
    if (!DsdHitAllocator) {
	DsdHitAllocator = new G4Allocator<DsdHit>;
    }
    void *hit;
    hit = (void *) DsdHitAllocator->MallocSingle();
    return hit;
}

inline void DsdHit::operator delete(void *hit)
{
    if (!DsdHitAllocator) {
	DsdHitAllocator = new G4Allocator<DsdHit>;
    }
    DsdHitAllocator->FreeSingle((DsdHit*) hit);
}

inline DsdHit* DsdHit::AddTotalEnergy( G4double energy )
{
    total_energy += energy;
    return this;
}


// inline void DsdHit::Add(G4double de, G4double dl) {
//     fEdep += de; 
//     fTrackLength += dl;
// }

// inline G4double DsdHit::GetEdep() const { 
//     return fEdep; 
// }

// inline G4double DsdHit::GetTrackLength() const { 
//     return fTrackLength; 
// }

#endif
