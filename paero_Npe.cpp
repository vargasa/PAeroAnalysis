#include <TStyle.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <TCanvas.h>
#include <iostream>

#define ZAERO 235.
#define XLO -60.
#define XHI  60.
#define YLO -50.
#define YHI  50
#define NXBIN 120
#define NYBIN 100

//using namespace std;

// Plot coordinates of the tracks at the SHMS aerogel detector.

void paero_Npe() {

  Int_t RunNumber = 465;
  
  //Getting pointers from ROOT file
  TFile *fTT = new TFile(Form("shms_replay_production_%d_-1.root", RunNumber));

  TTree *fEntries = (TTree*)fTT->Get("T"); //TTree is called T in the file
  
  // I didn't find any T->Draw("P.tr.n","P.tr.n>10","")
  // Double_t fx[10];
  // Double_t fy[10];
  // Double_t fth[10];
  // Double_t fph[10];
  Double_t fnegNpe[7];
  Double_t fposNpe[7];
  Double_t fnegNpesum;
  Double_t fposNpesum;
  Double_t fn;
  
  // TBranch *b_x;
  // TBranch *b_y;
  // TBranch *b_th;
  // TBranch *b_ph;
  
  TBranch *b_n;
  TBranch *b_negNpe;
  TBranch *b_posNpe;
  TBranch *b_negNpesum;
  TBranch *b_posNpesum;

  fEntries->SetBranchStatus("*",0);
  fEntries->SetBranchStatus("P.tr.n",1);
  fEntries->SetBranchStatus("P.aero.negNpe",1);
  fEntries->SetBranchStatus("P.aero.posNpe",1);
  fEntries->SetBranchStatus("P.aero.negNpeSum",1);
  fEntries->SetBranchStatus("P.aero.posNpeSum",1);

  // fEntries->SetBranchAddress("P.tr.x", fx, &b_x);
  // fEntries->SetBranchAddress("P.tr.y", fy, &b_y);
  // fEntries->SetBranchAddress("P.tr.th", fth, &b_th);
  // fEntries->SetBranchAddress("P.tr.ph", fph, &b_ph);

  fEntries->SetBranchAddress("P.tr.n", &fn, &b_n);
  fEntries->SetBranchAddress("P.aero.negNpe", fnegNpe, &b_negNpe);
  fEntries->SetBranchAddress("P.aero.posNpe", fposNpe, &b_posNpe);
  fEntries->SetBranchAddress("P.aero.negNpeSum", &fnegNpesum, &b_negNpesum);
  fEntries->SetBranchAddress("P.aero.posNpeSum", &fposNpesum, &b_posNpesum);

  //Set the limits depending on T->Draw("*") Output
  TH1I *hn= new TH1I("hn", "Tracks per Event", 10, 0, 10);
  TH1I *hnegNpe = new TH1I("hnegNpe", "Photoelectrons per Event", 250, 0, 250);
  TH1I *hposNpe = new TH1I("hposNpe", "Photoelectrons per Event", 250, 0, 250);
  TH1I *hNpeDiff = new TH1I("hNpeDiff", "", 130, 0, 130);
  
  //Loop through the events
  Int_t Nentries = fEntries->GetEntries();
  //Just for testing purposes
  //Nentries = 1e3 ;
  
  cout << "Nentries= " << Nentries << endl;

  TCanvas *CG = new TCanvas();

  for( Int_t ientry= 0; ientry < Nentries; ientry++){
    fEntries->GetEntry(ientry);
    hn->Fill((int)fn);
    if((int)fn == 1){
      hnegNpe->Fill(TMath::Nint(fnegNpesum));
      hposNpe->Fill(TMath::Nint(fposNpesum));
      hNpeDiff->Fill(TMath::Abs(TMath::Nint(fnegNpesum)-TMath::Nint(fposNpesum)));
    }
    if (ientry%100000==1) cout << ientry << endl;
  }

  hn->SetTitle(Form("Tracks per Event Run Number: %d;Tracks;Counts (Number of Events)", RunNumber));
  hn->Draw();
  gPad->SetLogy();
  CG->Print(Form("TracksPerEvent_%d.png",RunNumber));
  delete hn;

  CG->cd();
  gPad->SetLogy();
  hnegNpe->SetTitle(Form("Photoelectrons per 1-Single track Events 'Negative side'\n Run Number: %d;Photoelectrons;Counts (Number of Events)", RunNumber));
  hnegNpe->Draw();
  CG->Print(Form("negNpe0_%d.png",RunNumber));

  CG->cd();
  gPad->SetLogy();
  hposNpe->SetTitle(Form("Photoelectrons per 1-Single track Events 'Positive side'\n Run Number: %d;Photoelectrons;Counts (Number of Events)", RunNumber));
  hposNpe->Draw();
  CG->Print(Form("posNpe0_%d.png",RunNumber));

  CG->cd();
  gPad->SetLogy();
  hNpeDiff->SetTitle(Form("Photoelectrons Difference per Event Run Number: %d;Photoelectrons Difference;Counts (Number of Events)", RunNumber));
  hNpeDiff->Draw();
  CG->Print(Form("DiffNpe0_%d.png",RunNumber));

  //T->Draw("P.tr.n","","")
  
}

