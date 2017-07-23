/**
 * @file LineTrace_param.c
 * @brief ���C���g���[�X�iPID����j�̃p�����[�^�ǂݍ���
 * @author kanako baba ww
 * @date 2017/07/23
 */
 
 #include "LineTrace_param.h"
 
 /* �p�����[�^�t�@�C����� */
 FILE *fp;
 char *filename = "/ev3rt/apps/param.txt";
 int buf[5][4];
 int param[5];
 
 
/**
 * LineTrace_param�֐�
 * input�F���݂̏�ԁi�����E�J�[�u�E��蓙�j
 * output�Fforward,kp,ki,kd
 * 0:�����@-1:���s
 * �@���ꂼ��̃p�����[�^�́uparam.txt�v�Őݒ肷��
 */
 
int LineTrace_param(enum Line_status status, int8_t* forward, float* kp, float* ki, float* kd){
	
	/* for���[�v�p�ϐ� */
	int i=0;
	int j=0;
	
	/* �p�����[�^�t�@�C����ǂݍ��ݗp�Ƃ��ĊJ�� */
	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("%s���J���܂���ł���\n",filename);
		/* default�l�������Ė߂� */
		*forward = D_FORWARD;
		*kp = D_KP;
		*ki = D_KI;
		*kd = D_KD;
		return -1;
	}
	
	/* �t�@�C����ǂݍ���Ŕz��buf�Ɋi�[ */
	for (j=0; j<4; j++){
		for(i=0; i<5; i++){
			if(fscanf(fp,"%d,",&buf[i][j])!='\0'){
			}
		}
	}
	
	switch(status){
	
	case LINESTATUS_DEFAULT:
		for(j=0; j<4; j++){
			if(buf[0][j] == 0){//default�̍s�Ȃ�΂��̍s��ǂ�
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_STRAIGHT:
		for(j=0; j<4; j++){
			if(buf[0][j] == 1){//straight�̍s�Ȃ�΂��̍s��ǂ�
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_CURVE_WEAK:
		for(j=0; j<4; j++){
			if(buf[0][j] == 2){//weak�̍s�Ȃ�΂��̍s��ǂ�
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	case LINESTATUS_CURVE_STRONG:
		for(j=0; j<4; j++){
			if(buf[0][j] == 3){//strong�̍s�Ȃ�΂��̍s��ǂ�
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	default:
		for(j=0; j<4; j++){
			if(buf[0][j] == 0){//default�̍s�Ȃ�΂��̍s��ǂ�
				for(i=0; i<5; i++){
					param[i] = buf[i][j];
				}
			}
		}
		break;
	}

	/* �l���� */
	*forward = param[1];
	*kp = (float)param[2]/100;
	*ki = (float)param[3]/100;
	*kd = (float)param[4]/100;
	
	fclose(fp);
	return 0;
	
}


