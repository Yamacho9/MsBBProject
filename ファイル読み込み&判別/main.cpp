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
int linenum = 0; // データの行数を数える

int main()
{
	std::cerr << "main start" << std::endl;
	reading();
	JudgingOnLine(1500,20);

	free(arr0);
	free(arr1);
	return 0;
}

int i = 0;
//int *arr0, *arr1;

void reading() {

	std::cerr << "reading" << std::endl;

	std::ifstream ifs;  // ファイル読み取り用ストリーム
	ifs.open("robocon.txt");	// ファイルオープン

	if (ifs.fail()) {	// ファイルオープンに失敗したらそこで終了
		std::cerr << "ファイルを開けません\n";
		exit(1);
	}

	char buf[256];	// データ一時保管用配列

	while (ifs.getline(buf, sizeof(buf))) {	// ファイルから1行ずつ読み込む
		linenum++;	// 行数をカウントしている
	}

	//std::cerr << "読み込んだ行数 = " << linenum << "\n";

	ifs.clear(); // ファイル末尾に到達というフラグをクリア
	ifs.seekg(0, std::ios::beg);	// ファイル先頭に戻る

	arr0 = (int *)malloc(linenum * sizeof(int));
	arr1 = (int *)malloc(linenum * sizeof(int));

	for (i = 0; i<linenum; i++) {
		ifs.getline(buf, sizeof(buf));	// 一行読込
		split(buf, ",");
		//std::cout << i << " = " << arr0[i] << ", " << arr1[i] << std::endl;
	}

	return;
}

void split(char* s, const std::string& delim)
{
	using namespace std;
	int count = 0;
	char *tok;

	std::cerr << "split" << std::endl;

	tok = strtok(s, delim.c_str());
	while (tok != NULL) {
		if ((count % 2) == 0) {
			arr0[i] = atoi(tok);
		}
		else {
			arr1[i] = atoi(tok);
		}
		tok = strtok(NULL, delim.c_str());  /* 2回目以降 */
		count++;
	}
	//printf("i=%d, count=%d, arr0=%d, arr1=%d\n", i, count, arr0[i], arr1[i]);
}

uint32_t flag;
bool JudgingOnLine(int distance, int degree) {
	bool result = true; //on line
	int degMax=0, degMin=0;
	flag = linenum;

	printf("judge\n");

	//distance range check
	while (1) {
		if ((arr0[linenum - (flag - 1)] + DistMargine) <= distance) { //次の区間へ
			flag--;
			//printf("flag=%d, linenum=%d, arr0[]=%d\n", flag, linenum, arr0[linenum - (flag - 1)]);
		}
		else {
			//printf("else %d\n", (flag - 1));
			break;
		}
	}
	
	if (arr1[linenum - flag] <= arr0[linenum - (flag - 1)]) {
		degMax = arr0[linenum - (flag - 1)];
		degMin = arr1[linenum - flag];
	}else {
		degMax = arr1[linenum - flag];
		degMin =  arr0[linenum - (flag - 1)];
	}

	if ((degMin - DistMargine) <= degree && distance<=(degMax + DistMargine)) {
		result = true;
	}else {
		result = false;
	}

	return result;
}
