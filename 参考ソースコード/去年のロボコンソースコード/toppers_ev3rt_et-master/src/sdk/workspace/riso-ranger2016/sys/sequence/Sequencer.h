///////////////////////////////////////////////////////////
//  Sequencer.h
//  Implementation of the Class Sequencer
//  Created on:      2016/08/03 12:01:55
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_F418CAF2_0897_4136_8D10_CE7B01E2D986__INCLUDED_)
#define EA_F418CAF2_0897_4136_8D10_CE7B01E2D986__INCLUDED_

#include "Sequence.h"

class Sequencer
{

public:
	Sequencer(Sequence* sequence);
	virtual ~Sequencer();
	void startSequence(Sequence* sequence);
	void cycle();

private:
	Sequence* m_CurrentSequence;

};
#endif // !defined(EA_F418CAF2_0897_4136_8D10_CE7B01E2D986__INCLUDED_)
