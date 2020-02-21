int draw()
{
    int nhits;
    std::vector<int>    * detid = nullptr;
    std::vector<double> * edep = nullptr;
    tree->SetBranchAddress( "nhits",    &nhits );
    tree->SetBranchAddress( "detid",    &detid );
    tree->SetBranchAddress( "edep",     &edep  );

    auto h = new TH1D("h", "h", 1000, 0, 1000);
    
    for ( int i = 0; i<tree->GetEntries(); ++i ) {
    	tree->GetEntry(i);
	if ( nhits == 0 ) continue;

	double etot = 0.0;
	for ( auto e : *edep ) {
	    // if ( e <= 10.0 ) continue;
	    etot += e;
	}

	h->Fill( etot );
    }

    h->Draw();
    gPad->SetLogy();
    return 0;
}
