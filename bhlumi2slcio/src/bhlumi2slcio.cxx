/******************************************************************
 *
 * 	Create slcio file from BHLUMI output (Bhabha events)
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
#include <SIO/SIOWriter.h>
#include <IMPL/LCRunHeaderImpl.h>

#include "math.h"
#include <fstream>
#include <cstdio>
//#include <cassert>

using namespace std;


int open(const char *genname, int fileCounter, SIO::SIOWriter *lcWriter, IMPL::LCRunHeaderImpl *rnHdr) {
  static const int extsize = 12;
  char extension[extsize] = "_0000.slcio";
  string slcioname(genname);
  const int gensize = slcioname.size();
//  slcioname.resize(gensize + extsize);
  sprintf(extension, "_%04d.slcio", fileCounter);
  slcioname.insert(gensize, extension, extsize);

  try {
    lcWriter->open(slcioname.c_str());
    cout << "Opened output file " << slcioname.c_str() << endl;
  }
  catch (IO::IOException &e) {
    cout << "WARNING: Overwriting file " << slcioname.c_str() << endl;
    lcWriter->open(slcioname.c_str(), lcio::LCIO::WRITE_NEW);
  }

  lcWriter->writeRunHeader(rnHdr);

  return 0;
}


int main(int argc, char* argv[])
{
  int iarg = 1;

  string path(".");
  if(argc>iarg) path = argv[iarg];
  if(path.substr(path.size()-1, 1).compare("/") != 0) path += "/";

  iarg++;
  string slcioname = path + "bhabha";
  if(argc>iarg) slcioname = path + argv[iarg];
  if(slcioname.substr(slcioname.size()-7, 6).compare(".slcio") == 0) {
    slcioname.resize(slcioname.size() - 6);
    char null[1] = {'0'};
    slcioname.copy(null, 1, slcioname.size()-1);
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
	int fileCounter = 1;
	int eventCounter = 1;


	SIO::SIOWriter * lcWriter = new SIO::SIOWriter;
  IMPL::LCRunHeaderImpl *rnHdr = new IMPL::LCRunHeaderImpl();
  rnHdr->setDescription("BHLUMI Bhabha events");
  rnHdr->setDetectorName("Generator events");
  rnHdr->setRunNumber(0);

  open(slcioname.c_str(), fileCounter, lcWriter, rnHdr);

  IMPL::LCEventImpl *evt = NULL;

	IMPL::LCCollectionVec *mcCollection = NULL;
	IMPL::MCParticleImpl *lastgamma = NULL;
	int lastgammaidx = 0;

	int iEvt = 0;

	while(leptons.good())
	{

	  double p[3];
	  double e;
	  int nphot, idx_phot;

	  if(evt) {
	    delete evt;
	    evt = NULL;
	  }
	  evt = new IMPL::LCEventImpl();
	  IMPL::MCParticleImpl* pp = NULL;
		mcCollection = new IMPL::LCCollectionVec(UTIL::lctypename<EVENT::MCParticle>());
		evt->addCollection(mcCollection, "MCBhabha");

		if ( eventCounter > fileCounter*evtPerFile ) {
		  try { lcWriter->close(); }
		  catch (IO::IOException &ex) {
		    cout << "Cannot close file. " << ex.what() << "\n";
		  }
		  fileCounter++;
		  open(slcioname.c_str(), fileCounter, lcWriter, rnHdr);
		}

	  leptons >> iEvt;
	  evt->setEventNumber(iEvt);

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
    mcCollection->addElement(pp);

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
    mcCollection->addElement(pp); // BHLUMI does not write negative e!

	  leptons >> nphot;
	  int iphot = 0;

	  if(lastgamma) { // This should never happen, but just in case...
	    if(lastgammaidx == iEvt) {
	      mcCollection->addElement(lastgamma);
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
	      mcCollection->addElement(pp);
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

	  lcWriter->writeEvent(evt);
		eventCounter++;

	}
	lcWriter->close();

	cout << "Wrote " << iEvt << " events.\n";

	return 0;

}


