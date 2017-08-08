///////////////////////////////////////////////////////////
//  CollisionCondition.h
//  Implementation of the Class CollisionCondition
//  Created on:      2016/08/03 12:28:36
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_B9D80660_930A_4a06_AEF8_377875A921FD__INCLUDED_)
#define EA_B9D80660_930A_4a06_AEF8_377875A921FD__INCLUDED_

#include "Condition.h"

/**
 * 指定なし。衝突の瞬間のみ真となる。
 */
class CollisionCondition : public Condition
{

public:
	CollisionCondition();
	virtual ~CollisionCondition();

	virtual void onStart();
	virtual bool check();
	
private:
	int m_Velocity;				// 走行速度[mm/sec]
	int m_VelocityPrev;			// 単位時間前の走行速度[mm/sec]
	const int c_dif = 80;		// 閾値[mm/sec]

};
#endif // !defined(EA_B9D80660_930A_4a06_AEF8_377875A921FD__INCLUDED_)
