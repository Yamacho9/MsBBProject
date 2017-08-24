#include "judgeSection.h"

/*
int main()
{
	std::cerr << "main start" << std::endl;
	reading();
	JudgingOnLine(6122,80);

	free(arr0);
	free(arr1);
	return 0;
}
*/

int judgeSection(int distance, int degree) {
	//bool result = true; //on line
	//int degMax=0, degMin=0;
	int linenum = getlinenum();
	int flag = linenum;

	printf("judge %d\n", flag);

	//distance range check
	while (1) {
		
		if(arr0[linenum-1]<=distance) {
			return (linenum-1);
		}
		if ((arr0[linenum - (flag-1)] + DistMargine) <= distance) { //???I?a?O?O
			flag--;
			printf("callednum=%d, arr0[]=%d\n", linenum-flag, arr0[linenum - flag]);
		}
		else {
			printf("else %d\n", linenum-flag);
			break;
		}
	}

	printf("‹æŠÔ:%d (distance: %d - %d )\n", (linenum-flag+1), arr0[linenum - flag], arr0[linenum - (flag-1)]);
	
	return (linenum-flag+1);

}
