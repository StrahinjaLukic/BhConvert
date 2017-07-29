/******************************************************************
 *
 * 	Create stdhep file from BHLUMI output (Bhabha events)
 *
 * 	Strahinja Lukic, 2016
 *
 ******************************************************************/


// LCIO includes
#include "lcio.h"
#include <IMPL/MCParticleImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCEventImpl.h>
#include <UTIL/LCIOTypeInfo.h>

#include "StdhepWriter.h"

#include "math.h"
#include <fstream>
#include <cassert>

#include <string>
#include <cstdio>

using namespace std;




int main(int argc, char* argv[])
{
  int iarg = 1;

  string path(".");
  if(argc>iarg) path = argv[iarg];
  if(path.substr(path.size()-1, 1).compare("/") != 0) path += "/";

  iarg++;
  string stdhepname = path + "bhabha";
  if(argc>iarg) stdhepname = path + argv[iarg];
  if(stdhepname.substr(stdhepname.size()-7, 6).compare(".stdhep") == 0) {
    stdhepname.resize(stdhepname.size() - 6);
    char null[1] = {'0'};
    stdhepname.copy(null, 1, stdhepname.size()-1);
  }

  iarg++;
  string fname = path + "bhabha.dat";
  if(argc>iarg) fname = path + argv[iarg];
  ifstream leptons(fname.c_str());

  iarg++;
  fname = path + "bhabha_photon.dat";
  if(argc>iarg) fname = path + argv[iarg];
  ifstream photons(fname.c_str());

  if(!leptons.good())
  {
    std::cout << "Could not open the lepton file. Aborting.\n";
    return 1;
  }
  if(!photons.good())
  {
    std::cout << "Could not open the photon file. Aborting.\n";
    return 1;
  }


	const int evtPerFile = 100000;
	int fileCounter = 0;
	int eventCounter = 1;
	const int gensize = stdhepname.size();
  const int extsize = 13;
	char extension[extsize] = "_0000.stdhep";


  StdhepWriter * stdhepfile = NULL;

	IMPL::LCCollectionVec mcCollection(UTIL::lctypename<EVENT::MCParticle>());
	IMPL::MCParticleImpl *lastgamma = NULL;
	int lastgammaidx = 0;

	int iEvt = 0;

	while(leptons.good())
	{

	  double p[3];
	  double e;
	  int nphot, idx_phot;
	  IMPL::MCParticleImpl* pp = NULL;
		mcCollection.clear();

		if ( eventCounter > fileCounter*evtPerFile ) {

		  fileCounter++;
		  sprintf(extension, "_%04d.stdhep", fileCounter);
		  stdhepname.insert(gensize, extension, extsize);
      cout << "Opening output file " << stdhepname.c_str() << endl;
      if (stdhepfile) {
        stdhepfile->open(stdhepname.c_str());
      }
      else {
        stdhepfile = new StdhepWriter(stdhepname.c_str(), "BHLUMI Bhabha events", evtPerFile);
      }
		  cout << "Opened output file " << stdhepname.c_str() << endl;
		} // if ( eventCounter > fileCounter*evtPerFile )

	  leptons >> iEvt;

    if(iEvt%50000==0)
      std::cout << "Event # " << iEvt << std::endl;

    leptons >> p[0] >> p[1] >> p[2] >> e;

	  pp = new IMPL::MCParticleImpl;
	  pp->setMomentum(p);
	  pp->setMass(511.e-6);
	  pp->setCharge(-1.);
	  pp->setPDG(11);
	  pp->setGeneratorStatus(1);
//	  cout << "electron mass: " << pp->getMass() << " = " << sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) << endl;
//	  assert(fabs(sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) - pp->getMass()) < 1.e-9);
    mcCollection.addElement(pp);

    leptons >> p[0] >> p[1] >> p[2] >> e;
    pp = new IMPL::MCParticleImpl;
    pp->setMomentum(p);
    pp->setMass(511.e-6);
    pp->setCharge(1.);
    pp->setPDG(-11);
    pp->setGeneratorStatus(1);
//    cout << "Positron mass: " << pp->getMass() << " = " << sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) << endl;
  //  cout << "Positron pdg: " << pp->getPDG() << " ; charge: " << pp->getCharge() << endl;
 //   assert(sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) - pp->getMass() < 1.e-9);
    mcCollection.addElement(pp); // BHLUMI does not write negative e!

	  leptons >> nphot;
    int iphot = 0;

    if(lastgamma) { // This should never happen, but just in case...
      if(lastgammaidx == iEvt) {
        mcCollection.addElement(lastgamma);
        cout << "Successfully assigned the stray photon to event with index " << iEvt << endl;
        iphot++;
        lastgamma = NULL;
      }
    }

	  while (iphot<nphot) {

	    photons >> idx_phot >> p[0] >> p[1] >> p[2] >> e;
	    pp = new IMPL::MCParticleImpl;
	    pp->setMomentum(p);
	    pp->setMass(0.);
	    pp->setCharge(0.);
	    pp->setPDG(22);
	    pp->setGeneratorStatus(1);
//	    cout << "Gamma mass: " << pp->getMass() << " = " << sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) << endl;
	//    assert(sqrt(e*e - p[0]*p[0] - p[1]*p[1] - p[2]*p[2]) - pp->getMass() < 1.e-9);

	    if(idx_phot == iEvt) {
	      mcCollection.addElement(pp);
        iphot++;
	    }
      else { // This should never happen, but just in case...
        cout << "WARNING: Photon index mismatch.\n";
        cout << "Event index in lepton file: " << iEvt << endl;
        cout << "Event index in photon file: " << idx_phot << endl;
        if (lastgamma) {
          delete lastgamma;
          lastgamma = NULL;
        }
        if (idx_phot > iEvt) {
          lastgamma = pp;
          lastgammaidx = idx_phot;
          cout << nphot << " photons declared in the lepton file for this event but only " << iphot << " photons found.\n";
          cout << "The photon may be stored with the event with matching index, if such event is found.\n";
          break;
        }
        else {
          cout << "Apparently read a photon belonging to a previous event. Discarding.\n";
        }
      }
	  } // loop over photons

		stdhepfile->writeEvent(&mcCollection);
		eventCounter++;

		for(unsigned i=0; i<mcCollection.size(); i++) {
		  IMPL::MCParticleImpl*part = dynamic_cast<IMPL::MCParticleImpl*>(mcCollection.getElementAt(i));
//		  cout << "Deleting particle with PDG " << part->getPDG() << " and charge " << part->getCharge() << endl;
		  delete part;
		}

	} // loop over events
	stdhepfile->close();

	cout << "Wrote " << iEvt << " events.\n";

	mcCollection.clear();

	return 0;

}


