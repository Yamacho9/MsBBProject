///////////////////////////////////////////////////////////
//  EmTimerId.h
//  Implementation of the Enumeration EmTimerId
//  Created on:      2016/04/28 15:18:33
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_71298F09_B5FE_499c_8A9F_B6F117E762AE__INCLUDED_)
#define EA_71298F09_B5FE_499c_8A9F_B6F117E762AE__INCLUDED_

// -----------------------------------------------
//! @enum  EmTimerId
//! @brief タイムアウト通知イベント定義
//!
//! システム全体のタイムアウト通知種類をIDとして定義する.
//! 数が多くなる可能性があるので、ヘッダファイルとして用意した.
// -----------------------------------------------
enum EmTimerId
{
	kDummyTimerId,
	kTempTimer,

	kMaxTimerId                 //!< 終端定義.
};
#endif // !defined(EA_71298F09_B5FE_499c_8A9F_B6F117E762AE__INCLUDED_)
