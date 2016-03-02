
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TLatex.h>
#include <TAttAxis.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TString.h>

void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2, 
		     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05,
		     Float_t vSpacing = 0.0, Float_t hSpacing = 0.0);

void SimulationFigures() {

  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 
  std::string histoNames[3][3]={{"pi-","mu+","gamma"},
				{"neutron","mu-","e+"},
				{"proton","pi+","e-"}};

  TString Lead = "10"; TString Energy = "4.4"; TString Bfield = "4";
  TString OutputDir = "/work/hallc/cgen/tireman/MagFieldOn/MagField_" + Bfield + "Bdl/Plots/";
   TString InputDir = "/work/hallc/cgen/tireman/MagFieldOn/MagField_" + Bfield + "Bdl/LeadOn" + Lead +"cm/";

  TString OutputFile = OutputDir + "JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Fig21-23.root";
  TString InputFile = InputDir + "JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";

  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");

  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = ((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  //Double_t fluxscaling1 = 1/(totalElectrons*1.602e-13*(98*60));  // My scale
  //Double_t fluxscaling2 = 1/(totalElectrons*1.602e-13*(42.99*26.32));  // My scale
  Double_t fluxscaling1 = 1/(totalElectrons*(98*60));  //Proposal Scale
  Double_t fluxscaling2 = 1/(totalElectrons*(42.99*26.32));  //Proposal Scale

  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e6 << " Million" << std::endl;

  TCanvas *c1 = new TCanvas("c1","NPOL Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  Int_t Nx = 3, Ny =3, fillStyle = 1001;
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.07, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  CanvasPartition(c1,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);

  TPad *pad[Nx][Ny];
    
  for(int i = 0; i < Nx; i++){
   for(int j = 0; j < Ny; j++){
     c1->cd(0);
     // Get the pads previosly created.
     char pname[24];
     sprintf(pname,"pad_%i_%i",i,j);
     pad[i][j] = (TPad*) gROOT->FindObject(pname);
     pad[i][j]->Draw();
     pad[i][j]->SetLogx();
     pad[i][j]->SetLogy();
     pad[i][j]->SetFillStyle(4000);
     pad[i][j]->SetFrameFillStyle(4000);
     pad[i][j]->cd();
     // Size factors
     Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
     Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     char hname[24];
     std::string str = histoNames[i][j];
     sprintf(hname,"NpolFlux_%s",histoNames[i][j].c_str());
     TH1F *hFrame = (TH1F*) inFile->Get(hname);
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetFillStyle(fillStyle);
     hFrame->Scale(fluxscaling1);
     hFrame->Draw();
  
     // y axis range
     hFrame->GetYaxis()->SetRangeUser(2e-15,4e-8); // proposal scale
     //hFrame->GetYaxis()->SetRangeUser(0.005,200); // my scale
     
     // Format for y axis
     //hFrame->GetYaxis()->SetTitle("#frac{Particles}{#muA #times cm^{2}}"); // My scale
     hFrame->GetYaxis()->SetTitle("#frac{Particles}{electron #times cm^{2}}"); // Proposal scale
     hFrame->GetYaxis()->SetLabelFont(43);
     hFrame->GetYaxis()->SetLabelSize(16);
     hFrame->GetYaxis()->SetLabelOffset(0.02);
     hFrame->GetYaxis()->SetTitleFont(43);
     hFrame->GetYaxis()->SetTitleSize(16);
     hFrame->GetYaxis()->SetTitleOffset(5);
     
     hFrame->GetYaxis()->CenterTitle();
     hFrame->GetYaxis()->SetNdivisions(505);
     
     // TICKS Y Axis
     hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
     
     // Format for x axis
     hFrame->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
     hFrame->GetXaxis()->SetLabelFont(43);
     hFrame->GetXaxis()->SetLabelSize(16);
     hFrame->GetXaxis()->SetLabelOffset(0.02);
     hFrame->GetXaxis()->SetTitleFont(43);
     hFrame->GetXaxis()->SetTitleSize(16);
     hFrame->GetXaxis()->SetTitleOffset(5);
     hFrame->GetXaxis()->CenterTitle();
     hFrame->GetXaxis()->SetNdivisions(505);
     
     // Set X axis range
     hFrame->GetXaxis()->SetRangeUser(1e-1,8e3);

     // TICKS X Axis
     hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
     
   }
  }


  TCanvas *c2 = new TCanvas("c2","Target Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  Nx = 3, Ny =3, fillStyle = 1001;
  lMargin = 0.10, rMargin = 0.05, bMargin = 0.07, tMargin = 0.05;
  vSpacing = 0.0; hSpacing = 0.0;
  CanvasPartition(c2,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
    
  for(int i = 0; i < Nx; i++){
   for(int j = 0; j < Ny; j++){
     c2->cd(0);
     // Get the pads previosly created.
     char pname[24];
     sprintf(pname,"pad_%i_%i",i,j);
     pad[i][j] = (TPad*) gROOT->FindObject(pname);
     pad[i][j]->Draw();
     pad[i][j]->SetLogx();
     pad[i][j]->SetLogy();
     pad[i][j]->SetFillStyle(4000);
     pad[i][j]->SetFrameFillStyle(4000);
     pad[i][j]->cd();
     // Size factors
     Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
     Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     char hname[24];
     std::string str = histoNames[i][j];
     sprintf(hname,"TargetFlux_%s",histoNames[i][j].c_str());
     TH1F *hFrame = (TH1F*) inFile->Get(hname);
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetFillStyle(fillStyle);
     hFrame->Scale(fluxscaling2);
     hFrame->Draw();

     // y axis range
     hFrame->GetYaxis()->SetRangeUser(2e-15,4e-8); // proposal scale
     //hFrame->GetYaxis()->SetRangeUser(0.5,500000); // my scale
     
     // Format for y axis
     //hFrame->GetYaxis()->SetTitle("#frac{Particles}{#muA #times cm^{2}}"); // My scale
     hFrame->GetYaxis()->SetTitle("#frac{Particles}{electron #times cm^{2}}"); // Proposal Scale
     hFrame->GetYaxis()->SetLabelFont(43);
     hFrame->GetYaxis()->SetLabelSize(16);
     hFrame->GetYaxis()->SetLabelOffset(0.02);
     hFrame->GetYaxis()->SetTitleFont(43);
     hFrame->GetYaxis()->SetTitleSize(16);
     hFrame->GetYaxis()->SetTitleOffset(5);
     
     hFrame->GetYaxis()->CenterTitle();
     hFrame->GetYaxis()->SetNdivisions(505);
     
     // TICKS Y Axis
     hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
     
     // Format for x axis
     hFrame->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
     hFrame->GetXaxis()->SetLabelFont(43);
     hFrame->GetXaxis()->SetLabelSize(16);
     hFrame->GetXaxis()->SetLabelOffset(0.02);
     hFrame->GetXaxis()->SetTitleFont(43);
     hFrame->GetXaxis()->SetTitleSize(16);
     hFrame->GetXaxis()->SetTitleOffset(5);
     hFrame->GetXaxis()->CenterTitle();
     hFrame->GetXaxis()->SetNdivisions(505);

     // Set X axis range
     hFrame->GetXaxis()->SetRangeUser(1e-1,8e3);

     // TICKS X Axis
     hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
     
   }
  }

 TCanvas *c2b = new TCanvas("c2b","Correlated Target Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  Nx = 3, Ny =3, fillStyle = 1001;
  lMargin = 0.10, rMargin = 0.05, bMargin = 0.07, tMargin = 0.05;
  vSpacing = 0.0; hSpacing = 0.0;
  CanvasPartition(c2b,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
    
  for(int i = 0; i < Nx; i++){
   for(int j = 0; j < Ny; j++){
     c2b->cd(0);
     // Get the pads previosly created.
     char pname[24];
     sprintf(pname,"pad_%i_%i",i,j);
     pad[i][j] = (TPad*) gROOT->FindObject(pname);
     pad[i][j]->Draw();
     pad[i][j]->SetLogx();
     pad[i][j]->SetLogy();
     pad[i][j]->SetFillStyle(4000);
     pad[i][j]->SetFrameFillStyle(4000);
     pad[i][j]->cd();
     // Size factors
     Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
     Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     char hname[34];
     std::string str = histoNames[i][j];
     sprintf(hname,"Correlated_TargetFlux_%s",histoNames[i][j].c_str());
     TH1F *hFrame = (TH1F*) inFile->Get(hname);
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetFillStyle(fillStyle);
     hFrame->Scale(fluxscaling2);
     hFrame->Draw();

     // y axis range
     hFrame->GetYaxis()->SetRangeUser(2e-15,2e-8); // proposal scale
     //hFrame->GetYaxis()->SetRangeUser(0.5,500000); // my scale
     
     // Format for y axis
     //hFrame->GetYaxis()->SetTitle("#frac{Particles}{#muA #times cm^{2}}"); // My scale
     hFrame->GetYaxis()->SetTitle("#frac{Particles}{electron #times cm^{2}}"); // Proposal Scale
     hFrame->GetYaxis()->SetLabelFont(43);
     hFrame->GetYaxis()->SetLabelSize(16);
     hFrame->GetYaxis()->SetLabelOffset(0.02);
     hFrame->GetYaxis()->SetTitleFont(43);
     hFrame->GetYaxis()->SetTitleSize(16);
     hFrame->GetYaxis()->SetTitleOffset(5);
     
     hFrame->GetYaxis()->CenterTitle();
     hFrame->GetYaxis()->SetNdivisions(505);
     
     // TICKS Y Axis
     hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
     
     // Format for x axis
     hFrame->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
     hFrame->GetXaxis()->SetLabelFont(43);
     hFrame->GetXaxis()->SetLabelSize(16);
     hFrame->GetXaxis()->SetLabelOffset(0.02);
     hFrame->GetXaxis()->SetTitleFont(43);
     hFrame->GetXaxis()->SetTitleSize(16);
     hFrame->GetXaxis()->SetTitleOffset(5);
     hFrame->GetXaxis()->CenterTitle();
     hFrame->GetXaxis()->SetNdivisions(505);
     
     // Set X axis range
     hFrame->GetXaxis()->SetRangeUser(1e-1,8e3);

     // TICKS X Axis
     hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
     
   }
  }
     
  TCanvas *c3 = new TCanvas("c3","Position in Npol Tagger with Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
 
  lMargin = 0.07, rMargin = 0.03, bMargin = 0.08, tMargin = 0.05;
  Nx = 3; Ny = 3;
  vSpacing = 0.0; hSpacing = 0.0;
  CanvasPartition(c3,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
    
  for(int i = 0; i < Nx; i++){
   for(int j = 0; j < Ny; j++){
     c3->cd(0);
     // Get the pads previosly created.
     char pname[16];
     sprintf(pname,"pad_%i_%i",i,j);
     pad[i][j] = (TPad*) gROOT->FindObject(pname);
     pad[i][j]->Draw();
     //pad[i][j]->SetLogz();
     pad[i][j]->SetFillStyle(4000);
     pad[i][j]->SetFrameFillStyle(4000);
     pad[i][j]->cd();
     // Size factors
     Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
     Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     char hname[16];
     std::string str = histoNames[i][j];
     sprintf(hname,"npolXY_%s",histoNames[i][j].c_str());
     TH1F *hFrame = (TH1F*) inFile->Get(hname);
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetOption("Cont1");
     hFrame->SetFillStyle(fillStyle);
     hFrame->Draw();
     
     // Format for Z axis
     hFrame->GetZaxis()->SetTitle("#frac{Particles}{#muA #times cm^{2}}");
     
     // Z axis range
     hFrame->GetZaxis()->SetRangeUser(0.001,1.0*hFrame->GetMaximum());
     
     // Format for y axis
     hFrame->GetYaxis()->SetTitle("Vertical Position (cm)");
     hFrame->GetYaxis()->SetLabelFont(43);
     hFrame->GetYaxis()->SetLabelSize(16);
     hFrame->GetYaxis()->SetLabelOffset(0.02);
     hFrame->GetYaxis()->SetTitleFont(43);
     hFrame->GetYaxis()->SetTitleSize(16);
     hFrame->GetYaxis()->SetTitleOffset(4);
     
     hFrame->GetYaxis()->CenterTitle();
     hFrame->GetYaxis()->SetNdivisions(505);
     
     // TICKS Y Axis
     hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
     
     // Format for x axis
     hFrame->GetXaxis()->SetTitle("Horizontal Position (cm)");
     hFrame->GetXaxis()->SetLabelFont(43);
     hFrame->GetXaxis()->SetLabelSize(16);
     hFrame->GetXaxis()->SetLabelOffset(0.02);
     hFrame->GetXaxis()->SetTitleFont(43);
     hFrame->GetXaxis()->SetTitleSize(16);
     hFrame->GetXaxis()->SetTitleOffset(4);
     hFrame->GetXaxis()->CenterTitle();
     hFrame->GetXaxis()->SetNdivisions(505);
     
     // TICKS X Axis
     hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
     
   }
  }

  
  c1->Write();
  c2->Write();
  c2b->Write();
  c3->Write();
  outFile->Close();
  
  //inFile->Close();

}

void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin,
		     Float_t vSpacing, Float_t hSpacing)
{
   if (!C) return;

   // Setup Pad layout:
   //Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;

   //Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;

   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;

   for (Int_t i=0;i<Nx;i++) {

      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }

      for (Int_t j=0;j<Ny;j++) {

         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }

         C->cd(0);

         char name[16];
         sprintf(name,"pad_%i_%i",i,j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
	 pad->SetTopMargin(vmaru);

         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);

         pad->Draw();
      }
   }
}
