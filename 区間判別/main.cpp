#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define DistMargine 100
#define DegMargine 10
#define OffLineSeconds 1.5

void reading(void);
void split(char* s, const std::string& delim);
bool JudgingOnLine(int, int);

int *arr0, *arr1;
int linenum = 0; // �f�[�^�̍s���𐔂���

int main()
{
	std::cerr << "main start" << std::endl;
	reading();
	JudgingOnLine(6122,80);

	free(arr0);
	free(arr1);
	return 0;
}

int i = 0;
//int *arr0, *arr1;

void reading() {

	std::cerr << "reading" << std::endl;

	std::ifstream ifs;  // �t�@�C���ǂݎ��p�X�g���[��
	ifs.open("sumL.txt");	// �t�@�C���I�[�v��

	if (ifs.fail()) {	// �t�@�C���I�[�v���Ɏ��s�����炻���ŏI��
		std::cerr << "�t�@�C�����J���܂���\n";
		exit(1);
	}

	char buf[256];	// �f�[�^�ꎞ�ۊǗp�z��

	while (ifs.getline(buf, sizeof(buf))) {	// �t�@�C������1�s���ǂݍ���
		linenum++;	// �s�����J�E���g���Ă���
	}

	std::cerr << "�ǂݍ��񂾍s�� = " << linenum << "\n";

	ifs.clear(); // �t�@�C�������ɓ��B�Ƃ����t���O���N���A
	ifs.seekg(0, std::ios::beg);	// �t�@�C���擪�ɖ߂�

	arr0 = (int *)malloc(linenum * sizeof(int));
	arr1 = (int *)malloc(linenum * sizeof(int));

	for (i = 0; i<linenum; i++) {
		ifs.getline(buf, sizeof(buf));	// ��s�Ǎ�
		split(buf, ",");
		std::cout << i << " = " << arr0[i] << ", " << arr1[i] << std::endl;
	}

	return;
}

void split(char* s, const std::string& delim)
{
	using namespace std;
	int count = 0;
	char *tok;

	//std::cerr << "split" << std::endl;

	tok = strtok(s, delim.c_str());
	while (tok != NULL) {
		if ((count % 2) == 0) {
			arr0[i] = atoi(tok);
		}
		else {
			arr1[i] = atoi(tok);
		}
		tok = strtok(NULL, delim.c_str());  /* 2��ڈȍ~ */
		count++;
	}
	//printf("i=%d, count=%d, arr0=%d, arr1=%d\n", i, count, arr0[i], arr1[i]);
}

int flag=0;
bool JudgingOnLine(int distance, int degree) {
	bool result = true; //on line
	int degMax=0, degMin=0;
	flag = linenum;

	printf("judge %d\n", flag);

	//distance range check
	while (1) {
		if ((arr0[linenum - (flag-1)] + DistMargine) <= distance) { //���̋�Ԃ�
			flag--;
			printf("callednum=%d, arr0[]=%d\n", linenum-flag, arr0[linenum - flag]);
		}
		else {
			printf("else %d\n", linenum-flag);
			break;
		}
	}

	printf("���:%d (distance: %d - %d )\n", (linenum-flag+1), arr0[linenum - flag], arr0[linenum - (flag-1)]);
	
	return (linenum-flag+1);

	/*
	if (arr1[linenum - flag] <= arr1[linenum - (flag - 1)]) {
		degMax = arr1[linenum - (flag - 1)];
		degMin = arr1[linenum - flag];
	}else {
		degMax = arr1[linenum - flag];
		degMin =  arr1[linenum - (flag - 1)];
	}

	printf("degMinn=%d\n", (degMin - DegMargine));
	printf("degMaxx=%d\n", (degMax + DegMargine));
	printf("disMinn=%d\n", (arr0[linenum - flag] - DistMargine));
	printf("disMaxx=%d\n", (arr0[linenum - (flag-1)] + DistMargine));

	if ((degMin - DegMargine) <= degree && degree <= (degMax + DegMargine) 
		&& (arr0[linenum - flag] - DistMargine)<=distance && distance<=(arr0[linenum - (flag-1)] + DistMargine)) {
		result = true;
		printf("OK\n");
	}else {
		result = false;
		printf("NG\n");
	}

	std::cout << false << std::endl; //true=1, false=0

	return result;
	*/
}
