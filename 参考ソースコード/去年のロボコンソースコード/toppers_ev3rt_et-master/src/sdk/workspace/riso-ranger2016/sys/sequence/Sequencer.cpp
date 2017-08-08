///////////////////////////////////////////////////////////
//  Sequencer.cpp
//  Implementation of the Class Sequencer
//  Created on:      2016/08/03 12:01:55
//  Original author: 039389
///////////////////////////////////////////////////////////

#include "Sequencer.h"
#include "Action.h"
#include "SitWaitAction.h"
#include "EmptyCondition.h"
#include <t_syslog.h>


Sequencer::Sequencer(Sequence* sequence){

	// メンバ初期化
	m_CurrentSequence = sequence;

	// 初期シーケンス開始処理
	//m_CurrentSequence->getAction()->onStart();
	//m_CurrentSequence->getCondition()->onStart();
}


Sequencer::~Sequencer(){

}


void Sequencer::startSequence(Sequence* sequence){

	// シーケンス終了処理
	m_CurrentSequence->getAction()->onStop();

	// シーケンスdelete
	m_CurrentSequence->deleteAllFollowingSequences();
	delete m_CurrentSequence;

	// シーケンス更新
	m_CurrentSequence = sequence;

	// 新シーケンス初期処理
	m_CurrentSequence->getAction()->onStart();
	m_CurrentSequence->getCondition()->onStart();

}


void Sequencer::cycle(){

	// 現在の単動作の周期処理実行
	m_CurrentSequence->getAction()->onCycle();
	//syslog(LOG_EMERG, "Scquencer 1\n");
	// 現在のConditionの判定処理実行
	if(m_CurrentSequence->getCondition()->check()) {
		syslog(LOG_EMERG, "Scquencer 2\n");
		// Action終了処理実行
		m_CurrentSequence->getAction()->onStop();
		syslog(LOG_EMERG, "Scquencer 3\n");
		// 次のシーケンス取得
		Sequence* sequence = m_CurrentSequence->getNextSequence();
		syslog(LOG_EMERG, "Scquencer 4\n");
		// 次のシーケンスが無い場合はデフォルト設定のシーケンスをセット
		if(sequence == NULL) {
			sequence = new Sequence(new SitWaitAction(80), new EmptyCondition());
			syslog(LOG_EMERG, "Scquencer 5\n");
		}
		syslog(LOG_EMERG, "Scquencer 6\n");
		// 旧シーケンスdelete
		delete m_CurrentSequence;
		syslog(LOG_EMERG, "Scquencer 7\n");
		// 現在のシーケンスを更新
		m_CurrentSequence = sequence;
		syslog(LOG_EMERG, "Scquencer 8\n");
		// 新シーケンス初期処理


		m_CurrentSequence->getCondition()->onStart();
		syslog(LOG_EMERG, "Scquencer 10\n");
		m_CurrentSequence->getAction()->onStart();
		syslog(LOG_EMERG, "Scquencer 9\n");
	}

}

