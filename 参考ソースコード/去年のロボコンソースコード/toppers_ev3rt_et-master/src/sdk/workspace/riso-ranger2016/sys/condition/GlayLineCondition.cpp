/*
 * GlayLineCondition.cpp
 *
 *  Created on: 2016/08/31
 *      Author: Reina
 */

#include "GlayLineCondition.h"
#include "Bluetooth.h"


/**
 * 指定なし。グレーラインを検知した瞬間のみ真となる。
 */
GlayLineCondition::GlayLineCondition(){
	m_ColorValue = 0;
	m_ColorValuePrev = 0;
	m_Count = 0;
		
	for( int i = 0; i < c_arrayNum ; i++ ){
		ColorValueArray[i] = m_StateObserver->GetLinePosition();
//		bool_Array[i] = 0;
//		printf("INITColorValue%d=%f\n\n\n",i, ColorValueArray[m_Count]);
//		fprintf(btlog,"INITColorValue%d=%f\n = %f\r\n", i, ColorValueArray[m_Count]);


	}
}



GlayLineCondition::~GlayLineCondition(){

}



void GlayLineCondition::onStart(){

}


bool GlayLineCondition::check(){

	bool ret = false;

	m_ColorValue = m_StateObserver->GetLinePosition();
	
	ColorValueArray[m_Count] = m_ColorValue;

	
	//m_ColorValueDif  = m_ColorValue - m_ColorValuePrev;
	//m_ColorValuePrev = m_ColorValue;
	
	m_Count++;
	
	if (m_Count >= c_arrayNum){
		m_Count = 0;
//		printf("m_ColorValue = %f\n", m_ColorValue);
	}

	m_ColorValuePrev = ColorValueArray[m_Count];
	
	m_ColorValueDif  = m_ColorValue - m_ColorValuePrev;

	fprintf(btlog,"ColorValue = %f \n\r", m_ColorValue);
//	fprintf(btlog, "Direction = %f \n\r", m_StateObserver->GetDirection());
//	fprintf(btlog,"ColorValueDif = %f\r\n", m_ColorValueDif);
//	fprintf(btlog,"m_ColorValue = %f b_Array[%d] = %d \r\n", ColorValueArray[m_Count], m_Count, bool_Array[m_Count]);
//	fprintf(btlog,"ColorValue = %f\r\n,ColorValueDif = %f\r\n", ColorValueArray[m_Count],m_ColorValueDif);
//	fprintf(btlog,"b_Array[%d] = %d", m_Count, b_Array[m_Count]);
	
//	printf("ColorValue%d=%f\n\n\n", m_Count, ColorValueArray[m_Count]);
//	printf("ColorValueDif=%f\n\n\n", m_ColorValueDif);
//	printf("ColorValue = %f\n ColorValuePrev = %f\n", m_ColorValue, m_ColorValuePrev);
	
	// 条件判定
	
	if(m_ColorValueDif < c_dif){	// 一定時間前のカラーセンサ値との差が閾値以下だったら
		ret = true;
	}

/*
	int count = 0;
	for(int  j = 0; j < c_arrayNum ; j++ ){
		if(bool_Array[j] == 1){
			count++;
				
			if(count == c_arrayNum){	// 一定期間、カラーセンサ値の変動が一定範囲内だったら
				ret = true;
			}

		}
	}
*/	
	return ret;
	
}
