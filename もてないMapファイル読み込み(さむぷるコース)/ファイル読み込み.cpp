#include <fstream>
#include <iostream>
#include <string>

double *arr0, *arr1;
int datanum;

void ReadMapFile(void);

int main()
{
	ReadMapFile();
	
	return 0;
}

void ReadMapFile(){
	std::ifstream ifs;  // �t�@�C���ǂݎ��p�X�g���[��
	ifs.open("robocon.txt");	// �t�@�C���I�[�v��

	if (ifs.fail()) {	// �t�@�C���I�[�v���Ɏ��s�����炻���ŏI��
		std::cerr << "�t�@�C�����J���܂���\n";
		exit(1);
	}

	char buf[256];	// �f�[�^�ꎞ�ۊǗp�z��

	int linenum = 0; // �f�[�^�̍s���𐔂���

	while (ifs.getline(buf, sizeof(buf))) {	// �t�@�C������1�s���ǂݍ���
		linenum++;	// �s�����J�E���g���Ă���
	}

	std::cerr << "�ǂݍ��񂾍s�� = " << linenum << "\n";

	ifs.clear(); // �t�@�C�������ɓ��B�Ƃ����t���O���N���A
	ifs.seekg(0, std::ios::beg);	// �t�@�C���擪�ɖ߂�

	double *arr0, *arr1;
	datanum = (linenum) / 2;
	arr0 = new double[datanum];	// �s�� linenum ���̔z��𓮓I�Ɋm��
	arr1 = new double[datanum];	// �s�� linenum ���̔z��𓮓I�Ɋm��

	for (int i = 0; i<datanum; i++) {
		ifs.getline(buf, sizeof(buf));	// ��s�ǂݍ���Łc
		arr0[i] = atof(buf);	// �����z��Ɋi�[
		ifs.getline(buf, sizeof(buf));	// ��s�ǂݍ���Łc
		arr1[i] = atof(buf);	// �����z��Ɋi�[
		//std::cout << i << " = " << arr0[i] << ", "  << arr1[i] << std::endl;
	}	
}
