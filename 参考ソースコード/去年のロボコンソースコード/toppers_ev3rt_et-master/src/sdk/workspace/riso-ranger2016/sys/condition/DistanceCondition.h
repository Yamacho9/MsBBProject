///////////////////////////////////////////////////////////
//  DistanceCondition.h
//  Implementation of the Class DistanceCondition
//  Created on:      2016/08/03 12:28:33
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_34E09B17_35C3_4016_801D_2788BFA732ED__INCLUDED_)
#define EA_34E09B17_35C3_4016_801D_2788BFA732ED__INCLUDED_

#include "Condition.h"

/**
 * シーケンスが遷移してからの距離変分を指定
 */
class DistanceCondition : public Condition
{

public:
	DistanceCondition(int distance);
	virtual ~DistanceCondition();

	void onStart();
	bool check();



private:
	int m_StartDistance;		// 基準距離
	int m_DistanceDifference;	// 指定の距離変分

};
#endif // !defined(EA_34E09B17_35C3_4016_801D_2788BFA732ED__INCLUDED_)
