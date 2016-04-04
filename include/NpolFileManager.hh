//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	         *
//********************************************************************

#ifndef Npol_File_Manager_h
#define Npol_File_Manager_h

#include <vector>

class G4String;
class TFile;

class NpolFileManager {

	public:
		static NpolFileManager *GetInstance();
		
		bool CheckIfChangingFiles(const G4int events);
		void ChangeFiles();
		void CloseFile();

	private:
		NpolFileManager(int instanceNum);
		~NpolFileManager();

		void ConstructFilenamePrefix();
		void OpenNextFile();

	private:
		static std::vector<NpolFileManager *> *instances;
		int instanceNo;

		int eventsPerFile;
		int fileNumber;
		TFile *outFile;
		G4String filenamePrefix;
};

#endif

