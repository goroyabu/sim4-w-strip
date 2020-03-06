/**
   @file    AnalysisManager.hpp
   @author  Goro Yabu
   @date    2020/02/04
**/

#ifndef AnalysisManager_hpp
#define AnalysisManager_hpp

#include <g4root.hh>

class AnalysisManager : public G4RootAnalysisManager
{
    struct column_index;
    
public:

    explicit AnalysisManager();
    ~AnalysisManager();

    static AnalysisManager* Instance();

    column_index GetColumnIndex(const G4String& name);
    G4int AddColumnIndex(const G4String& name, const column_index& column);
    G4bool ClearNtupleVector();
    
    G4int CreateNtupleIColumn(const G4String& name);
    G4int CreateNtupleDColumn(const G4String& name);
    G4int CreateNtupleIColumnV(const G4String& name, G4int maxSize=1);
    G4int CreateNtupleDColumnV(const G4String& name, G4int maxSize=1);
    
    G4bool FillNtupleIColumnName(const G4String& name, G4int value);
    G4bool FillNtupleDColumnName(const G4String& name, G4double value);
    G4bool FillNtupleIColumnVName(const G4String& name, G4int value);
    G4bool FillNtupleDColumnVName(const G4String& name, G4double value);            
    
    G4bool FillNtupleIColumnV(G4int columnId, G4int elementId, G4int value);
    G4bool FillNtupleDColumnV(G4int columnId, G4int elementId, G4double value);

    G4bool AddNtupleIColumnName(const G4String& name, G4int value);
    G4bool AddNtupleDColumnName(const G4String& name, G4double value);    
    
private:

    /** type_number **/
    static constexpr int int_number     = 0;
    static constexpr int float_number   = 1;
    static constexpr int double_number  = 2;
    static constexpr int string_number  = 3;
    
    static AnalysisManager* AnalysisManagerInstance;
    std::vector<G4int> IColumn;
    std::vector<G4double> DColumn;
    std::vector<std::vector<G4int>*> IColumnV;
    std::vector<std::vector<G4double>*> DColumnV;

    struct column_index
    {
	int ntupleId;
	int columnId;
	int maxSize;
	int typeNumber; /** @var typeNumber
			@detail 
			0 for int, I or G4int
			1 for float, F or G4float
			2 for double, D or G4double
			3 for string, S or G4String **/       
	int indexInType;
	bool isVector;
	bool isVariable;

	column_index()
	    : ntupleId(-1),
	      columnId(-1),
	      maxSize(-1),
	      typeNumber(-1),
	      indexInType(-1),
	      isVector(false),
	      isVariable(false)
	{}
	column_index(int ntupleId, int columnId, int maxSize,
		     int typeNumber, int indexInType=-1,
		     bool isVector=true, bool isVariable=true)
	    : ntupleId(ntupleId),
	      columnId(columnId),
	      maxSize(maxSize),
	      typeNumber(typeNumber),
	      indexInType(indexInType),
	      isVector(isVector),
	      isVariable(isVariable)
	      
	{
	    if ( this->maxSize == 1 ) {
		this->isVector = false;
		this->isVariable = false;
	    }
	}	
	column_index(const column_index& old)
	    : ntupleId(old.ntupleId),
	      columnId(old.columnId),
	      maxSize(old.maxSize),
	      typeNumber(old.typeNumber),
	      indexInType(old.indexInType),
	      isVector(old.isVector),
	      isVariable(old.isVariable)
	{}
	~column_index(){}
	column_index& operator=(const column_index& old)
	{
	    ntupleId = old.ntupleId;
	    columnId = old.columnId;
	    maxSize = old.maxSize;
	    typeNumber = old.typeNumber;
	    indexInType = old.indexInType;
	    isVector = old.isVector;
	    isVariable = old.isVariable;
	    return *this;
	}
    };
    std::map<G4String, column_index> ColumnIndex;
};

#endif
