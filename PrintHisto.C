// From ROOT6.06 is better to use  $rootprint
// see  https://root.cern.ch/how/how-quickly-inspect-content-file
void PrintHisto (Int_t RunNumber = 929) {
  
  TFile *f1 = TFile::Open(Form("Output/%d.root", RunNumber));
  TIter keyList(f1->GetListOfKeys());
  TKey *key;
  TCanvas c1;
			  
  while ((key = (TKey*)keyList())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1 *h = (TH1*)key->ReadObj();
    
    h->Draw();
    c1.Print(Form("Output/%s.png", h->GetTitle()));
  }

}
