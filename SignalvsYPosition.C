#include "TStyle.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TTreeReader.h"
#include <iostream>

#define ZAERO 235.
#define XLO -60.
#define XHI 60.
#define YLO -50.
#define YHI 50.
#define NXBIN 120
#define NYBIN 100

//ROOTfile for 488 seems to be broken 
Int_t SignalvsYPosition(Int_t RunNumber = 929){

  TFile *fFile = TFile::Open(Form("../ROOTfiles/shms_replay_production_%d_-1.root", RunNumber));

  TTreeReader fReader("T",fFile);

  //Drift chamber variables
  TTreeReaderValue<Double_t> fn(fReader,"P.tr.n");
  TTreeReaderArray<Double_t> fx(fReader,"P.tr.x");
  TTreeReaderArray<Double_t> fy(fReader,"P.tr.y");
  TTreeReaderArray<Double_t> fth(fReader,"P.tr.th");
  TTreeReaderArray<Double_t> fph(fReader,"P.tr.ph");

  TCanvas *ch = new TCanvas("ch");
  TH1I *hnTracks = new TH1I("hnTracks",Form("Tracks per event Run:%d",RunNumber),15,0,15);
  TH2D *hxy = new TH2D("hxy",Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber),NXBIN,XLO,XHI,NYBIN,YLO,YHI);
  
  while(fReader.Next()){

      Int_t n = TMath::Nint(*fn);
      hnTracks->Fill(n);

      //Only takes into account one single track events
      if(n==1){
	//Projections into the Aerogel detector
	Float_t xh = fx[0] + fth[0]*ZAERO;
	Float_t yh = fy[0] + fph[0]*ZAERO;
        hxy->Fill(xh, yh);
      }
  }

  hxy->GetXaxis()->SetTitle("X-AeroAxis");
  hxy->GetYaxis()->SetTitle("Y-AeroAxis");
  hxy->GetZaxis()->SetTitle("Counts");
  hxy->Draw("LEGO");
  ch->Print(Form("SpacialAerogelDistribution_r%d.png",RunNumber));
  
  gPad->SetLogy();
  hnTracks->GetXaxis()->SetTitle("Number of Tracks");
  hnTracks->GetYaxis()->SetTitle("Counts");
  hnTracks->Draw();
  ch->Print(Form("TracksPerEvnt_r%d.png",RunNumber));

  return 0;
}
