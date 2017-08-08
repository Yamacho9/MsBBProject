///////////////////////////////////////////////////////////
//  LineCondition.h
//  Implementation of the Class LineCondition
//  Created on:      2016/08/03 12:28:35
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_4C4970BB_A2D9_4f75_8886_FBCC9CD89663__INCLUDED_)
#define EA_4C4970BB_A2D9_4f75_8886_FBCC9CD89663__INCLUDED_

#include "Condition.h"

/**
 * 検出するライン種別を指定
 */
class LineCondition : public Condition
{

public:
	LineCondition();
	virtual ~LineCondition();

	virtual void onStart();
	virtual bool check();

};
#endif // !defined(EA_4C4970BB_A2D9_4f75_8886_FBCC9CD89663__INCLUDED_)
