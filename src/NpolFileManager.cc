//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#include <cstdio>
#include <cstdlib>
#include <string>
#include <pthread.h>

#include <G4ios.hh>
#include <G4String.hh>
#ifdef G4MULTITHREADED
#include <G4Threading.hh>
#endif

#include <TFile.h>

#include "NpolFileManager.hh"
#include "NpolAnalysisManager.hh"

static pthread_mutex_t fileMan_lock = PTHREAD_MUTEX_INITIALIZER;

std::vector<NpolFileManager *> *NpolFileManager::instances = NULL;

NpolFileManager *NpolFileManager::GetInstance() {
#ifdef G4MULTITHREADED
	G4int threadId = G4Threading::G4GetThreadId();
	if(threadId < 0)  // Either we're on the master thread or running in sequential mode
		threadId = 0;
#else
	G4int threadId = 0;
#endif

	pthread_mutex_lock(&fileMan_lock);

	if(instances == NULL) 
		instances = new std::vector<NpolFileManager *>();

	if(instances->capacity() <= threadId) 
		instances->resize(threadId+1, NULL);

	if((*instances)[threadId] == NULL)
		(*instances)[threadId] = new NpolFileManager(threadId);
	
	pthread_mutex_unlock(&fileMan_lock);

	return (*instances)[threadId];
}

NpolFileManager::NpolFileManager(int instanceNum) {
	G4cout << "Constructing NpolFileManager singleton on thread #"
	   << (instanceNo = instanceNum) << G4endl;

	const char *eventsPerFileString = getenv("NPOLEVENTSPERFILE");
	if(eventsPerFileString == NULL)
		eventsPerFileString = "100000";
	eventsPerFile = atoi(eventsPerFileString);

	outFile = NULL;
	fileNumber = 1;
	ConstructFilenamePrefix();
}

NpolFileManager::~NpolFileManager() {
	delete outFile;
}

// Construct the prefix for output filenames from enviornment variables, or
// use defaults if enviornment variables are not defined.
void NpolFileManager::ConstructFilenamePrefix() {
	const char *dirName = getenv("NPOLDIR");
	const char *baseName = getenv("NPOLBASENAME");
	const char *jobNumber = getenv("JOBNUMBER");
	char threadNumber[4];
	sprintf(threadNumber,"%02d",instanceNo);

	filenamePrefix = "" + 
		(dirName != NULL ? (G4String)dirName + "/root" : "output") + "/"
		+ (baseName != NULL ? baseName : "npol") + "_"
		+ (jobNumber != NULL ? jobNumber : "99999") + "_"
		+ threadNumber + "_";
}

// Close the current file, open a new one, and increment the file number.
void NpolFileManager::OpenNextFile() {
	char fileNumberString[11];
	sprintf(fileNumberString,"%05d",fileNumber++);

	G4String filename = filenamePrefix + fileNumberString + ".root";
	
	delete outFile;
	outFile = new TFile(filename,"RECREATE");
	G4cout << filename << " was opened." << G4endl;
}

void NpolFileManager::CloseFile() {
	if(outFile != NULL)
		outFile->Write();
	delete outFile;
	outFile = NULL;
}

bool NpolFileManager::CheckIfChangingFiles(const G4int events) {
//	G4cout << events << "!" << G4endl;
	if(eventsPerFile == 0) return false;
	return !(events % eventsPerFile);
}

void NpolFileManager::ChangeFiles() {
	if(outFile != NULL)
		outFile->Write();
	OpenNextFile();
}

TFile *NpolFileManager::GetCurrentFile() {
	return outFile;
}

