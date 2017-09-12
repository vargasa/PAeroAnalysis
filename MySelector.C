// For more information on the TSelector framework see
// $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.
// The following methods are defined in this file:
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers, a convenient place to create your histograms.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:

#include "MySelector.h"
#include "TParameter.h"

MySelector::MySelector(TTree *)
  : fn(fReader, "P.tr.n"),
    fx(fReader, "P.tr.x"),
    fy(fReader, "P.tr.y"),
    fth(fReader, "P.tr.th"),
    fph(fReader, "P.tr.ph"),
    fsumNpe(fReader,"P.aero.npeSum"),
    fposNpe(fReader,"P.aero.posNpe"),
    fnegNpe(fReader,"P.aero.negNpe") 
{
  //Initialization avoids warning messages
  hnTracks = 0;
  hxy = 0;
  hNpeY = 0;
  hNpeX = 0;
  hxyNpe = 0;
}

void MySelector::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree); 
  
}
void MySelector::Begin(TTree *tree) {
  
  // When using PROOF, Begin() is called on the client only.
  if (fInput->FindObject("RunNumber")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("RunNumber"));
    RunNumber = p->GetVal();
  }
  
}
 
void MySelector::SlaveBegin(TTree *tree) {
   // SlaveBegin() is a good place to create histograms.
   // For PROOF, this is called for each worker.
   // The TTree* is there for backward compatibility; e.g. PROOF passes 0.

  // fOutput is inherited from TSelector. Make sure you ::Add the objects
  // which will be used in different methods of the class (specially in Terminate).

  // Histogram's title is being set by the Client
  hnTracks = new TH1I("hnTracks","",15,0,15);
  fOutput->Add(hnTracks);
  
  hxy = new TH2D("hxy","",NXBIN,XLO,XHI,NYBIN,YLO,YHI);
  fOutput->Add(hxy);

  hNpeY = new TH2D("hNpeY","",NYBIN,YLO,YHI,100,SGNLO,SGNHI);
  fOutput->Add(hNpeY);
  
  hNpeX = new TH2D("hNpeX","",NXBIN,XLO,XHI,100,SGNLO,SGNHI);
  fOutput->Add(hNpeX);

  hxyNpe = new TH3D("hxyNpe","",12,XLO,XHI,10,YLO,YHI,20,SGNLO,SGNHI);
  fOutput->Add(hxyNpe);
 
}
 
Bool_t MySelector::Process(Long64_t entry) {
   // The Process() function is called for each entry in the tree to be
   // processed. The entry argument specifies which entry in the currently
   // loaded tree is to be processed.
   // It can be passed to either EventSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the TTree.
   //
   // This function should contain the "body" of the analysis: select relevant
   // tree entries, run algorithms on the tree entry and typically fill histograms.
 
   // *** 1. *** Tell the reader to load the data for this entry:
   fReader.SetEntry(entry);
 
   // *** 2. *** Do the actual analysis
   Int_t n = TMath::Nint(*fn);
   hnTracks->Fill(n);

   //Only takes into account one single track events
   if(n==1){
     //Projections into the Aerogel detector
     Float_t xh = fx[0] + fth[0]*ZAERO;
     Float_t yh = fy[0] + fph[0]*ZAERO;

     //We need the deference operator when using TTreeReaderValue instances
     hxy->Fill(yh, xh);
     hNpeY->Fill(yh, *fsumNpe);
     hNpeX->Fill(xh, *fsumNpe);
     hxyNpe->Fill(yh, xh, *fsumNpe);
   }

   return kTRUE;
}
 
void MySelector::Terminate() {
   // The Terminate() function is the last function to be called during the
   // analysis of a tree with a selector. It always runs on the client, it can
   // be used to present the results graphically or save the results to file.

  hnTracks->SetTitle(Form("Tracks per event Run:%d",RunNumber));
  hnTracks->GetXaxis()->SetTitle("Number of Tracks");
  hnTracks->GetYaxis()->SetTitle("Counts");
  
  hxy->SetTitle(Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber));
  hxy->GetXaxis()->SetTitle("Y-AeroAxis");
  hxy->GetYaxis()->SetTitle("X-AeroAxis");
  hxy->GetZaxis()->SetTitle("Counts");

  hNpeY->SetTitle(Form("Signal vs Y Run:%d",RunNumber));
  hNpeY->GetXaxis()->SetTitle("Y-AeroAxis");
  hNpeY->GetYaxis()->SetTitle("Total Npe");
  hNpeY->GetZaxis()->SetTitle("Counts");
  
  hNpeX->SetTitle(Form("Signal vs X Run:%d",RunNumber));
  hNpeX->GetXaxis()->SetTitle("X-AeroAxis");
  hNpeX->GetYaxis()->SetTitle("Total Npe");
  hNpeX->GetZaxis()->SetTitle("Counts");

  hxyNpe->SetTitle(Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber));
  hxyNpe->GetXaxis()->SetTitle("Y-AeroAxis");
  hxyNpe->GetYaxis()->SetTitle("X-AeroAxis");
  hxyNpe->GetZaxis()->SetTitle("fsumNpe");

  ch = new TCanvas("ch");
  hNpeX->Draw("COLZ");
  ch->Print(Form("Output/SignalVsX_r%d_COL.png",RunNumber));
  
  //hNpeY->Draw();
  //ch->Print(Form("SignalVsY_r%d.png",RunNumber));
  hNpeY->Draw("COLZ");
  ch->Print(Form("Output/SignalVsY_r%d_COL.png",RunNumber));
  hNpeY->Draw("LEGO");
  ch->Print(Form("Output/SignalVsY_r%d_LEGO.png",RunNumber));

  hxyNpe->Draw("BOX2 Z");
  ch->Print(Form("Output/xyNpe_r%d.png",RunNumber));

  hxy->Draw("COLZ");
  ch->Print(Form("Output/xy_r%d_COL.png",RunNumber));
  hxy->Draw("LEGO");
  ch->Print(Form("Output/xy_r%d_LEGO.png",RunNumber));
  
  gPad->SetLogy();
  hnTracks->Draw();
  ch->Print(Form("Output/TracksPerEvnt_r%d.png",RunNumber));
  
}

