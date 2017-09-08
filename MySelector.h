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
  
  //Aerogel Variables
  TTreeReaderValue<Double_t> fsumNpe;
  TTreeReaderArray<Double_t> fposNpe;
  TTreeReaderArray<Double_t> fnegNpe;

  //Histograms
  TH1I *hnTracks;
  TH2D *hxy;
  TH2D *hNpeY;
  TH2D *hNpeX;
  TH3D *hxyNpe;
  
  TCanvas *ch;

  Int_t RunNumber = 464;

 public :
  
 MySelector(TTree * = 0): fn(fReader, "P.tr.n"),
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
  virtual ~MySelector() { }
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }
  
  ClassDef(MySelector,0);
};

