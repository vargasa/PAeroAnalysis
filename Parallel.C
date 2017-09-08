Int_t Parallel(Int_t RunNumber = 464, TString fROOTFilesDir = "/volatile/hallc/spring17/vargasa/ROOTfiles/"){

  TChain* fChain = new TChain("T");
  fChain->AddFile(Form("%s/shms_replay_production_%d_-1.root", fROOTFilesDir.Data(), RunNumber));

  TProof *fProof = TProof::Open("workers=4");
  fProof->SetProgressDialog(false);
  //fProof->AddInput(new TNamed("RunNumber",Form("%d",RunNumber)));

  fChain->SetProof();  
  fChain->Process("MySelector.C+");

  return 0;
}

