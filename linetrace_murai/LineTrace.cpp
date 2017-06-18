/**
 * @brief Cg[X§ä
 * @param [in] targetVal ÚWÌõZTl
 * @param [in] currentVal »ÝÌõZTl
 * @param [in] opePeriod üú[s]
 * @param [in,out] errList Î·ð
 * @param [in,out] nextErrIndex ÌÎ·ðCfbNX
 * @return ùñpx[deg.] 100(EùñÅål)`-100(¶ùñÅål)
 * @detail õZTlðîÉPID§äðs¤
 */

#include "LineTrace.h"

#define KP					0.74 //Cg[X§ä äáW
#define KI					0.01 //Cg[X§ä ÏªW
#define KD					0.03 //Cg[X§ä ÷ªW

#define MAX_TURN_RIGHT	100
#define MAX_TURN_LEFT	-100

float LineTrace(int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex) {
	int err;	//Î·
	int i;	//Î·ðpCfbNX
	int errNum;	//Î·ð
	int lastErrIndex;	//OñÎ·ÌCfbNX
	int integral;	//Î·Ïª
	int diff;	//Î·÷ª
	float turn;	//ùñpx

	// P§ä
	err = currentVal - targetVal;	// üÌ¶¤ðg[X

	// I§ä
	errList[nextErrIndex] = err;
	errNum = sizeof errList / sizeof errList[0];
	integral = 0;
	for (i = 0; i < errNum; i++) {
		integral += errorList[i] * opePeriod;
	}

	// D§ä
	nextErrIndex == 0 ? lastErrIndex = errNum - 1 : lastErrIndex = nextErrIndex - 1;
	diff = (err - errList[lastErrIndex]) / opePeriod;

	turn = KP * (float)error + KI * (float)integral + KD * (float)diff;

	nextErrIndex + 1 < errNum ? nextErrIndex++ : nextErrIndex = 0;

	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}

}