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
};//ÿ��������ֵ����ɫ��������

class CompareCards
{
public:
	bitset<18> cards[5];            // 5������5���18��bitλ����ʾ�ұ�4λ��ʾ��ɫ�����14λ��ʾ��ֵ��
	bitset<14> cards_num; //cards_num��ʾ5������ֵ�����14λbits�� //��1����14������A��2,3��...,K, 1����A��13����K.
	int grade;                      //grade��ʾ��������������һ�����1����9�ֱ����ͬ��˳����գ�...��
	CompareCards(vector<card> &hand_cards); //���캯������5��cards����5�� bitset��
	void grade5is();
	int grade3is();
	bool issequence();
	int  compare_5cards(CompareCards &Handcards2);
	int compare_3cards(CompareCards &Handcards2);
	int compare_35cards(int num, CompareCards &Handcards2);
	int compare_cards_num(CompareCards &Handcards);
};