/*
 * StdhepWriter.h
 *
 *  Created on: 29.04.2011
 *      Author: weuste
 *  Adapted in 2017 by S. Lukic, Vinca Belgrade
 */

#ifndef STDHEPWRITER_H_
#define STDHEPWRITER_H_

#include "stdhep.h"
#include "stdlun.h"
#include "stdcnt.h"
#include "stdhep_mcfio.h"

#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"
#include "IMPL/MCParticleImpl.h"

//using namespace EVENT;

class StdhepWriter
{
public:
	StdhepWriter(const char* outputFile, const char* title = "", int nrEvtTot = 10, int ist=0);
	virtual ~StdhepWriter();

	void init();
	void writeEvent(EVENT::LCCollection* mcpCollection);
	void open(const char* outputFile);
	void close();

	bool is_open() const { return _is_open; }

protected:
	const char*	_outfile;

	int	_ist;
	int _nrEvtTot;
	const char* _title;

	unsigned _nEvt;

	bool _is_open;

private:
	static StdhepWriter*	_instance;
};

//extern StdhepWriter::_instance;

#endif /* STDHEPWRITER_H_ */
