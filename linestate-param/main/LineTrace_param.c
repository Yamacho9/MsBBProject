/**
 * @file LineTrace_param.c
 * @brief ライントレース（PID制御）のパラメータ読み込み
 * @author kanako baba ww
 * @date 2017/07/23
 */
 
 #include "LineTrace_param.h"
 
 /* パラメータファイル情報 */
 FILE *fp;
 char *filename = "/ev3rt/apps/param.txt";
 int buf[5][4];
 int param[5];
 
 
/**
 * LineTrace_param関数
 * input：現在の状態（直線・カーブ・一定等）
 * output：forward,kp,ki,kd
 * 0:成功　-1:失敗
 * 　それぞれのパラメータは「param.txt」で設定する
 */
 
int LineTrace_param(enum Line_status status, int8_t* forward, float* kp, float* ki, float* kd){
	
	/* forループ用変数 */
	int i=0;
	int j=0;
	
	/* パラメータファイルを読み込み用として開く */
	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("%sが開けませんでした\n",filename);
		/* default値を代入して戻る */
		*forward = D_FORWARD;
		*kp = D_KP;
		*ki = D_KI;
		*kd = D_KD;
		return -1;
	}
	
	/* ファイルを読み込んで配列bufに格納 */
	for (j=0; j<4; j++){
		for(i=0; i<5; i++){
			if(fscanf(fp,"%d,",&buf[i][j])!='\0'){
			}
		}
	}
	
	switch(status){
	
	case LINESTATUS_DEFAULT:
		for(j=0; j<4; j++){
			if(buf[0][j] == 0){//defaultの行ならばその行を読む
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_STRAIGHT:
		for(j=0; j<4; j++){
			if(buf[0][j] == 1){//straightの行ならばその行を読む
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_CURVE_WEAK:
		for(j=0; j<4; j++){
			if(buf[0][j] == 2){//weakの行ならばその行を読む
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_CURVE_STRONG:
		for(j=0; j<4; j++){
			if(buf[0][j] == 3){//strongの行ならばその行を読む
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	default:
		for(j=0; j<4; j++){
			if(buf[0][j] == 0){//defaultの行ならばその行を読む
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	}

	/* 値を代入 */
	*forward = param[1];
	*kp = (float)param[2]/100;
	*ki = (float)param[3]/100;
	*kd = (float)param[4]/100;
	
	fclose(fp);
	return 0;
	
}


