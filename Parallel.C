Int_t Parallel(Int_t RunNumber = 464){

  TChain* fChain = new TChain("T");
  fChain->AddFile(Form("/volatile/hallc/spring17/vargasa/ROOTfiles/shms_replay_production_%d_-1.root", RunNumber));

  TProof::Open("");
  fChain->SetProof();
  fChain->Process("MySelector.C+");

  return 0;
}

