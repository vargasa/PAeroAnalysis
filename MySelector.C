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

#include "TFile.h"
#include "MySelector.h"
#include "TParameter.h"

TGraph2D* MySelector::SpatialAerogelSignal(){

  TGraph2D *gxyNpe = new TGraph2D();

  Int_t nx = hxyNpe->GetNbinsX();
  Int_t ny = hxyNpe->GetNbinsY();
  Int_t nz = hxyNpe->GetNbinsZ();

  Int_t nbin;
  Int_t nn = 0;
  Double_t x, y, npe, nw, nevts;
  Double_t avgnpe = 0.0;

  for (Int_t i = 1; i <= nx; i++) {
    x = hxyNpe->GetXaxis()->GetBinCenter(i);
    for (Int_t j = 1; j <= ny; j++) {
      y = hxyNpe->GetYaxis()->GetBinCenter(j);  
      for (Int_t k = 1; k <= nz; k++) {
	//nbin = hxyNpe->GetBin(k,j,i);
	npe = hxyNpe->GetZaxis()->GetBinCenter(k);
	nw = hxyNpe->GetBinContent(i,j,k);
	avgnpe += (npe*nw);
	nevts += nw;
      }
      avgnpe = avgnpe/nevts;
      gxyNpe->SetPoint(nn,x,y,avgnpe);
      nevts = 0;
      avgnpe = 0.0;
      nn++;      
    }
  }
  return gxyNpe;
  
}

MySelector::MySelector(TTree *)
  : fn(fReader, "P.tr.n"),
    fx(fReader, "P.tr.x"),
    fy(fReader, "P.tr.y"),
    fth(fReader, "P.tr.th"),
    fph(fReader, "P.tr.ph"),
    fsumNpe(fReader,"P.aero.npeSum"),
    fposNpe(fReader,"P.aero.posNpe"),
    fnegNpe(fReader,"P.aero.negNpe"),
    fpreplane(fReader,"P.cal.pr.eplane"),
    fhgcernpeSum(fReader,"P.hgcer.npeSum"),
    fngcernpeSum(fReader,"P.ngcer.npeSum"),
    fcalearray(fReader,"P.cal.fly.earray"),
    fgtrp(fReader,"P.gtr.p"),
    fgtrbeta(fReader,"P.gtr.beta"),
    fhodbeta(fReader,"P.hod.beta")
{
  
  //Initialization avoids warning messages
  hnTracks = 0;
  hxy = 0;
  hNpeY = 0;
  hNpeX = 0;
  hxyNpe = 0;
  hHGCPreShEnergy = 0;
  hNGCPreShEnergy = 0;
  hShEArrayPreShEnergy = 0;
  hgtrBetaP = 0;
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

  hxyNpe = new TH3D("hxyNpe","",NXBIN,XLO,XHI,NYBIN,YLO,YHI,40,SGNLO,SGNHI);
  fOutput->Add(hxyNpe);

  // Values have been taken from hallc_replay/SHMS/PRODUCTION/CAL/pcal_histos.def
  hHGCPreShEnergy = new TH2D("hHGCPreShEnergy","",300.,0.,3.,40.,0.,20.);
  fOutput->Add(hHGCPreShEnergy);

  hNGCPreShEnergy = new TH2D("hNGCPreShEnergy","",300.,0.,3.,40.,0.,20.);
  fOutput->Add(hNGCPreShEnergy);

  // No information given 
  hShEArrayPreShEnergy = new TH2D("hShEtotPreShEnergy","",350,0.,5.,350,0.,10.);
  fOutput->Add(hShEArrayPreShEnergy);

  hgtrBetaP = new TH2D("hgtrBetaP","",300,0.,6.,300,0.,2.);
  fOutput->Add(hgtrBetaP);
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
     
     // Need to clarify this cut
     if ( *fpreplane>0.0) {
       if (*fhgcernpeSum>0.0) hHGCPreShEnergy->Fill(*fpreplane,*fhgcernpeSum);
       if (*fngcernpeSum>0.0) hNGCPreShEnergy->Fill(*fpreplane,*fngcernpeSum);
       if (*fcalearray>0.0) hShEArrayPreShEnergy->Fill(*fpreplane,*fcalearray);
       hgtrBetaP->Fill(*fgtrp,*fgtrbeta);
     }

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

  ch = new TCanvas("ch");

  TFile *fFile = new TFile(Form("Output/%d.root",RunNumber), "RECREATE");

  hxy->SetTitle(Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber));
  hxy->GetXaxis()->SetTitle("Y-AeroAxis (cm)");
  hxy->GetYaxis()->SetTitle("X-AeroAxis (cm)");
  hxy->GetZaxis()->SetTitle("Counts");
  hxy->SetOption("COLZ 0");
  hxy->Write();
  hxy->Draw("COLZ 0");
  ch->Print(Form("Output/xy_r%d_COL.png",RunNumber));
  hxy->Draw("LEGO2Z");
  ch->Print(Form("Output/xy_r%d_LEGO.png",RunNumber));

  hNpeY->SetTitle(Form("Signal vs Y Run:%d",RunNumber));
  hNpeY->GetXaxis()->SetTitle("Y-AeroAxis (cm)");
  hNpeY->GetYaxis()->SetTitle("Total Npe");
  hNpeY->GetZaxis()->SetTitle("Counts");
  hNpeY->SetOption("COLZ 0");
  hNpeY->Write();
  //hNpeY->Draw();
  //ch->Print(Form("SignalVsY_r%d.png",RunNumber));
  hNpeY->Draw("COLZ 0");
  ch->Print(Form("Output/SignalVsY_r%d_COL.png",RunNumber));
  hNpeY->Draw("LEGO");
  ch->Print(Form("Output/SignalVsY_r%d_LEGO.png",RunNumber));

  hNpeX->SetTitle(Form("Signal vs X Run:%d",RunNumber));
  hNpeX->GetXaxis()->SetTitle("X-AeroAxis (cm)");
  hNpeX->GetYaxis()->SetTitle("Total Npe");
  hNpeX->GetZaxis()->SetTitle("Counts");
  hNpeX->SetOption("COLZ 0");
  hNpeX->Write();
  hNpeX->Draw("COLZ 0");
  ch->Print(Form("Output/SignalVsX_r%d_COL.png",RunNumber));
  
  hxyNpe->SetTitle(Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber));
  hxyNpe->GetXaxis()->SetTitle("Y-AeroAxis (cm)");
  hxyNpe->GetYaxis()->SetTitle("X-AeroAxis (cm)");
  hxyNpe->GetZaxis()->SetTitle("fsumNpe");
  hxyNpe->SetOption("BOX2 Z");
  hxyNpe->Draw("BOX2 Z");
  hxyNpe->Write();
  ch->Print(Form("Output/xyNpe_r%d.png",RunNumber));

  TGraph2D *gxyNpe = this->SpatialAerogelSignal();
  gxyNpe->SetTitle("Npe vs Spatial Coordinates");
  gxyNpe->GetXaxis()->SetTitle("X-Aero(cm)");
  gxyNpe->GetYaxis()->SetTitle("Y-Aero(cm)");
  gxyNpe->GetZaxis()->SetTitle("Npe");
  gxyNpe->SetNpx(160);
  gxyNpe->SetNpy(160);
  gxyNpe->Draw("COLZ");
  ch->Print(Form("Output/gxyNpe_r%d.png",RunNumber));

  hHGCPreShEnergy->SetTitle(Form("SHMS HGC Total N.P.E. vs. PreSh Energy; Run:%d",RunNumber));
  hHGCPreShEnergy->GetXaxis()->SetTitle("Total PreSh Energy Deposition / 0.01 GeV");
  hHGCPreShEnergy->GetYaxis()->SetTitle("HGC Total N.P.E. / 0.5");
  hHGCPreShEnergy->SetOption("COLZ 0");
  hHGCPreShEnergy->Draw("COLZ 0");
  hHGCPreShEnergy->Write();
  ch->Print(Form("Output/hHGC_r%d.png",RunNumber));

  hNGCPreShEnergy->SetTitle(Form("SHMS NGC Total N.P.E. vs. PreSh Energy; Run:%d",RunNumber));
  hNGCPreShEnergy->GetXaxis()->SetTitle("Total PreSh Energy Deposition / 0.01 GeV");
  hNGCPreShEnergy->GetYaxis()->SetTitle("NGC Total N.P.E. / 0.5");
  hNGCPreShEnergy->SetOption("COLZ 0");
  hNGCPreShEnergy->Draw("COLZ 0");
  hNGCPreShEnergy->Write();
  ch->Print(Form("Output/hNGC_r%d.png",RunNumber));

  hShEArrayPreShEnergy->SetTitle(Form("Sh Array Energy vs. PreSh Energy; Run:%d",RunNumber));
  hShEArrayPreShEnergy->GetXaxis()->SetTitle("PreShower Energy / 0.1 GeV");
  hShEArrayPreShEnergy->GetYaxis()->SetTitle("Shower Array Energy / 0.1 GeV");
  gPad->SetLogz();
  hShEArrayPreShEnergy->SetOption("COLZ 0");
  hShEArrayPreShEnergy->Draw("COLZ 0");
  hShEArrayPreShEnergy->Write();
  ch->Print(Form("Output/hShPreSh_r%d.png",RunNumber));

  hgtrBetaP->SetTitle(Form("Gtr Beta vs Momentum; Run:%d",RunNumber));
  hgtrBetaP->GetXaxis()->SetTitle("Momentum");
  hgtrBetaP->GetYaxis()->SetTitle("Beta");
  hgtrBetaP->SetOption("COLZ 0");
  hgtrBetaP->Draw("COLZ 0");
  hgtrBetaP->Write();
  hgtrBetaP->SetStats(kFALSE);
  ch->Print(Form("Output/hGTR_r%d.png",RunNumber));

  hnTracks->SetTitle(Form("Tracks per event Run:%d",RunNumber));
  hnTracks->GetXaxis()->SetTitle("Number of Tracks");
  hnTracks->GetYaxis()->SetTitle("Counts");
  hnTracks->Write();
  gPad->SetLogy();
  hnTracks->Draw();
  ch->Print(Form("Output/TracksPerEvnt_r%d.png",RunNumber));

  fFile->Write();
  
}

