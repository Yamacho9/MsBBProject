///////////////////////////////////////////////////////////
//  LeftCourseScenario.cpp
//  Implementation of the Class LeftCourseScenario
//  Created on:      2016/08/04 13:35:24
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "LeftCourseScenario.h"
#include "Scenario.h"

#include "SitWaitAction.h"
#include "LineTraceAction.h"
#include "TailBrakeAction.h"
#include "RecliningAction.h"
#include "TailRunAction.h"
#include "TailRunAction.h"
#include "TailCalibrationAction.h"
#include "CommandCondition.h"
#include "TimeCondition.h"
#include "CollisionCondition.h"
#include "DistanceToObjectCondition.h"
#include "GlayLineCondition.h"
#include "InclinationCondition.h"
#include "DistanceCondition.h"
#include "TailStopCondition.h"
#include "EmptyCondition.h"
#include "RotationCondition.h"


LeftCourseScenario::LeftCourseScenario(){

}



LeftCourseScenario::~LeftCourseScenario(){

}


void LeftCourseScenario::start(){

	// シーケンス順設定
	Sequence* sequence;
	Sequence* firstSequence;
///////////////////////////////////
//スタート
///////////////////////////////////
  // 尻尾キャリブレーション部
	firstSequence = 							new Sequence(new TailCalibrationAction(),	new TailStopCondition(1000)	);
	sequence = firstSequence->setNextSequence(	new Sequence(new SitWaitAction(97),			new CommandCondition())	);
	// スタート部
	sequence = sequence->setNextSequence(	new Sequence(new LineTraceAction(20, 45, 0.5, new PIDControl(40, 0, 0)),	new DistanceCondition(300))		);	// スタート直後はゆっくり走行
//	sequence = sequence->setNextSequence(	new Sequence(new LineTraceAction(80, 45, new PIDControl(80, 0, 3000)),	new EmptyCondition())		);	// 直線終わりまで
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(80, 2, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(2100)));
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(60, 30, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(720)));
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(80, 2, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(2850)));
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(70, 30, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(1700)));
//////////////////////////////////////
//ゴール
//////////////////////////////////////
//////////////////////////////////////
//ルックアップ
//////////////////////////////////////
//	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(10, 10, 0.5, new PIDControl(80, 0.5, 3000)),	new TimeCondition(3000))	);	
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(30, 10, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceToObjectCondition(40))		);
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(20, 80, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceToObjectCondition(20))		);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(300))		);
	// !!! リクライニングの前に、尻尾の角度を80度にする !!!
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);	
	sequence = sequence->setNextSequence(		new Sequence(new RecliningAction(-10),		new InclinationCondition(64))	);
	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(10, 0),		new DistanceCondition(300))		);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);

	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(0, 15),		new RotationCondition(177))	);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);
	
	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(10, 0),		new DistanceCondition(340))		);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);
	
	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(0, 15),		new RotationCondition(177))	);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);

	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(10, 0),		new DistanceCondition(360))		);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(500))		);

// 尻尾走行からの倒立走行切り替え
	sequence = sequence->setNextSequence(		new Sequence(new TailRunAction(-10, 0),		new TimeCondition(500))		);
	sequence = sequence->setNextSequence(		new Sequence(new RecliningAction(80),		new InclinationCondition(115))	);
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(30, 97, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(300))	);
//////////////////////////////////////
//ルックアップ終わり
//////////////////////////////////////
///////////////////////////////////////
//グレーライン検知
//////////////////////////////////////
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(30, 10, 0.15, new PIDControl(80, 0.5, 3000)),	new GlayLineCondition())	);
///////////////////////////////////////
//ガレージシナリオ
///////////////////////////////////////
	//ToDo グレーラインの検知する位置が決まったら微調整
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(40, 60, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(80))	);
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(30, 80, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(80))	);
	sequence = sequence->setNextSequence(		new Sequence(new LineTraceAction(20, 80, 0.5, new PIDControl(80, 0.5, 3000)),	new DistanceCondition(40))	);
	sequence = sequence->setNextSequence(		new Sequence(new TailBrakeAction(),			new TimeCondition(700))		);
///////////////////////////////////////
//ガレージシナリオ終わり
/////////////////////////////////////////
	
	// シーケンス開始
	m_Sequencer->startSequence(firstSequence);
}
