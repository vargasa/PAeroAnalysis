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
//Signal
//#define SGNBIN 10
#define SGNLO 0
#define SGNHI 50

//ROOTfile for 488 seems to be broken 
Int_t SignalvsYPosition(Int_t RunNumber = 929, TString fROOTFilesDir = "/volatile/hallc/spring17/vargasa/ROOTfiles/"){

  TFile *fFile = TFile::Open(Form("%s/shms_replay_production_%d_-1.root", fROOTFilesDir.Data(), RunNumber));

  TTreeReader fReader("T",fFile);

  //Drift chamber variables
  TTreeReaderValue<Double_t> fn(fReader,"P.tr.n");
  TTreeReaderArray<Double_t> fx(fReader,"P.tr.x");
  TTreeReaderArray<Double_t> fy(fReader,"P.tr.y");
  TTreeReaderArray<Double_t> fth(fReader,"P.tr.th");
  TTreeReaderArray<Double_t> fph(fReader,"P.tr.ph");
  
  //Aerogel Variables 
  TTreeReaderValue<Double_t> fsumNpe(fReader,"P.aero.npeSum");
  TTreeReaderArray<Double_t> fposNpe(fReader,"P.aero.posNpe");
  TTreeReaderArray<Double_t> fnegNpe(fReader,"P.aero.negNpe");


  TCanvas *ch = new TCanvas("ch");
  TH1I *hnTracks = new TH1I("hnTracks",Form("Tracks per event Run:%d",RunNumber),15,0,15);
  TH2D *hxy = new TH2D("hxy",Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber),NXBIN,XLO,XHI,NYBIN,YLO,YHI);
  TH2D *hNpeY = new TH2D("hNpeY",Form("Signal vs Y Run:%d",RunNumber),NYBIN,YLO,YHI,100,SGNLO,SGNHI);
  TH2D *hNpeX = new TH2D("hNpeX",Form("Signal vs X Run:%d",RunNumber),NXBIN,XLO,XHI,100,SGNLO,SGNHI);
  TH3D *hxyNpe = new TH3D("hxyNpe",Form("Spacial distribution of events in the Aerogel Detector Run:%d",RunNumber),12,XLO,XHI,10,YLO,YHI,20,SGNLO,SGNHI);

  
  while(fReader.Next()){

      Int_t n = TMath::Nint(*fn);
      hnTracks->Fill(n);

      //Only takes into account one single track events
      if(n==1){
	//Projections into the Aerogel detector
	Float_t xh = fx[0] + fth[0]*ZAERO;
	Float_t yh = fy[0] + fph[0]*ZAERO;

        hxy->Fill(yh, xh);
	hNpeY->Fill(yh, *fsumNpe);
	hNpeX->Fill(xh, *fsumNpe);
	hxyNpe->Fill(yh, xh, *fsumNpe);
      }
  }

  hNpeX->GetXaxis()->SetTitle("X-AeroAxis");
  hNpeX->GetYaxis()->SetTitle("Total Npe");
  hNpeX->GetZaxis()->SetTitle("Counts");
  hNpeX->Draw("COLZ");
  ch->Print(Form("Output/SignalVsX_r%d_COL.png",RunNumber));

  hNpeY->GetXaxis()->SetTitle("Y-AeroAxis");
  hNpeY->GetYaxis()->SetTitle("Total Npe");
  hNpeY->GetZaxis()->SetTitle("Counts");
  //hNpeY->Draw();
  //ch->Print(Form("SignalVsY_r%d.png",RunNumber));
  hNpeY->Draw("COLZ");
  ch->Print(Form("Output/SignalVsY_r%d_COL.png",RunNumber));
  hNpeY->Draw("LEGO");
  ch->Print(Form("Output/SignalVsY_r%d_LEGO.png",RunNumber));
 
  hxyNpe->GetXaxis()->SetTitle("Y-AeroAxis");
  hxyNpe->GetYaxis()->SetTitle("X-AeroAxis");
  hxyNpe->GetZaxis()->SetTitle("fsumNpe");
  hxyNpe->Draw("BOX2 Z");
  ch->Print(Form("Output/xyNpe_r%d.png",RunNumber));

  hxy->GetXaxis()->SetTitle("Y-AeroAxis");
  hxy->GetYaxis()->SetTitle("X-AeroAxis");
  hxy->GetZaxis()->SetTitle("Counts");
  hxy->Draw("COLZ");
  ch->Print(Form("Output/xy_r%d_COL.png",RunNumber));
  hxy->Draw("LEGO");
  ch->Print(Form("Output/xy_r%d_LEGO.png",RunNumber));
  
  gPad->SetLogy();
  hnTracks->GetXaxis()->SetTitle("Number of Tracks");
  hnTracks->GetYaxis()->SetTitle("Counts");
  hnTracks->Draw();
  ch->Print(Form("Output/TracksPerEvnt_r%d.png",RunNumber));

  return 0;
}
