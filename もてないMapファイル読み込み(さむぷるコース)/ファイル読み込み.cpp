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
	std::ifstream ifs;  // ファイル読み取り用ストリーム
	ifs.open("robocon.txt");	// ファイルオープン

	if (ifs.fail()) {	// ファイルオープンに失敗したらそこで終了
		std::cerr << "ファイルを開けません\n";
		exit(1);
	}

	char buf[256];	// データ一時保管用配列

	int linenum = 0; // データの行数を数える

	while (ifs.getline(buf, sizeof(buf))) {	// ファイルから1行ずつ読み込む
		linenum++;	// 行数をカウントしている
	}

	std::cerr << "読み込んだ行数 = " << linenum << "\n";

	ifs.clear(); // ファイル末尾に到達というフラグをクリア
	ifs.seekg(0, std::ios::beg);	// ファイル先頭に戻る

	double *arr0, *arr1;
	datanum = (linenum) / 2;
	arr0 = new double[datanum];	// 行数 linenum 分の配列を動的に確保
	arr1 = new double[datanum];	// 行数 linenum 分の配列を動的に確保

	for (int i = 0; i<datanum; i++) {
		ifs.getline(buf, sizeof(buf));	// 一行読み込んで…
		arr0[i] = atof(buf);	// それを配列に格納
		ifs.getline(buf, sizeof(buf));	// 一行読み込んで…
		arr1[i] = atof(buf);	// それを配列に格納
		//std::cout << i << " = " << arr0[i] << ", "  << arr1[i] << std::endl;
	}	
}
