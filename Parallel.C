Int_t Parallel(Int_t RunNumber = 464, TString fROOTFilesDir = "/volatile/hallc/spring17/vargasa/ROOTfiles/"){

  TChain* fChain = new TChain("T");
  fChain->AddFile(Form("%s/shms_replay_production_%d_-1.root", fROOTFilesDir.Data(), RunNumber));

  TProof *fProof = TProof::Open("workers=2");
  fProof->SetProgressDialog(false);
  // Send a TParameter<Int_t> Object to MySelector
  // Needed because of fProof use
  fProof->SetParameter("RunNumber",RunNumber);

  fChain->SetProof();  
  fChain->Process("MySelector.C+");

  return 0;
}

