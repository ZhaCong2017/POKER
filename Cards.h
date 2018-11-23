#include <iostream>
#include<bitset>
#include<vector>
#include<string>
//#include<system>
using namespace std;

struct card
{
	char num;
	char ch;
};//每张牌用面值跟花色来描述。

class CompareCards
{
public:
	bitset<18> cards[5];            // 5张牌用5组的18个bit位来表示右边4位表示花色，左边14位表示面值。
	bitset<14> cards_num; //cards_num表示5张牌面值相或后的14位bits。 //用1——14来代表A，2,3，...,K, 1代表A，13代表K.
	int grade;                      //grade表示这五张牌属于哪一种类别，1——9分别代表同花顺，金刚，...。
	CompareCards(vector<card> &hand_cards); //构造函数，将5张cards生成5组 bitset。
	void grade5is();
	int grade3is();
	bool issequence();
	int  compare_5cards(CompareCards &Handcards2);
	int compare_3cards(CompareCards &Handcards2);
	int compare_35cards(int num, CompareCards &Handcards2);
	int compare_cards_num(CompareCards &Handcards);
};