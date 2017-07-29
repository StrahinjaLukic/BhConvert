/*
 * StdhepWriter.cpp
 *
 *  Created on: 29.04.2011
 *      Author: weuste
 *  Adapted in 2017 by S. Lukic, Vinca Belgrade
 */

#include "StdhepWriter.h"

#include <map>
#include <iostream>
#include <iomanip>
StdhepWriter* StdhepWriter::_instance = NULL;

using namespace EVENT;
using namespace std;

StdhepWriter::StdhepWriter(const char* outputFile, const char* title, int nrEvtTot, int ist) :
			_outfile(outputFile), _ist(ist), _title(title), _nEvt(0), _is_open(false)
{
	if (StdhepWriter::_instance)
	{
		throw "Currently we support only one stdhep outputfile only!";
	}

	StdhepWriter::_instance = this;

	// the number of events we write to the file
	_nrEvtTot = nrEvtTot;

	this->init();
}

StdhepWriter::~StdhepWriter()
{
}


void StdhepWriter::open(const char* outputFile) {
  if (_is_open) {
    close();
  }
  _outfile = outputFile;
  _ist++;
  this->init();
}


void StdhepWriter::init()
{
  if (_is_open) {
    cout << "StdHep file already open.\n";
    return;
  }

	int ierr = StdHepXdrWriteInit((char*)_outfile, (char*)_title, _nrEvtTot, _ist);

	if (ierr != 0) {
		throw "Could not open StdHep file for writing";
		_is_open = false;
	}
	else {
	  _is_open = true;
	}
}

void StdhepWriter::writeEvent(EVENT::LCCollection* mcpCollection)
{
	map <MCParticle*, int>	pointer2Index;

	// fill the event number and number of particles into the HepEvt common block
	hepevt_.nevhep = _nEvt++;
	hepevt_.nhep = mcpCollection->getNumberOfElements();

//	cout << setw(3) << hepevt_.nevhep << " " << hepevt_.nhep << endl;

	// loop over all particles in the collection and translate them to stdhep
	for (int i = 0; i < mcpCollection->getNumberOfElements(); ++i)
	{
		// get the MCParticle + check
	  IMPL::MCParticleImpl* mcp = dynamic_cast< IMPL::MCParticleImpl* >(mcpCollection->getElementAt(i));
		if (!mcp)
			throw "This is not a MCParticle! Please pass only LCCollection containing MCParticle*";

		// particle id and generator status
		hepevt_.idhep[i] = mcp->getPDG();
		hepevt_.isthep[i] = mcp->getGeneratorStatus();

		// the indices of the mother / daughter have to be generated here
		pointer2Index[mcp] = i;

		// the momentum, energy and mass
		hepevt_.phep[i][0] = mcp->getMomentum()[0];
		hepevt_.phep[i][1] = mcp->getMomentum()[1];
		hepevt_.phep[i][2] = mcp->getMomentum()[2];
		hepevt_.phep[i][3] = mcp->getEnergy();
		hepevt_.phep[i][4] = mcp->getMass();

		// the vertex + time
		hepevt_.vhep[i][0] = mcp->getVertex()[0];
		hepevt_.vhep[i][1] = mcp->getVertex()[1];
		hepevt_.vhep[i][2] = mcp->getVertex()[2];
		hepevt_.vhep[i][3] = mcp->getTime();
	}

	// loop over all particles in the collection a 2nd time, to fill the mother/daughter relation
	for (int i = 0; i < mcpCollection->getNumberOfElements(); ++i)
	{
		// get the MCParticle
		MCParticle* mcp = dynamic_cast< MCParticle* >(mcpCollection->getElementAt(i));

		// helper variables of the start, stop and the current Index
		int minIdx = -1;
		int maxIdx = -1;
		int curIdx = -1;

		// first: get the min and max index for the daughters
		MCParticleVec daughters(mcp->getDaughters());

		for (unsigned iD = 0; iD < daughters.size(); ++iD)
		{
			curIdx = pointer2Index[daughters[iD]];
			if (minIdx == -1 || minIdx > curIdx)
				minIdx = curIdx;

			if (maxIdx == -1 || maxIdx < curIdx)
				maxIdx = curIdx;
		}

		// save the indices (increment it by 1, because Fortran is not 0 based)
		hepevt_.jdahep[i][0] = ++minIdx;
		hepevt_.jdahep[i][1] = ++maxIdx;


		// 2nd: get the min and max index for the parents
		maxIdx = -1;
		minIdx = -1;
		MCParticleVec mothers(mcp->getParents());

		for (unsigned iM = 0; iM < mothers.size(); ++iM)
		{
			curIdx = pointer2Index[mothers[iM]];
			if (minIdx == -1 || minIdx > curIdx)
				minIdx = curIdx;

			if (maxIdx == -1 || maxIdx < curIdx)
				maxIdx = curIdx;
		}

		// save the indices (increment it by 1, because Fortran is not 0 based)
		hepevt_.jmohep[i][0] = ++minIdx;
		hepevt_.jmohep[i][1] = ++maxIdx;


	}


	// write the HepEvt block to the file
	int ierr = StdHepXdrWrite(1, _ist);

	if (ierr != 0)
		throw "Error while writing in StdHep file";
}

void StdhepWriter::close()
{

	StdHepXdrEnd(_ist);
	_is_open = false;
}
