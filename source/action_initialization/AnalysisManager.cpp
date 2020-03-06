/**
   @file    AnalysisManager.cpp
   @author  Goro Yabu
   @date 2020/02/04
**/

#include "AnalysisManager.hpp"

#include <algorithm>

using std::cout;
using std::endl;

AnalysisManager * AnalysisManager::AnalysisManagerInstance = 0;

AnalysisManager * AnalysisManager::Instance()
{
   if ( AnalysisManagerInstance == 0 ) {
       AnalysisManagerInstance = new AnalysisManager();
   }   
   return AnalysisManagerInstance;
} 
AnalysisManager::AnalysisManager()
    : G4RootAnalysisManager()
{
    AnalysisManagerInstance = this;
}
AnalysisManager::~AnalysisManager()
{
    AnalysisManagerInstance = 0;
}

AnalysisManager::column_index AnalysisManager::GetColumnIndex(const G4String& name)
{
    auto itr = ColumnIndex.find(name);
    if ( itr == ColumnIndex.end() ) return column_index();
    return itr->second;
}
G4int AnalysisManager::AddColumnIndex(const G4String& name, const column_index& column)
{
    if ( this->GetColumnIndex(name).ntupleId != -1 ) return -1;
    ColumnIndex[name] = column; return 0;
}
G4bool AnalysisManager::ClearNtupleVector()
{
    for ( auto&& column : IColumn  ) column = 0;
    for ( auto&& column : DColumn  ) column = 0.0;
    for ( auto&& column : IColumnV ) column->clear();
    for ( auto&& column : DColumnV ) column->clear();
    return true;
}

G4int AnalysisManager::CreateNtupleIColumn(const G4String& name)
{
    auto index = column_index( 0, ColumnIndex.size(), 1,
			       this->int_number, IColumn.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    IColumn.emplace_back(0);
    return G4RootAnalysisManager::CreateNtupleIColumn(name);
}
G4int AnalysisManager::CreateNtupleDColumn(const G4String& name)
{
    auto index = column_index( 0, ColumnIndex.size(), 1,
			       this->double_number, DColumn.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    DColumn.emplace_back( 0.0 );
    return G4RootAnalysisManager::CreateNtupleDColumn(name);
}
G4int AnalysisManager::CreateNtupleIColumnV(const G4String& name, G4int maxSize)
{
    auto index = column_index( 0, ColumnIndex.size(), maxSize,
			       this->int_number, IColumnV.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;
    
    auto column = new std::vector<G4int>; column->reserve(maxSize);
    IColumnV.emplace_back( column );
    return this->G4RootAnalysisManager::CreateNtupleIColumn(name, *column);
}
G4int AnalysisManager::CreateNtupleDColumnV(const G4String& name, G4int maxSize)
{
    auto index = column_index( 0, ColumnIndex.size(), maxSize,
			       this->double_number, DColumnV.size() );
    if ( this->AddColumnIndex( name, index ) < 0 ) return -1;

    auto column = new std::vector<G4double>; column->reserve(maxSize);
    DColumnV.emplace_back( column );
    return this->G4RootAnalysisManager::CreateNtupleDColumn(name, *column);
}

G4bool AnalysisManager::FillNtupleIColumnName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    return G4RootAnalysisManager::FillNtupleIColumn(index.columnId, value);
}
G4bool AnalysisManager::FillNtupleDColumnName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    DColumn[ index.indexInType ] = value;
    return G4RootAnalysisManager::FillNtupleDColumn(index.columnId, value);
}
G4bool AnalysisManager::FillNtupleIColumnVName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.maxSize <= (int)IColumnV[ index.indexInType ]->size() ) return false;
    IColumnV[ index.indexInType ]->emplace_back( value );
    return true;
}
G4bool AnalysisManager::FillNtupleDColumnVName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.maxSize <= (int)DColumnV[ index.indexInType ]->size() ) return false;
    DColumnV[ index.indexInType ]->emplace_back( value );
    return true;
}

G4bool AnalysisManager::FillNtupleIColumnV(G4int columnId, G4int elementId, G4int value)
{
    if ( (int)IColumnV.size()<=columnId ) return false;
    if ( (int)IColumnV[columnId]->size()<=elementId ) return false;
    IColumnV[columnId]->at(elementId) = value;
    return true;
}

G4bool AnalysisManager::FillNtupleDColumnV(G4int columnId, G4int elementId, G4double value)
{
    if ( (int)DColumnV.size()<=columnId ) return false;
    if ( (int)DColumnV[columnId]->size()<=elementId ) return false;
    DColumnV[columnId]->at(elementId) = value;
    return true;
}

G4bool AnalysisManager::AddNtupleIColumnName(const G4String& name, G4int value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->int_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    IColumn[ index.indexInType ] += value;
    return G4RootAnalysisManager::FillNtupleIColumn(index.columnId,
						    IColumn[index.indexInType] );
}
G4bool AnalysisManager::AddNtupleDColumnName(const G4String& name, G4double value)
{
    auto index = this->GetColumnIndex( name );
    if ( index.typeNumber != this->double_number ) return false;
    if ( index.isVector!=false || index.isVariable!=false ) return false;
    DColumn[ index.indexInType ] += value;
    return G4RootAnalysisManager::FillNtupleDColumn(index.columnId,
						    DColumn[index.indexInType] );
}
