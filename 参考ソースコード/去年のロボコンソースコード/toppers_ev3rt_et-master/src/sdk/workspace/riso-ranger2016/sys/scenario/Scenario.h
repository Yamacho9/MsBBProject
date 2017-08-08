///////////////////////////////////////////////////////////
//  Scenario.h
//  Implementation of the Class Scenario
//  Created on:      2016/08/04 13:29:31
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_BA5C7DE7_BC6E_46f0_BEC4_665AB2E9F880__INCLUDED_)
#define EA_BA5C7DE7_BC6E_46f0_BEC4_665AB2E9F880__INCLUDED_

#include "Sequencer.h"

class Scenario
{

public:
	Scenario();
	virtual ~Scenario();

	void init(Sequencer *sequencer);
	virtual void start() =0;

protected:
	Sequencer *m_Sequencer;

};
#endif // !defined(EA_BA5C7DE7_BC6E_46f0_BEC4_665AB2E9F880__INCLUDED_)
