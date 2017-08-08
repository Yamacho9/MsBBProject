///////////////////////////////////////////////////////////
//  TimerEvent.h
//  Implementation of the Class TimerEvent
//  Created on:      2016/04/28 14:48:19
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_5F470FC4_44EB_4fdf_8D60_16295EAF938A__INCLUDED_)
#define EA_5F470FC4_44EB_4fdf_8D60_16295EAF938A__INCLUDED_

#include "TimerId.h"


class TimerEvent
{

public:
	TimerEvent();
	virtual ~TimerEvent();

    // -----------------------------------------------------------------------------
    //! @brief タイムアウト通知.
    //!
    //! タイマからのイベントタイムアウト通知.
    //! @param[in]     rv_id タイムアウトイベント種別.
    //! @exception none
    // -----------------------------------------------------------------------------
    void ntfTimeOut(EmTimerId rv_id);

};
#endif // !defined(EA_5F470FC4_44EB_4fdf_8D60_16295EAF938A__INCLUDED_)
