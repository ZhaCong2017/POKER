#include"Cards.h"
//#include<system>
using namespace std;

CompareCards::CompareCards(vector<card> &hand_cards) :grade(9) 
{  //���캯������5��cards����5�� bitset��
	int i;                                        //����ʼ������������
	for (i = 0; i < hand_cards.size(); i++) {
		switch (hand_cards[i].num) {
		case 'A':
			cards[i].set(4); break;
		case '2':
			cards[i].set(5); break;
		case '3':
			cards[i].set(6); break;
		case '4':
			cards[i].set(7); break;
		case '5':
			cards[i].set(8); break;
		case '6':
			cards[i].set(9); break;
		case '7':
			cards[i].set(10); break;
		case '8':
			cards[i].set(11); break;
		case '9':
			cards[i].set(12); break;
		case 'T':
			cards[i].set(13); break;
		case 'J':
			cards[i].set(14); break;
		case 'Q':
			cards[i].set(15); break;
		case 'K':
			cards[i].set(16); break;
		case 'X':
			cards[i].set(17);
		}
		switch (hand_cards[i].ch) {
		case 'c':
			cards[i].set(0); break;
		case 'd':
			cards[i].set(1); break;
		case 'h':
			cards[i].set(2); break;
		case 's':
			cards[i].set(3);
		}
	}
	bitset<18>cards_temp2 = cards[0] >> 4;
	string s2 = cards_temp2.to_string();
	bitset<14>temp2(s2, 4, 14);
	for (int i = 1; i < hand_cards.size(); i++) {
		cards_temp2 = cards[i] >> 4;
		s2 = cards_temp2.to_string();
		bitset<14>Temp2(s2, 4, 14);
		temp2 = temp2 | Temp2;
	}
	cards_num = temp2;
}

void CompareCards::grade5is(void) 
{           //�ж�5�����������
	bitset<18>cards_temp = (cards[0] << 14) >> 14;
	string s = cards_temp.to_string();
	bitset<4> temp(s, 14, 4);
	bool sqflush;
	sqflush = issequence();   //�����ж��Ƿ�Ϊ˳�ӡ�
	for (int i = 1; i < 5; i++) {
		cards_temp = (cards[i] << 14) >> 14;
		s = cards_temp.to_string();
		bitset<4> Temp(s, 14, 4);
		temp &= Temp;         //temp��5���ƵĻ�ɫ������4λbits��
	}
	if (temp.count() == 1) {  //�ж��Ƿ���ͬ����
		if (sqflush) {      //�ж��Ƿ�˳�ӣ����γ�ͬ��˳��
			if (cards_num[0] == 1 && cards_num[12] == 1)
				grade = 0;
			else
				grade = 1;
		}
		else grade = 4;    //����˳�ӣ�����ֻ����ͨͬ����
	}
	else {
		if (sqflush) {   //����ͬ����ֻ����ͨ˳�ӡ�
			grade = 5;
		}
		else {
			int cnt = cards_num.count();
			if (cnt == 2) {               //��cards_numֻ��2λΪ1�����ǽ�ջ��ߺ�«��
				int i, j, count_1 = 0;
				for (i = 0; i<14; i++) {
					if (cards_num[i] == 1)
						break;
				}
				for (j = 0; j<5; j++) {
					if (cards[j][i + 4] == 1) {
						count_1++;
					}
				}
				//cout<<count_1<<endl;
				if (count_1 == 1 || count_1 == 4) {// �ж�Ϊ��ա�
					grade = 2;
				}
				else grade = 3;        //����Ϊ��«��
			}
			else {
				if (cnt == 3) {      //�� cards_num��3λΪ1���������Ż������ӡ�
					int i_1, i_2, j, count_1 = 0, count_2 = 0;
					for (i_1 = 0; i_1<14; i_1++) {
						if (cards_num[i_1] == 1)break;
					}
					for (i_2 = i_1 + 1; i_2<14; i_2++) {
						if (cards_num[i_2] == 1)break;
					}
					for (j = 0; j<5; j++) {
						if (cards[j][i_1 + 4] == 1)count_1++;
						if (cards[j][i_2 + 4] == 1)count_2++;
					}
					if ((count_1*count_2) == 1 || (count_1*count_2) == 3)grade = 6;//�ж�Ϊ���š�
					else grade = 7;                                         //����Ϊ�����ӡ�
				}
				else {
					if (cnt == 4)grade = 8;   //��cards_num��4λΪ1������һ�����ӡ�
					else grade = 9;        //���cards_num ��5λΪ1�����Ǹ��ơ�
				}
			}
		}
	}
}

int CompareCards::grade3is(void)
{          
	int cnt = cards_num.count();
	int i, j, count = 0;
	if (cnt == 1)
	{
		grade = 6;
		for (i = 13; i >= 1; i--) {
			if (cards_num[i] == 1)
				return i;
		}
	}
	else if (cnt == 2)
	{
		grade = 8;
		for (i = 13; i > 0; i--) {
			if (cards_num[i] == 1)break;
		}
		for (j = i - 1; j >= 0; j--) {
			if (cards_num[j] == 1)break;
		}
		for (int k = 0; k < 3; k++) {
			if (cards[k][i + 4] == 1)count++;
		}
		if (count == 2)
			return i;
		else
			return j;
	}
	else if (cnt == 3)
	{
		grade = 9;
		return 0;
	}
}

bool CompareCards::issequence(void) 
{    //�Ѿ�ȷ��cards_num��5λΪ1������£��ж�5�����Ƿ�˳�ӡ�
	if (cards_num[0] == 1) {
		if (cards_num[4] == 1) {
			for (int i = 3; i>0; i--) {
				if (cards_num[i] == 0)return false;
			}
			return true;
		}
		else {
			for (int i = 12; i > 8; i--) {
				if (cards_num[i] == 0)return false;
			}
			return true;
		}
	}
	else {
		int i;
		for (i = 13; i>3; i--) {
			if (cards_num[i] == 1)
				break;
		}
		if (i == 3)return false;
		for (int j = i - 1; j>i - 5; j--) {
			if (cards_num[j] == 0) {
				return false;
			}
		}
		return true;
	}
}

int CompareCards::compare_5cards(CompareCards &Handcards2)
{ //���е�5���Ƹ������5�����������Ƚϡ�
	//CompareCards Handcards2(cards2);
	//gradeis();
	Handcards2.grade5is();             //���жϴ���������ƣ��öԷ����棩�����
									  // cout<<"Handcards:"<<endl;
									  //cout<<Handcards2.grade<<endl;
									  //cout<<Handcards2.cards_num<<endl;
									  //for(int i=0;i<5;i++){
									  //      cout<<Handcards2.cards[i]<<endl;
									  // }
	if (grade < Handcards2.grade) {   //������ͬ��ֱ�ӱȽϴ�С
		return 1;                     //������
	}
	else {
		if (grade>Handcards2.grade) { //������ͬ��ֱ�ӱȽϴ�С
			return -1;              //�Է���
		}
		else {                      //��˫�������ͬ�������Ƚϡ�
			switch (grade) {
			case 1:           //��������ͬ��˳��
				if (cards_num[0] == 1 && cards_num[12] == 1) { //����Ϊ�ʼ�ͬ��˳
					if (Handcards2.cards_num[0] == 1 && Handcards2.cards_num[12] == 1)return 0;//�Է�Ҳ�ǻʼ�ͬ��˳������ȡ�
					else return 1;//���򼺷���
				}
				else {              //�����ǻʼ�ͬ��˳���ж����������ƣ�
					int i, j;
					for (i = 13; i>3; i--) {
						if (cards_num[i] == 1)break;
					}
					for (j = 13; j>3; j--) {
						if (Handcards2.cards_num[j] == 1)break;
					}
					if (i>j) {     //���������ƱȶԷ�������ƴ��򼺷�ʤ
						return 1;
					}
					else {
						if (i<j)return -1;//�Է������Ʊȼ���������ƴ���Է�ʤ
						else return 0;// ������ȡ�
					}
				}
				break;
			case 2:// �������ǽ�ա�
				int i1, j1, count_4cards1, pos1;//pos1��¼4��ͬ��ֵ�Ƶ�λ��
				count_4cards1 = 0;
				int i2, j2, count_4cards2, pos2;//pos2��¼4��ͬ��ֵ�Ƶ�λ��
				count_4cards2 = 0;
				for (i1 = 13; i1>0; i1--) {
					if (cards_num[i1] == 1)break;
				}
				for (i2 = 13; i2>0; i2--) {
					if (Handcards2.cards_num[i2] == 1)break;
				}
				for (j1 = i1 - 1; j1 >= 0; j1--) {
					if (cards_num[j1] == 1)break;
				}
				for (j2 = i2 - 1; j2 >= 0; j2--) {
					if (Handcards2.cards_num[j2] == 1)break;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][i1 + 4] == 1)count_4cards1++;
					if (Handcards2.cards[i][i2 + 4] == 1)count_4cards2++;
				}
				if (count_4cards1 == 4) {
					pos1 = i1;

				}
				else {
					pos1 = j1;
				}
				if (count_4cards2 == 4) {
					pos2 = i2;

				}
				else {
					pos2 = j2;
				}
				if (pos1 == pos2) { //��4��ͬ��ֵ��һ����
					bitset<14>dif;
					dif = cards_num ^ Handcards2.cards_num;//��˫����cards_num���ȡ����ֵ��ͬ��λ�á�
					if (dif.none())return 0;//��difȫΪ0����˫����ͬ��
					else {
						if (dif[0] == 1) {    //�ж�A����λ��Ϊ1����˫��A���ڵ�λ����һ��Ϊ1����һ��ʤ����
							if (cards_num[0] == 1)return 1;
							else return -1;
						}
						else {            //��˫��A����λ�ö�Ϊ0�����ж�����Ƶ���ֵ��
							int r;
							for (r = 13; r>0; r--) {
								if (dif[r] == 1)
									break;
							}
							if (cards_num[r] == 1)return 1;//�������ڼ���������ʤ����
							else return -1;             // ���򼺷��ܡ�
						} 
					}
				}
				else {                                 //��pos1!=pos2,��ֱ�ӱȽϡ�
					if (pos1 == 0)return 1;
					if (pos2 == 0)return -1;
					if (pos1>pos2)return 1;
					else return -1;
				}
				break;
			case 3:  // ˫����Ϊ��«��
				int count_3cards1;
				int count_3cards2;
				count_3cards1 = 0;
				count_3cards2 = 0;
				for (i1 = 13; i1>0; i1--) {
					if (cards_num[i1] == 1)break;
				}
				for (i2 = 13; i2>0; i2--) {
					if (Handcards2.cards_num[i2] == 1)break;
				}
				for (j1 = i1 - 1; j1 >= 0; j1--) {
					if (cards_num[j1] == 1)break;
				}
				for (j2 = i2 - 1; j2 >= 0; j2--) {
					if (Handcards2.cards_num[j2] == 1)break;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][i1 + 4] == 1)count_3cards1++;
					if (Handcards2.cards[i][i2 + 4] == 1)count_3cards2++;
				}
				if (count_3cards1 == 3)pos1 = i1;
				else pos1 = j1;           //pos1,pos2��¼3��ͬ�Ƶ�λ�á�
				if (count_3cards2 == 3)pos2 = i2;
				else pos2 = j2;
				if (pos1 == pos2) {
					bitset<14>dif;
					dif = cards_num ^ Handcards2.cards_num;
					if (dif.none())return 0;
					else {
						if (dif[0] == 1) {
							if (cards_num[0] == 1)return 1;
							else return -1;
						}
						else {
							int r;
							for (r = 13; r>0; r--) {
								if (dif[r] == 1)
									break;
							}
							if (cards_num[r] == 1)return 1;
							else return -1;
						}
					}
				}
				else {
					if (pos1 == 0)return 1;
					if (pos2 == 0)return -1;
					if (pos1>pos2)return 1;
					else return -1;
				}
				break;
			case 4:                 //������ͨͬ�����߶��Ǹ��Ƶ�����£������Ƚ�ÿ����
			case 9:
				return compare_cards_num(Handcards2);
				break;
			case 5:                //������ͨ˳�ӡ�
				if (cards_num[0] == 1 && cards_num[12] == 1) {
					if (Handcards2.cards_num[0] == 1 && Handcards2.cards_num[12] == 1)return 0;
					else return 1;
				}
				else {
					int i, j;
					for (i = 13; i>3; i--) {
						if (cards_num[i] == 1)break;
					}
					for (j = 13; j>3; j--) {
						if (Handcards2.cards_num[j] == 1)break;
					}
					if (i>j) {
						return 1;
					}
					else {
						if (i<j)return -1;
						else return 0;
					}
				}
				break;
			case 6:        //�������š�
				int k1, count_3cards1_1, count_3cards1_2;
				int k2, count_3cards2_1, count_3cards2_2;
				count_3cards1_1 = 0; count_3cards1_2 = 0;
				count_3cards2_1 = 0; count_3cards2_2 = 0;
				for (i1 = 13; i1>1; i1--) {
					if (cards_num[i1] == 1)break;
				}
				for (i2 = 13; i2>1; i2--) {
					if (Handcards2.cards_num[i2] == 1)break;
				}
				for (j1 = i1 - 1; j1>0; j1--) {
					if (cards_num[j1] == 1)break;
				}
				for (j2 = i2 - 1; j2>0; j2--) {
					if (Handcards2.cards_num[j2] == 1)break;
				}
				for (k1 = j1 - 1; k1 >= 0; k1--) {
					if (cards_num[k1] == 1)break;
				}
				for (k2 = j2 - 1; k2 >= 0; k2--) {
					if (Handcards2.cards_num[k2] == 1)break;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][i1 + 4] == 1)count_3cards1_1++;
					if (Handcards2.cards[i][i2 + 4] == 1)count_3cards2_1++;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][j1 + 4] == 1)count_3cards1_2++;
					if (Handcards2.cards[i][j2 + 4] == 1)count_3cards2_2++;
				}
				if (count_3cards1_1 == 3)pos1 = i1;
				else {
					if (count_3cards1_2 == 3)pos1 = j1;
					else pos1 = k1;
				}
				if (count_3cards2_1 == 3)pos2 = i2;
				else {
					if (count_3cards2_2 == 3)pos2 = j2;
					else pos2 = k2;
				}
				if (pos1 == pos2) {
					bitset<14>dif;
					dif = cards_num ^ Handcards2.cards_num;
					if (dif.none())return 0;
					else {
						if (dif[0] == 1) {
							if (cards_num[0] == 1)return 1;
							else return -1;
						}
						else {
							int r;
							for (r = 13; r>0; r--) {
								if (dif[r] == 1)
									break;
							}
							if (cards_num[r] == 1)return 1;
							else return -1;
						}
					}
				}
				else {
					if (pos1 == 0)return 1;
					if (pos2 == 0)return -1;
					if (pos1>pos2)return 1;
					else return -1;
				}
				break;
			case 7:  //�����������ӡ�
				int count_2cards1_1, count_2cards1_2, pos1_1, pos1_2, pos1_3;
				int count_2cards2_1, count_2cards2_2, pos2_1, pos2_2, pos2_3;
				count_2cards1_1 = 0; count_2cards1_2 = 0;
				count_2cards2_1 = 0; count_2cards2_2 = 0;
				for (i1 = 13; i1>1; i1--) {
					if (cards_num[i1] == 1)break;
				}
				for (i2 = 13; i2>1; i2--) {
					if (Handcards2.cards_num[i2] == 1)break;
				}
				for (j1 = i1 - 1; j1>0; j1--) {
					if (cards_num[j1] == 1)break;
				}
				for (j2 = i2 - 1; j2>0; j2--) {
					if (Handcards2.cards_num[j2] == 1)break;
				}
				for (k1 = j1 - 1; k1 >= 0; k1--) {
					if (cards_num[k1] == 1)break;
				}
				for (k2 = j2 - 1; k2 >= 0; k2--) {
					if (Handcards2.cards_num[k2] == 1)break;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][i1 + 4] == 1)count_2cards1_1++;
					if (Handcards2.cards[i][i2 + 4] == 1)count_2cards2_1++;
				}
				for (int i = 0; i<5; i++) {
					if (cards[i][j1 + 4] == 1)count_2cards1_2++;
					if (Handcards2.cards[i][j2 + 4] == 1)count_2cards2_2++;
				}
				if (count_2cards1_1 == 2) {
					pos1_1 = i1;
					if (count_2cards1_2 == 2) {
						pos1_2 = j1;
						pos1_3 = k1;
					}
					else {
						pos1_2 = k1;
						pos1_3 = j1;
					}
				}
				else {
					pos1_1 = j1;
					pos1_2 = k1;
					pos1_3 = i1;
				}
				if (count_2cards2_1 == 2) {
					pos2_1 = i2;
					if (count_2cards2_2 == 2) {
						pos2_2 = j2;
						pos2_3 = k2;
					}
					else {
						pos2_2 = k2;
						pos2_3 = j2;
					}
				}
				else {
					pos2_1 = j2;
					pos2_2 = k2;
					pos2_3 = i2;
				}
				/*if(pos1_1>pos2_1)return 1;
				else {
				if(pos1_1<pos2_1)return -1;
				else{
				if(pos1_2>pos2_2)return 1;
				else{
				if(pos1_2<pos2_2)return -1;
				else{
				if(pos1_3>pos2_3)return 1;
				else {
				if(pos1_3<pos2_3)return -1;
				else return 0;
				}
				}
				}
				}
				}*/
				if (pos1_2 == pos2_2) {
					if (pos1_1>pos2_1)return 1;
					else {
						if (pos1_1<pos2_1)return -1;
						else {
							if (pos1_3 == pos2_3)return 0;
							else {
								if (pos1_3 == 0)return 1;
								if (pos2_3 == 0)return -1;
								if (pos1_3>pos2_3)return 1;
								else return -1;
							}
						}
					}
				}
				else {
					if (pos1_2 == 0)return 1;
					if (pos2_2 == 0)return -1;
					if (pos1_1 > pos2_1)return 1;
					if (pos1_1 < pos2_1)return -1;
					if (pos1_2 > pos2_2)return 1;
					else return -1;
				}
				break;
			case 8:     //����ֻ��һ�����ӡ�
				int pos_1[5];
				int pos_2[5];
				int i, j, k;
				int whr1, whr2;
				for (i = 0; i<5; i++) {
					for (j = 17; j >= 4; j--) {
						if (cards[i][j] == 1) {
							pos_1[i] = j;
							//cout<<"pos_1["<<i<<"]="<<pos_1[i]<<endl;
							break;
						}
					}
					for (k = 17; k >= 4; k--) {
						if (Handcards2.cards[i][k] == 1) {
							pos_2[i] = k;
							//cout<<"pos_2["<<i<<"]="<<pos_2[i]<<endl;
							break;
						}
					}
				}
				for (i = 0; i<4; i++) {
					int tag = 0;
					for (j = i + 1; j<5; j++) {
						if (pos_1[i] == pos_1[j]) {
							whr1 = pos_1[i];
							tag = 1;
							break;
						}
					}
					if (tag == 1)break;
				}
				for (i = 0; i<4; i++) {
					int tag = 0;
					for (j = i + 1; j<5; j++) {
						if (pos_2[i] == pos_2[j]) {
							whr2 = pos_2[i];
							tag = 1;
							break;
						}
					}
					if (tag == 1)break;
				}
				//cout<<"whr1="<<whr1<<"whr2="<<whr2<<endl;
				if (whr1 == whr2) {
					bitset<14>dif;
					dif = cards_num ^ Handcards2.cards_num;
					if (dif.none())return 0;
					if (dif[0] == 1) {
						if (cards_num[0] == 1)return 1;
						else return -1;
					}
					else {
						int r;
						for (r = 13; r>0; r--) {
							if (dif[r] == 1)break;
						}
						if (cards_num[r] == 1)return 1;
						else return -1;
					}
				}
				else {
					if (whr1 == 4 || whr2 == 4) {
						if (whr1 == 4)return 1;
						else return -1;
					}
					if (whr1>whr2)return 1;
					else return -1;
				}
				break;
			}
		}
	}
}

int CompareCards::compare_3cards(CompareCards &Handcards2)
{ 
	//CompareCards Handcards2(cards2);
	//gradeis();
	Handcards2.grade3is();             //���жϴ���������ƣ��öԷ����棩�����
									  
	if (grade < Handcards2.grade) {   //������ͬ��ֱ�ӱȽϴ�С
		return 1;                     //������
	}
	else {
		if (grade > Handcards2.grade) { //������ͬ��ֱ�ӱȽϴ�С
			return -1;              //�Է���
		}
		else {                      //��˫�������ͬ�������Ƚϡ�
			switch (grade) {	
			case 6:        //�������š�
				int i, j;
				for (i = 13; i>= 1; i--) {
					//cout << cards_num[i] << ' ' << i;
					if (cards_num[i] == 1)
						break;
				}
				//cout << endl;
				for (j = 13; j >= 1; j--) {
					//cout << Handcards2.cards_num[j] << ' ' << j;
					if (Handcards2.cards_num[j] == 1)
						break;
				}
				/*cout << endl;*/
				//cout << i << " " << j << endl;
				if (i == 0 && j == 0)
					return 0;
				else if (i == 0)     //���������ƱȶԷ�������ƴ��򼺷�ʤ
					return 1;
				else if (j == 0)
					return -1;//�Է������Ʊȼ���������ƴ���Է�ʤ
				else if(i > j)
					return 1;
				else if(i < j)
					return -1;
				else 
					return 0;// ������ȡ�
			case 8:     //����ֻ��һ�����ӡ�
				int i1, j1, count_2cards1, pos1;//pos1��¼2��ͬ��ֵ�Ƶ�λ��
				count_2cards1 = 0;
				int i2, j2, count_2cards2, pos2;//pos2��¼2��ͬ��ֵ�Ƶ�λ��
				count_2cards2 = 0;
				for (i1 = 13; i1 > 0; i1--) {
					if (cards_num[i1] == 1)break;
				}
				for (i2 = 13; i2 > 0; i2--) {
					if (Handcards2.cards_num[i2] == 1)break;
				}
				for (j1 = i1 - 1; j1 >= 0; j1--) {
					if (cards_num[j1] == 1)break;
				}
				for (j2 = i2 - 1; j2 >= 0; j2--) {
					if (Handcards2.cards_num[j2] == 1)break;
				}
				for (int i = 0; i < 3; i++) {
					if (cards[i][i1 + 4] == 1)count_2cards1++;
					if (Handcards2.cards[i][i2 + 4] == 1)count_2cards2++;
				}
				if (count_2cards1 == 2)
					pos1 = i1;
				else 
					pos1 = j1;
				
				if (count_2cards2 == 2) 
					pos2 = i2;
				else 
					pos2 = j2;
				
				if (pos1 == pos2) { //��2��ͬ��ֵ��һ����
					bitset<14>dif;
					dif = cards_num ^ Handcards2.cards_num;//��˫����cards_num���ȡ����ֵ��ͬ��λ�á�
					if (dif.none())return 0;//��difȫΪ0����˫����ͬ��
					else {
						if (dif[0] == 1) {    //�ж�A����λ��Ϊ1����˫��A���ڵ�λ����һ��Ϊ1����һ��ʤ����
							if (cards_num[0] == 1)return 1;
							else return -1;
						}
						else {            //��˫��A����λ�ö�Ϊ0�����ж�����Ƶ���ֵ��
							int r;
							for (r = 13; r > 0; r--) {
								if (dif[r] == 1)
									break;
							}
							if (cards_num[r] == 1)return 1;//�������ڼ���������ʤ����
							else return -1;             // ���򼺷��ܡ�
						}
					}
				}
				else {                                 //��pos1!=pos2,��ֱ�ӱȽϡ�
					if (pos1 == 0) return 1;
					if (pos2 == 0) return -1;
					if (pos1 > pos2) return 1;
					else return -1;
				}
				break;
			case 9:
				return compare_cards_num(Handcards2);
				break;
			}
		}
	}
}

int CompareCards::compare_35cards(int num, CompareCards &Handcards2)
{
	//CompareCards Handcards2(cards2);
	//gradeis();
	//Handcards2.grade3is();             //���жϴ���������ƣ��öԷ����棩�����

	if (grade < Handcards2.grade) {   //������ͬ��ֱ�ӱȽϴ�С
		return 1;                     //������
	}
	else if (grade > Handcards2.grade) { //������ͬ��ֱ�ӱȽϴ�С
			return -1;              //�Է���
		}
	else {                      //��˫�������ͬ�������Ƚϡ�
		switch (grade) {
		case 6:        //�������š�
			int j;
			for (j = 13; j >= 1; j--) {
				if (Handcards2.cards_num[j] == 1)break;
			}
			if (num == 0 && j == 0)
				return 0 ;
			else if (num > j || num == 0)     //���������ƱȶԷ�������ƴ��򼺷�ʤ
				return 1;
			//else if (num < j || j == 1)
			//	return -1;//�Է������Ʊȼ���������ƴ���Է�ʤ
			else
				return 0;// ������ȡ�
		case 8:     //����ֻ��һ�����ӡ�
			int pos1, pos2;
			int i2, j2, count_2cards2, pos_2[5];//pos2��¼2��ͬ��ֵ�Ƶ�λ��
			pos1 = num;

			for (int i = 0; i < 5; i++) {
				for (int k = 17; k >= 4; k--) {
					if (Handcards2.cards[i][k] == 1) {
						pos_2[i] = k - 4;
						//cout<<"pos_2["<<i<<"]="<<pos_2[i]<<endl;
						break;
					}
				}
			}
			int tag;
			for (int i = 0; i < 4; i++) {
				tag = 0;
				for (int j = i + 1; j < 5; j++) {
					if (pos_2[i] == pos_2[j]) {
						pos2 = pos_2[i];
						tag = 1;
						break;
					}
				}
				if (tag == 1)break;
			}
			//cout << pos1 << " " << pos2 << endl;
			if (pos1 == pos2) { //��2��ͬ��ֵ��һ����
				
				/*if (cards_num[0] == 1 && Handcards2.cards_num[0] == 0)
					return 1;
				else if (cards_num[0] == 0 && Handcards2.cards_num[0] == 1)
					return -1;

				for (int r = 13; r >= 0; r--)
				{
					if (cards_num[r] == 1 && Handcards2.cards_num[r] == 0)
						return 1;
					else if (cards_num[r] == 0 && Handcards2.cards_num[r] == 1)
						return -1;
				}*/
				bitset<14>dif;
				dif = cards_num ^ Handcards2.cards_num;//��˫����cards_num���ȡ����ֵ��ͬ��λ�á�
				if (dif[0] == 1) {    //�ж�A����λ��Ϊ1����˫��A���ڵ�λ����һ��Ϊ1����һ��ʤ����
					if (cards_num[0] == 1)return 1;
					else return -1;
				}
				else {            //��˫��A����λ�ö�Ϊ0�����ж�����Ƶ���ֵ��
					int r;
					for (r = 13; r > 0; r--) {
						if (dif[r] == 1)
							break;
					}
					if (cards_num[r] == 1)return 1;//�������ڼ���������ʤ����
					else return -1;             // ���򼺷��ܡ�
				}
				
			}
			else {                                 //��pos1!=pos2,��ֱ�ӱȽϡ�
				if (pos1 == 0) return 1;
				if (pos2 == 0) return -1;
				if (pos1 > pos2) return 1;
				else return -1;
			}
			break;
		case 9:
			return compare_cards_num(Handcards2);
			break;
		}
	}
}

int CompareCards::compare_cards_num(CompareCards &Handcards) 
{ //����ͨͬ�����߶��Ǹ��Ƶ�����±Ƚϴ�С
	bitset<14>dif;
	dif = cards_num ^ Handcards.cards_num; //��˫����cards_num���ȡ�����еĲ�ͬλ��
	if (dif.none())return 0;   //��û�в�ͬλ����˫���Եȡ�
	else {
		if (dif[0] == 1) {
			if (cards_num[0] == 1)return 1;
			else return -1;
		}
		else {
			int r;
			for (r = 13; r>0; r--) {
				if (dif[r] == 1)
					break;
			}
			if (cards_num[r] == 1)return 1;
			else return -1;
		}
	}
}

//class compare_7cards {      //7���������Ƚϡ�
//public:
//	int compare_7(vector<card> &cards1, vector<card>&cards2) {
//		int i, j;
//		vector<card>max1, max2;
//		max1 = max_7_to_5(cards1); //��7������ѡ����������5���ơ�
//		max2 = max_7_to_5(cards2);
//		Compare_5_cards Handcards1(max1);
//		Handcards1.gradeis();
//		return Handcards1.compare_5cards(max2);//��ѡ���������������������ƽ��бȽϡ�
//	}
//	vector<card> max_7_to_5(vector<card>&cards) { //7ѡ5.
//		int i, j, k;
//		vector<card>max;
//		for (k = 2; k<7; k++) {
//			max.push_back(cards[k]);
//		}
//		for (i = 0; i<6; i++) {               //ѭ��21�Σ�����compare_5�ȽϺ���21��ѡ����������5�š�
//			for (j = i + 1; j<7; j++) {
//				if (i == 0 && j == 1)continue;
//				Compare_5_cards Hcd(max);
//				Hcd.gradeis();
//				vector<card>temp;
//				for (k = 0; k<7; k++) {
//					if (k != i && k != j) {
//						temp.push_back(cards[k]);
//					}
//				}
//				if (Hcd.compare_5cards(temp) == -1) {
//					max = temp;
//				}
//			}
//		}
//		return max;
//	}
//};
/*
int main()
{
vector<card> cards_7;
vector<card> cards_7_2;
compare_7cards cpr;
cout << "Please input 14 cards." << endl;
cout << "First input the first 7 cards, first nums(1-14,1:A,14:K) ,and suit(c,d,h,s)" << endl;
for (int i = 1; i <= 7; i++) {
card cards;
cin >> cards.num >> cards.ch;
cards_7.push_back(cards);
}
cout << "Second input the second 7 cards, first nums(1-14,1:A,14:K) ,and suit(c,d,h,s)" << endl;
for (int i = 1; i <= 7; i++) {
card cards;
cin >> cards.num >> cards.ch;
cards_7_2.push_back(cards);
}
int flag = cpr.compare_7(cards_7, cards_7_2);
if (flag == 1)cout << "The first 7_cards is higher than the scond one" << endl;
else {
if (flag == -1)cout << "The second 7_cards is higher" << endl;
else cout << "They are equal" << endl;
}
system("pause");
return 0;
}
*/