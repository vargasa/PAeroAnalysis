#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TH3D.h"

#define ZAERO 235.
// Active area 60cm*90cm
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
 
class MySelector : public TSelector {
  
 private :

  // Variables used to access and store the data
  TTreeReader fReader;                            // The tree reader
  
  //Variables information can be fount at THcAerogel source code files
  //Drift chamber variables
  TTreeReaderValue<Double_t> fn;
  TTreeReaderArray<Double_t> fx;
  TTreeReaderArray<Double_t> fy;
  TTreeReaderArray<Double_t> fth;
  TTreeReaderArray<Double_t> fph;

  //Calorimeter Variables
  TTreeReaderValue<Double_t> fpreplane;
  TTreeReaderValue<Double_t> fcalearray;

  // Gas Cerenkov Variables
  TTreeReaderValue<Double_t> fhgcernpeSum;
  TTreeReaderValue<Double_t> fngcernpeSum;
  
  //Aerogel Variables
  TTreeReaderValue<Double_t> fsumNpe;
  TTreeReaderArray<Double_t> fposNpe;
  TTreeReaderArray<Double_t> fnegNpe;

  // Golden Track Variables
  TTreeReaderValue<Double_t> fgtrp;
  TTreeReaderValue<Double_t> fgtrbeta;

  TTreeReaderValue<Double_t> fhodbeta;

  // Histograms
  // Aerogel
  // for Variable descriptions hcana/src/THcAerogel.cxx
  TH1I *hnTracks;
  TH2D *hxy;
  TH2D *hNpeY;
  TH2D *hNpeX;
  TH3D *hxyNpe;

  // Heavy Gas Cherenkov
  // for Variable descriptions hcana/src/THcShowerPlane.cxx
  TH2D *hHGCPreShEnergy;
  
  // Noble Gas Cherenkov
  TH2D *hNGCPreShEnergy;

  // Calorimeter
  // For Variable descriptions hcana/src/THcShowerArray.cxx
  TH2D *hShEArrayPreShEnergy;

  // Golden Track
  TH2D *hgtrBetaP;
  
  TCanvas *ch;

 public :

  Int_t RunNumber;
  
  MySelector(TTree * = 0);
  virtual ~MySelector() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }
  
  ClassDef(MySelector,0);
};

