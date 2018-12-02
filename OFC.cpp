#include "OFC.h"
#include<omp.h>
#include<ctime>
//#include<fstream>


OFC::OFC()
{
	simu_score = 0;
	card_num = 0;
	simu_num = 650; // 3000;
}


OFC::~OFC()
{
}

bool OFC::operator < (const OFC &b) const
{
	return simu_score > b.simu_score;    //类中，x小的优先级高     
}

void OFC::combine(vector<string> &data, int n, int m, vector<int> &temp, const int M, vector<vector<string> > &vec_res)
{
	for (int i = n; i >= m; i--)   // 注意这里的循环范围
	{
		temp[m - 1] = i - 1;
		if (m > 1)
			combine(data, i - 1, m - 1, temp, M, vec_res);
		else                     // m == 1, 输出一个组合
		{
			vector<string> vec_temp;
			for (int j = M - 1; j >= 0; j--) {
				vec_temp.push_back(data[temp[j]]);
			}
			vec_res.push_back(vec_temp);
		}
	}
}

void OFC::scoreis(Score &s)
{
	/*float result = 0;
	vector<card> _up(3), _mid(5), _down(5);
	for (int i = 0; i < 3; i++)
	{
		_up[i].num = up[i][0];
		_up[i].ch = up[i][1];
	}

	for (int i = 0; i < 5; i++)
	{
		_mid[i].num = mid[i][0];
		_mid[i].ch = mid[i][1]; 
		_down[i].num = down[i][0];
		_down[i].ch = down[i][1];
	}
	return s.three_grade(_up, _mid, _down);*/
	float tmp_sum = 0;
#pragma omp parallel for reduction (+: tmp_sum)
	for (int i = 1; i <= simu_num; i++)
	{
		//cout << omp_get_thread_num() << endl;
		srand((unsigned)(time(NULL) * omp_get_thread_num() + clock()));
		float tmp_score = simulation(s, poker_can_use);
		tmp_sum += tmp_score;
		/*if (i % 100 == 0)
			cout << i << ":  " << tmp_sum / i << endl;*/
	}
	simu_score = tmp_sum / simu_num;
}

float OFC::simulation(Score &s, vector<string> poker_now)
{
	vector<string> cards(13 - card_num);
	//vector<string> poker_now = poker_can_use;
	//card_num = up.size() + mid.size() + down.size();
	for (int i = 0; i < 13 - card_num; i++)
	{
		//int tmp = rand() % poker_now.size();
		cards[i] = poker_now[rand() % poker_now.size()];
		poker_now.erase(remove(poker_now.begin(), poker_now.end(), cards[i]));
	}

	//cout << omp_get_thread_num() << "   ";
	//for (int i = 0; i < cards.size(); i++)
	//{
	//	//int tmp = rand() % poker_now.size();
	//	cout << cards[i] << " ";
	//}
	//cout << endl;
	//vector<string> _up = up, _mid = mid, _down = down;
	vector<card> a(3), b(5), c(5);
#pragma omp critical
	{
		for (int i = 0; i < up.size(); i++)
		{
			a[i].num = up[i][0];
			a[i].ch = up[i][1];
		}
		for (int i = 0; i < mid.size(); i++)
		{
			b[i].num = mid[i][0];
			b[i].ch = mid[i][1];
		}
		for (int i = 0; i < down.size(); i++)
		{
			c[i].num = down[i][0];
			c[i].ch = down[i][1];
		}
	}

	float score = -100, now_score;
	vector<int> t(5);
	vector<vector<string> > res_up(1);
	if (up.size() < 3)
	{
		res_up.clear();
		combine(cards, cards.size(), 3 - up.size(), t, 3 - up.size(), res_up);
	}

	for(int i = 0; i < res_up.size(); i++)
	{
		//vector<string> _up = up, _mid = mid, _down = down;
		vector<string> cards_nouse = cards;
		for (int j = 0; j < res_up[i].size(); j++)
			cards_nouse.erase(remove(cards_nouse.begin(), cards_nouse.end(), res_up[i][j]));
		vector<vector<string> > res_mid(1);
		if (mid.size() < 5)
		{
			res_mid.clear();
			combine(cards_nouse, cards_nouse.size(), 5 - mid.size(), t, 5 - mid.size(), res_mid);
		}
		for (int j = 0; j < res_mid.size(); j++)
		{
			vector<string> card_down = cards_nouse;
			int k, pos;
			for (k = 0; k < res_mid[j].size(); k++)
				card_down.erase(remove(card_down.begin(), card_down.end(), res_mid[j][k]));
			//for (k = 0; k < res_up[i].size(); k++)
			//	_up.push_back(res_up[i][k]);
			//for (k = 0; k < res_mid[j].size(); k++)
			//	_mid.push_back(res_mid[j][k]);
			//for (k = 0; k < card_down.size(); k++)
			//	_down.push_back(card_down[k]);
			for (k = up.size(), pos = 0; k < 3; k++, pos++)
			{
				a[k].num = res_up[i][pos][0];
				a[k].ch = res_up[i][pos][1];
			}
			for (k = mid.size(), pos = 0; k < 5; k++, pos++)
			{
				b[k].num = res_mid[j][pos][0];
				b[k].ch = res_mid[j][pos][1];
			}
			for (k = down.size(), pos = 0; k < 5; k++, pos++)
			{
				c[k].num = card_down[pos][0];
				c[k].ch = card_down[pos][1];
			}

			/*for (int k = 0; k < res_up[i].size(); k++)
				cout << res_up[i][k] << " ";
			cout << endl;
			for (int k = 0; k < res_mid[j].size(); k++)
				cout << res_mid[j][k] << " ";
			cout << endl;
			for (int k = 0; k < card_down.size(); k++)
				cout << card_down[k] << " ";
			cout << endl << endl;*/

			
			now_score = s.three_grade(a, b, c);
			//cout << now_score << endl;
			if (now_score > 300)
			{
				/*for (int k = 0; k < 3; k++)
					cout << a[k].num << a[k].ch << " ";
				cout << endl;
				for (int k = 0; k < 5; k++)
					cout << b[k].num << b[k].ch << " ";
				cout << endl;
				for (int k = 0; k < 5; k++)
					cout << c[k].num << c[k].ch << " ";
				cout << endl << endl;*/
				now_score = 0;			 
			}
			score = score > now_score ? score : now_score;
		}
	}
	return score;

}

void OFC::print()
{
	for (int i = 0; i < up.size(); i++)
		cout << up[i] << " ";
	for (int i = up.size(); i < 3; i++)
		cout << "- ";
	cout << endl;

	for (int i = 0; i < mid.size(); i++)
		cout << mid[i] << " ";
	for (int i = mid.size(); i < 5; i++)
		cout << "- ";
	cout << endl;

	for (int i = 0; i < down.size(); i++)
		cout << down[i] << " ";
	for (int i = down.size(); i < 5; i++)
		cout << "- ";
	cout << endl;
}
