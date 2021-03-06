#include<iostream>
//#include<omp.h>
#include<fstream>
#include<vector>
#include<string>
#include<set>
#include<queue>
#include<map>
#include<algorithm>
#include"OFC.h"
using namespace std;

//int total = 0;
Score s;

struct First5_color_select {
	char color;
	int number; //出现次数
	int value; //最大值
};

inline void swap(string &a, string &b)
{
	string tmp = a;
	a = b;
	b = tmp;
}

void combine(vector<string> &data, int n, int m, vector<int> &temp, const int M, vector<vector<string> > &vec_res)
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

OFC combine_mid(vector<string> &up, vector<string> &poker, vector<string> &poker_now, int king)
{
	int m = poker.size();
	vector<int> t(5);
	vector<vector<string> > res(1);
	for (int i = 1; i <= m; i++)
		combine(poker, m, i, t, i, res);

	OFC best_ofc;
	for (int i = 0; i < res.size(); i++)
	{
		OFC tmp;
		tmp.up = up;
		tmp.mid = res[i];
		vector<string> dd = poker;
		for (int j = 0; j < res[i].size(); j++)
			dd.erase(remove(dd.begin(), dd.end(), res[i][j]));
		tmp.down = dd;
		tmp.poker_can_use = poker_now;
		tmp.card_num = tmp.up.size() + tmp.mid.size() + tmp.down.size();

		/*tmp.up = { "Ac" };
		tmp.mid = {"6c"};
		tmp.down = { "Jc", "Kc", "Qc"};*/

		tmp.scoreis(s, king);
		tmp.print();
		cout << tmp.simu_score << endl << endl;

		if (tmp.simu_score > best_ofc.simu_score)
			best_ofc = tmp;
		//system("pause");
		//total++;
	}
	return best_ofc;
}

void find_best5(vector<string> &s, vector<string> &poker)
{
	priority_queue<OFC, vector<OFC> > best_5_ofc;
	vector<int> tmps(5);
	OFC best_ofc;
	vector<vector<string> > res(1);
	combine(s, 5, 1, tmps, 1, res);
	combine(s, 5, 2, tmps, 2, res);
	combine(s, 5, 3, tmps, 3, res);

	vector<string> poker_now = poker;
	for (int j = 0; j < s.size(); j++)
		poker_now.erase(remove(poker_now.begin(), poker_now.end(), s[j]));

	/*for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			cout << res[i][j] << " ";
		}
		cout << endl;
	}*/
	for (int i = 0; i < res.size(); i++)
	{
		vector<string> cards_nouse = s;
		for (int j = 0; j < res[i].size(); j++)
			cards_nouse.erase(remove(cards_nouse.begin(), cards_nouse.end(), res[i][j]));
		OFC tmp_ofc = combine_mid(res[i], cards_nouse, poker_now, 0);
		if (best_5_ofc.size() < 5)
			best_5_ofc.push(tmp_ofc);
		else if (tmp_ofc.simu_score > best_5_ofc.top().simu_score)
		{
			best_5_ofc.pop();
			best_5_ofc.push(tmp_ofc);
		}
	}

	//cout << total << endl;
	//system("pause");
	ofstream fout("result.txt", ios::app);
	for (int i = 0; i < 5; i++)
		fout << s[i] << " ";
	fout << endl;
	while (!best_5_ofc.empty())
	{
		best_ofc = best_5_ofc.top();
		best_5_ofc.pop();
		for (int i = 0; i < best_ofc.up.size(); i++)
			fout << best_ofc.up[i] << " ";
		for (int i = best_ofc.up.size(); i < 3; i++)
			fout << "- ";

		for (int i = 0; i < best_ofc.mid.size(); i++)
			fout << best_ofc.mid[i] << " ";
		for (int i = best_ofc.mid.size(); i < 5; i++)
			fout << "- ";

		for (int i = 0; i < best_ofc.down.size(); i++)
			fout << best_ofc.down[i] << " ";
		for (int i = best_ofc.down.size(); i < 5; i++)
			fout << "- ";
		fout << best_ofc.simu_score << endl;
	}
	fout << endl;
	fout.close();
}

void remove_signal(vector<string> &a)
{
	int stop = a.size();
	for (int i = 0; i < stop; i++)
	{
		if (a[i][0] == '-')
		{
			for (int j = i; j < stop; j++)
				a.pop_back();
			break;
		}
	}
}

bool cmp(First5_color_select a, First5_color_select b)
{
	if (a.number > b.number)
		return true;
	else if (a.number == b.number  && a.value > b.value)
		return true;
	else
		return false;
}

priority_queue<OFC> get_aim_7(OFC &now, vector<vector<string> > &poker_offer_type, int king)
{
	priority_queue<OFC, vector<OFC> > best_5;
	for (int i = 0; i < poker_offer_type.size(); i++)
	{
		//cout << i << endl;
		if (now.up.size() + 2 <= 3 && (poker_offer_type.size() < 8 || poker_offer_type[i][0][1] == poker_offer_type[i][1][1]))
		{
			now.up.push_back(poker_offer_type[i][0]);
			now.up.push_back(poker_offer_type[i][1]);
			now.scoreis(s, king);
			if (best_5.size() < 5)
				best_5.push(now);
			else if (best_5.top().simu_score < now.simu_score)
			{
				best_5.pop();
				best_5.push(now);
			}

			now.up.pop_back();
			now.up.pop_back();
		}
		if (now.mid.size() + 2 <= 5 && (poker_offer_type.size() < 8 || poker_offer_type[i][0][1] == poker_offer_type[i][1][1]))
		{
			now.mid.push_back(poker_offer_type[i][0]);
			now.mid.push_back(poker_offer_type[i][1]);
			now.scoreis(s, king);
			if (best_5.size() < 5)
				best_5.push(now);
			else if (best_5.top().simu_score < now.simu_score)
			{
				best_5.pop();
				best_5.push(now);
			}
			now.mid.pop_back();
			now.mid.pop_back();
		}
		if (now.down.size() + 2 <= 5 && (poker_offer_type.size() < 8 || poker_offer_type[i][0][1] == poker_offer_type[i][1][1]))
		{
			now.down.push_back(poker_offer_type[i][0]);
			now.down.push_back(poker_offer_type[i][1]);
			now.scoreis(s, king);
			if (best_5.size() < 5)
				best_5.push(now);
			else if (best_5.top().simu_score < now.simu_score)
			{
				best_5.pop();
				best_5.push(now);
			}
			now.down.pop_back();
			now.down.pop_back();
		}
		if (now.up.size() < 3 && now.mid.size() < 5)
		{
			now.up.push_back(poker_offer_type[i][0]);
			now.mid.push_back(poker_offer_type[i][1]);
			if (poker_offer_type.size() < 8 || poker_offer_type[i][0][1] == 'h')
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			swap(now.up[now.up.size() - 1], now.mid[now.mid.size() - 1]);

			if (poker_offer_type.size() < 8 || (now.mid.size() == 0 && poker_offer_type[i][0][1] == 'h') || (now.mid.size() > 0 && poker_offer_type[i][0][1] == now.mid[0][1]))
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			now.up.pop_back();
			now.mid.pop_back();
		}
		if (now.up.size() < 3 && now.down.size() < 5)
		{
			now.up.push_back(poker_offer_type[i][0]);
			now.down.push_back(poker_offer_type[i][1]);
			if (poker_offer_type.size() < 8 || poker_offer_type[i][0][1] == 'h')
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			swap(now.up[now.up.size() - 1], now.down[now.down.size() - 1]);

			if (poker_offer_type.size() < 8 || (now.down.size() == 0 && poker_offer_type[i][0][1] == 'h') || (now.down.size() > 0 && poker_offer_type[i][0][1] == now.down[0][1]))
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			now.up.pop_back();
			now.down.pop_back();
		}
		if (now.mid.size() < 5 && now.down.size() < 5)
		{
			now.mid.push_back(poker_offer_type[i][0]);
			now.down.push_back(poker_offer_type[i][1]);
			if (poker_offer_type.size() < 8 || (now.mid.size() == 0 && poker_offer_type[i][0][1] == 'h') || (now.mid.size() > 0 && poker_offer_type[i][0][1] == now.mid[0][1]))
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			swap(now.mid[now.mid.size() - 1], now.down[now.down.size() - 1]);

			if (poker_offer_type.size() < 8 || (now.down.size() == 0 && poker_offer_type[i][0][1] == 'h') || (now.down.size() > 0 && poker_offer_type[i][0][1] == now.down[0][1]))
			{
				now.scoreis(s, king);
				if (best_5.size() < 5)
					best_5.push(now);
				else if (best_5.top().simu_score < now.simu_score)
				{
					best_5.pop();
					best_5.push(now);
				}
			}
			now.mid.pop_back();
			now.down.pop_back();
		}
	}
	return best_5;
}

vector<OFC> get_OFC_7_answer(OFC &now, vector<int> &preX, vector<string> &poker_offer, vector<string> &poker_can_use)
{
	vector<string> num = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
	vector<string> ch = { "c", "d", "h", "s" };
	vector<string> poker;
	for (int i = 0; i < num.size(); i++)
	{
		for (int j = 0; j < ch.size(); j++)
			poker.push_back(num[i] + ch[j]);
	}

	//for (int i = 0; i < poker_offer.size(); i++)
	//{
	//	if (poker_offer[i][0] == 'X')
	//		poker_offer.erase(remove(poker_offer.begin(), poker_offer.end(), "Xx"));
	//}

	priority_queue<OFC, vector<OFC> > best_5;
	vector<vector<string> > poker_offer_type;
	vector<int> t(5);

	vector<string> tmp;
	switch (poker_offer.size())
	{
	case 3:
		combine(poker_offer, 3, 2, t, 2, poker_offer_type);
		break;
	case 2:
		for (int i = 0; i < poker_offer.size(); i++)
		{
			for (int j = 0; j < poker.size(); j++)
			{
				tmp = { poker[j], poker_offer[i] };
				poker_offer_type.push_back(tmp);
			}
		}
		break;
	case 1:
		for (int i = 0; i < poker.size(); i++)
		{
			for (int j = 0; j < poker.size(); j++)
			{
				if (poker[i][1] == poker[j][1])
				{
					tmp = { poker[i], poker[j] };
					poker_offer_type.push_back(tmp);
				}
			}
		}
		break;
	}

	/*for (int i = 0; i < poker_offer_type.size(); i++)
	{
		for (int j = 0; j < poker_offer_type[i].size(); j++)
			cout << poker_offer_type[i][j] << " ";
		cout << endl;
	}*/

	now.poker_can_use = poker_can_use;
	now.card_num = now.up.size() + now.mid.size() + now.down.size() + 2;
	int number = preX[0] + preX[1] + preX[2];
	int king;
	if (number == 2 && preX[0] != 2 && preX[1] != 2 && preX[2] != 2)
		king = 3;
	else
		king = number + 3 - poker_offer.size();
	switch (number)
	{
	case 0:
		best_5 = get_aim_7(now, poker_offer_type, king);
		break;
	case 1:
		now.card_num += 1;
		if (preX[0] == 1)
		{
			string color;
			if (now.up.size() == 0)
				color = ch[0];
			else
				color = now.up[now.up.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.up.push_back(num[i] + color);
				best_5 = get_aim_7(now, poker_offer_type, king);
				now.up.pop_back();
			}
		}
		else if (preX[1] = 1)
		{
			string color;
			if (now.mid.size() == 0)
				color = ch[0];
			else
				color = now.mid[now.mid.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.mid.push_back(num[i] + color);
				best_5 = get_aim_7(now, poker_offer_type, king);
				now.mid.pop_back();
			}
		}
		else if (preX[2] = 1)
		{
			string color;
			if (now.down.size() == 0)
				color = ch[0];
			else
				color = now.down[now.down.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.down.push_back(num[i] + color);
				best_5 = get_aim_7(now, poker_offer_type, king);
				now.down.pop_back();
			}
		}
		break;
	case 2:
		now.card_num += 2;
		string color1, color2;
		if (preX[0] == 2)
		{
			color1 = ch[0];
			for (int i = 0; i < 13; i++)
			{
				now.up.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.up.push_back(num[j] + color1);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.up.pop_back();
				}
				now.up.pop_back();
			}
		}
		else if (preX[1] == 2)
		{
			if (now.mid.size() == 0)
				color1 = ch[0];
			else
				color1 = now.mid[now.mid.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.mid.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.mid.push_back(num[j] + color1);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.mid.pop_back();
				}
				now.mid.pop_back();
			}
		}
		else if (preX[2] == 2)
		{
			if (now.down.size() == 0)
				color1 = ch[0];
			else
				color1 = now.down[now.down.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.down.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.down.push_back(num[j] + color1);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.down.pop_back();
				}
				now.down.pop_back();
			}
		}
		else if (preX[0] == 1 && preX[1] == 1)
		{
			color1 = ch[0];
			if (now.mid.size() == 0)
				color2 = ch[0];
			else
				color2 = now.mid[now.mid.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.up.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.mid.push_back(num[j] + color2);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.mid.pop_back();
				}
				now.up.pop_back();
			}
		}
		else if (preX[0] == 1 && preX[2] == 1)
		{
			color1 = ch[0];
			if (now.down.size() == 0)
				color2 = ch[0];
			else
				color2 = now.down[now.down.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.up.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.down.push_back(num[j] + color2);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.down.pop_back();
				}
				now.up.pop_back();
			}
		}
		else if (preX[1] == 1 && preX[2] == 1)
		{
			if (now.mid.size() == 0)
				color1 = ch[0];
			else
				color1 = now.mid[now.mid.size() - 1][1];
			if (now.down.size() == 0)
				color2 = ch[0];
			else
				color2 = now.down[now.down.size() - 1][1];
			for (int i = 0; i < 13; i++)
			{
				now.mid.push_back(num[i] + color1);
				for (int j = 0; j < 13; j++)
				{
					now.down.push_back(num[j] + color2);
					best_5 = get_aim_7(now, poker_offer_type, king);
					now.down.pop_back();
				}
				now.mid.pop_back();
			}
		}
		break;
	}


	vector<OFC> result;
	while (!best_5.empty())
	{
		result.push_back(best_5.top());
		best_5.pop();
	}

	set<string> poker_user(poker_offer.begin(), poker_offer.end());
	for (int i = 0; i < 5; i++)
	{
		switch (preX[0])
		{
		case 1:
			result[i].up[now.up.size()] = "Xx";
			break;
		case 2:
			result[i].up[now.up.size()] = result[i].up[now.up.size() + 1] = "Xx";
		}
		switch (preX[1])
		{
		case 1:
			result[i].mid[now.mid.size()] = "Xx";
			break;
		case 2:
			result[i].mid[now.mid.size()] = result[i].mid[now.mid.size() + 1] = "Xx";
		}
		switch (preX[2])
		{
		case 1:
			result[i].down[now.down.size()] = "Xx";
			break;
		case 2:
			result[i].down[now.down.size()] = result[i].down[now.down.size() + 1] = "Xx";
		}

		for (int j = now.up.size(); j < result[i].up.size(); j++)
		{
			if (poker_user.count(result[i].up[j]) == 0)
				result[i].up[j] = "Xx";
		}
		for (int j = now.mid.size(); j < result[i].mid.size(); j++)
		{
			if (poker_user.count(result[i].mid[j]) == 0)
				result[i].mid[j] = "Xx";
		}
		for (int j = now.down.size(); j < result[i].down.size(); j++)
		{
			if (poker_user.count(result[i].down[j]) == 0)
				result[i].down[j] = "Xx";
		}
	}
	return result;
}

vector<OFC> get_OFC_fantacy(vector<string> &poker_offer, vector<string> &poker_can_use, int king)
{
	vector<vector<string> > poker_offer_type;
	vector<int> t(13);
	combine(poker_offer, poker_offer.size(), 13 - king, t, 13 - king, poker_offer_type);
	priority_queue<OFC> best_3;
	OFC now;
	now.poker_can_use = poker_can_use;
	now.card_num = 13;
	for (int i = 0; i < poker_offer_type.size(); i++)
	{

	}
}

int main()
{
	vector<string> num = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
	vector<string> ch = { "c", "d", "h", "s" };
	vector<string> poker;
	for (int i = 0; i < num.size(); i++)
	{
		for (int j = 0; j < ch.size(); j++)
			poker.push_back(num[i] + ch[j]);
	}

	for (int step = 0; step < 1; step++)
	{
		vector<string>poke = poker;
		vector<string> poker_this_step = poker;
		/*random_shuffle(poker_this_step.begin(), poker_this_step.end());
		for (int i = 0; i < poker_this_step.size(); i++)
			cout << poker_this_step[i] << " ";
		cout << endl;
		set<string> tmp;
		OFC now;
		for (int i = 0; i < 5; i++)
		{
			tmp.insert(poker_this_step[i]);
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		}
		vector<OFC> result = get_OFC_First5(tmp);*/

		vector<OFC> result;
		set<string> tmp;
		OFC now;
		poke.erase(remove(poke.begin(), poke.end(), "As"));
		poke.erase(remove(poke.begin(), poke.end(), "6s"));
		//poke.erase(remove(poke.begin(), poke.end(), "6d"));
		poke.erase(remove(poke.begin(), poke.end(), "9c"));
		poke.erase(remove(poke.begin(), poke.end(), "9h"));
		//poke.erase(remove(poke.begin(), poke.end(), "Qc"));
		poke.erase(remove(poke.begin(), poke.end(), "Th"));
		//poke.erase(remove(poke.begin(), poke.end(), "8s"));
		/*poke.erase(remove(poke.begin(), poke.end(), "7d"));*/
		now.up = { "As" };
		now.mid = { "6s", "Th"/*, "8s" */ };
		now.down = { "9c", "9h"/*, "Qc"*/ /*, "7d"*/ };

		vector<string> tmp1;
		tmp1.push_back("9s");
		//tmp1.push_back("9d");
		//tmp1.push_back("Xx");
		//tmp1.push_back("6d");

		//poke.erase(remove(poke.begin(), poke.end(), "6d"));
		//poke.erase(remove(poke.begin(), poke.end(), "9d"));
		poke.erase(remove(poke.begin(), poke.end(), "9s"));
		//poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));

		vector<int> preX = { 0, 0, 0 };
		result = get_OFC_7_answer(now, preX, tmp1, poke);   //now -> result[4]
		for (int i = 0; i < 5; i++)
		{
			result[i].print();
			cout << result[i].simu_score << endl;
		}
		tmp1.clear();



		/*vector<string> tmp1;
		for(int i = 5; i < 15; i++)
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		for (int i = 15; i < 18; i++)
		{
			tmp1.push_back(poker_this_step[i]);
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		}
		result = get_OFC_7_answer(now, tmp1, poke);   //now -> result[4]
		tmp1.clear();


		for (int i = 18; i < 22; i++)
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		for (int i = 22; i < 25; i++)
		{
			tmp1.push_back(poker_this_step[i]);
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		}
		result = get_OFC_7_answer(result[4], tmp1, poke);
		tmp1.clear();

		for (int i = 25; i < 29; i++)
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		for (int i = 29; i < 32; i++)
		{
			tmp1.push_back(poker_this_step[i]);
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		}
		result = get_OFC_7_answer(result[4], tmp1, poke);
		tmp1.clear();

		for (int i = 32; i < 36; i++)
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		for (int i = 36; i < 39; i++)
		{
			tmp1.push_back(poker_this_step[i]);
			poke.erase(remove(poke.begin(), poke.end(), poker_this_step[i]));
		}
		result = get_OFC_7_answer(result[4], tmp1, poke);
		tmp1.clear();

		for (int i = 0; i < result.size(); i++)
		{
			result[i].print();
			cout << result[i].simu_score << endl;
		}*/
	}

	//vector<string> p = { "As", "Kc", "5d" };
	//OFC now;
	//now.up = { "Ac"};
	//now.mid = { "6s", "6d" };
	//now.down = { "9h", "9c" };
	//poker.erase(remove(poker.begin(), poker.end(), "As"));
	//poker.erase(remove(poker.begin(), poker.end(), "Kc"));
	//poker.erase(remove(poker.begin(), poker.end(), "5d"));
	//poker.erase(remove(poker.begin(), poker.end(), "Ac"));
	//poker.erase(remove(poker.begin(), poker.end(), "6s"));
	//poker.erase(remove(poker.begin(), poker.end(), "6d"));
	//poker.erase(remove(poker.begin(), poker.end(), "9h"));
	//poker.erase(remove(poker.begin(), poker.end(), "9c"));

	/*now.poker_can_use = poker;
	now.card_num = now.up.size() + now.mid.size() + now.down.size();
	now.scoreis(s);
	cout << now.simu_score << endl;*/

	//vector<OFC> result = get_OFC_7_answer(now, p, poker);
	//for (int i = 0; i < result.size(); i++)
	//{
	//	result[i].print();
	//	cout << result[i].simu_score << endl;
	//}


	system("pause");
	return 0;


	/*set<string> cards(s.begin(), s.end());
	set<string> G = cards;
	for (set<string>::iterator it = cards.begin(); it != cards.end(); it++)
		cout << *it << endl;
	poker.clear();*/





	/*vector<string> test;
	cout << "INPUT START" << endl;
	while (1)
	{
		for (int i = 0; i < 5; i++)
		{
			cin >> tmp;
			test.push_back(tmp);
		}
		start = clock();
		sort(test.begin(), test.end());
		t = "";
		for (int i = 0; i < 5; i++)
			t += test[i];
		cout << t << "  " << data[t] << endl;
		cout << float(clock() - start) / CLOCKS_PER_SEC << endl;
		test.clear();
		system("pause");
	}
	cout << float(clock() - start) / CLOCKS_PER_SEC << endl;
	fin.close();*/

}

//前5张牌读取测试
	//vector<string> num = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
	//vector<string> ch = { "c", "d", "h", "s" };
	//vector<string> poker;
	//for (int i = 0; i < num.size(); i++)
	//{
	//	for (int j = 0; j < ch.size(); j++)
	//		poker.push_back(num[i] + ch[j]);
	//}
	//set<string> aim;
	//string tmp;
	//for (int i = 0; i < 5; i++)
	//{
	//	cin >> tmp;
	//	if (tmp[0] != 'X')
	//	{
	//		poker.erase(remove(poker.begin(), poker.end(), tmp));
	//		aim.insert(tmp);
	//	}
	//}
	//vector<OFC> First_5 = get_OFC_First5(aim, poker);
	//for (int i = 0; i < 5; i++)
	//{
	//	First_5[i].print();
	//	cout << First_5[i].simu_score << endl << endl;
	//}


//验证特定牌型的分数
/*OFC tmp;
tmp.up = { "As", "Ah", "Ad" };
tmp.mid = { "Tc", "Qc", "Kc", "Jc", "9d" };
tmp.down = { "Ac", "Th", "Js", "Qd", "Kh" };
vector<card> a(3), b(5), c(5);
int k;
for (k = 0; k < 3; k++)
{
	a[k].num = tmp.up[k][0];
	a[k].ch = tmp.up[k][1];
}
for (k = 0; k < 5; k++)
{
	b[k].num = tmp.mid[k][0];
	b[k].ch = tmp.mid[k][1];
}
for (k = 0; k < 5; k++)
{
	c[k].num = tmp.down[k][0];
	c[k].ch = tmp.down[k][1];
}
cout << s.three_grade(a, b, c);
system("pause"); */


//计算5张牌所有情况，删减了重复情况。
//for (int i = 2; i < 10; i++)
//{
//	card_to_num[i + '0'] = i - 1;
//}
//card_to_num['T'] = 9;
//card_to_num['J'] = 10;
//card_to_num['Q'] = 11;
//card_to_num['K'] = 12;
//card_to_num['A'] = 13;
//for (int i = 0; i < num.size(); i++)
//{
//	C.push_back(num[i] + ch[0]);
//	D.push_back(num[i] + ch[1]);
//	H.push_back(num[i] + ch[2]);
//	S.push_back(num[i] + ch[3]);
//}
//vector<vector<string> > result;
//makeFirst5(result);
//cout << result.size() << endl;
//ofstream fout("First5Cards_final.txt");
//for (int i = 0; i < result.size(); i++)
//{
//	for (int j = 0; j < result[i].size(); j++)
//		fout << result[i][j] << "  ";
//	fout << endl;
//}

//vector<string> num = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
//vector<string> ch = { "c", "d", "h", "s" };
//int cnt[6][4] = { { 5, 0, 0, 0 },{ 4, 1, 0, 0 },{ 3, 2, 0, 0 },{ 2, 2, 1, 0 } ,{ 3, 1, 1, 0 },{ 2, 1, 1, 1 } };
//map<char, int> card_to_num;
//vector<string> C, D, H, S;
//vector<int> t(5);
//
//void makeFirst5(vector<vector<string> > &vec_res)
//{
//	vector<vector<string> > tmp1, tmp2, tmp3, tmp0;
//
//	vector<string> tmp;
//	for (int step = 0; step < 4; step++)
//	{
//		tmp1.clear();
//		tmp2.clear();
//		tmp3.clear();
//		tmp0.clear();
//		combine(C, 13, cnt[step][0], t, cnt[step][0], tmp0);
//		if (cnt[step][1] != 0)
//			combine(D, 13, cnt[step][1], t, cnt[step][1], tmp1);
//		if (cnt[step][2] != 0)
//			combine(H, 13, cnt[step][2], t, cnt[step][2], tmp2);
//		if (cnt[step][3] != 0)
//			combine(S, 13, cnt[step][3], t, cnt[step][3], tmp3);
//		cout << step << " " << tmp0.size() << " " << tmp1.size() << " " << tmp2.size() << " " << tmp3.size() << " " << endl;
//		for (int i = 0; i < tmp0.size(); i++)
//		{
//			int a = tmp0[i].size();
//			if (a == 5)
//			{
//				tmp = tmp0[i];
//				vec_res.push_back(tmp);
//				continue;
//			}
//			for (int j = 0; j < tmp1.size(); j++)
//			{
//				int b = tmp1[j].size();
//				if (a + b == 5)
//				{
//					tmp = tmp0[i];
//					tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//					vec_res.push_back(tmp);
//					continue;
//				}
//				for (int k = 0; k < tmp2.size(); k++)
//				{
//					int c = tmp2[k].size();
//					if (a + b + c == 5)
//					{
//						tmp = tmp0[i];
//						tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//						tmp.insert(tmp.end(), tmp2[k].begin(), tmp2[k].end());
//						vec_res.push_back(tmp);
//						continue;
//					}
//					for (int l = 0; l < tmp3.size(); l++)
//					{
//						int d = tmp3[l].size();
//
//						if (a + b + c + d == 5)
//						{
//							tmp = tmp0[i];
//							tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//							tmp.insert(tmp.end(), tmp2[k].begin(), tmp2[k].end());
//							tmp.insert(tmp.end(), tmp3[l].begin(), tmp3[l].end());
//							vec_res.push_back(tmp);
//							continue;
//						}
//					}
//				}
//			}
//		}
//		cout << step << "down!" << endl;
//		cout << vec_res.size() << endl;
//
//
//	}
//	for (int step = 4; step < 6; step++)
//	{
//		tmp1.clear();
//		tmp2.clear();
//		tmp3.clear();
//		tmp0.clear();
//		combine(C, 13, cnt[step][0], t, cnt[step][0], tmp0);
//		if (cnt[step][1] != 0)
//			combine(D, 13, cnt[step][1], t, cnt[step][1], tmp1);
//		/*if (cnt[step][2] != 0)
//			combine(H, 13, cnt[step][2], t, cnt[step][2], tmp2);*/
//			/*if (cnt[step][3] != 0)
//				combine(S, 13, cnt[step][3], t, cnt[step][3], tmp3);*/
//				//cout << step << " " << tmp0.size() << " " << tmp1.size() << " " << tmp2.size() << " " << tmp3.size() << " " << endl;
//		for (int i = 0; i < tmp0.size(); i++)
//		{
//			int a = tmp0[i].size();
//			if (a == 5)
//			{
//				tmp = tmp0[i];
//				vec_res.push_back(tmp);
//				continue;
//			}
//			for (int j = 0; j < tmp1.size(); j++)
//			{
//				int b = tmp1[j].size();
//				if (a + b == 5)
//				{
//					tmp = tmp0[i];
//					tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//					vec_res.push_back(tmp);
//					continue;
//				}
//				tmp2.clear();
//				if (cnt[step][2] != 0)
//					combine(H, card_to_num[tmp1[j][0][0]], cnt[step][2], t, cnt[step][2], tmp2);
//				for (int k = 0; k < tmp2.size(); k++)
//				{
//					int c = tmp2[k].size();
//					if (a + b + c == 5)
//					{
//						tmp = tmp0[i];
//						tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//						tmp.insert(tmp.end(), tmp2[k].begin(), tmp2[k].end());
//						vec_res.push_back(tmp);
//						continue;
//					}
//					tmp3.clear();
//					if (cnt[step][3] != 0)
//						combine(S, card_to_num[tmp2[k][0][0]], cnt[step][3], t, cnt[step][3], tmp3);
//					for (int l = 0; l < tmp3.size(); l++)
//					{
//						int d = tmp3[l].size();
//
//						if (a + b + c + d == 5)
//						{
//							tmp = tmp0[i];
//							tmp.insert(tmp.end(), tmp1[j].begin(), tmp1[j].end());
//							tmp.insert(tmp.end(), tmp2[k].begin(), tmp2[k].end());
//							tmp.insert(tmp.end(), tmp3[l].begin(), tmp3[l].end());
//							vec_res.push_back(tmp);
//							continue;
//						}
//					}
//				}
//			}
//		}
//		cout << step << "down!" << endl;
//		cout << vec_res.size() << endl;
//	}
//}


//验证0ne_grade的正确性
//string tmp;
//int n = 50;
//vector<vector<string> > a(n);
//ifstream fin("gradetest5.txt");
//for (int i = 0; i < n; i++)
//{
//	for (int j = 0; j < 5; j++)
//	{
//		fin >> tmp;
//		a[i].push_back(tmp);
//	}
//}
//fin.close();
//
//Score s;
//for (int i = 0; i < n; i++)
//{
//	vector<card> c(5);
//	for (int j = 0; j < 5; j++)
//	{
//		c[j].num = a[i][j][0];
//		c[j].ch = a[i][j][1];
//	}
//	CompareCards aa(c);
//	aa.grade5is();
//	cout << i << "  " << s.one_grade(9 - aa.grade, 0, 1) << endl;
//}


//随机选择3张、5张测试计分函数
//string tmp;
//int ans;
//int n = 24, m = 140;
//vector<vector<string> > a3(n), b5(m);
//ifstream fin("3cards.txt");
//for (int i = 0; i < n / 2; i++)
//{
//	for (int j = 0; j < 3; j++)
//	{
//		fin >> tmp;
//		a3[i].push_back(tmp);
//	}
//	for (int j = 0; j < 3; j++)
//	{
//		fin >> tmp;
//		a3[i + 12].push_back(tmp);
//	}
//	/*for (int j = 0; j < 3; j++)
//	cout << a3[j] << "   " << b3[j] << endl;*/
//	fin >> ans;
//	//cout << ans << endl;
//}
//fin.close();
//
//fin.open("5cards.txt");
//for (int i = 0; i < m / 2; i++)
//{
//	for (int j = 0; j < 5; j++)
//	{
//		fin >> tmp;
//		b5[i].push_back(tmp);
//	}
//	for (int j = 0; j < 5; j++)
//	{
//		fin >> tmp;
//		b5[i + 70].push_back(tmp);
//	}
//	/*for (int j = 0; j < 3; j++)
//	cout << a[j] << "   " << b[j] << endl;*/
//	fin >> ans;
//	//cout << ans << endl;
//}
//fin.close();
//
//Score s;
//srand(time(NULL));
//for (int i = 0; i < 50; i++)
//{
//	int a = rand() % n;
//	int b = rand() % m;
//	int c = rand() % m;
//	OFC tmp;
//	tmp.up = a3[a];
//	tmp.mid = b5[b];
//	tmp.down = b5[c];
//	tmp.print();
//	cout << tmp.scoreis(s) << endl << endl;;
//}


//检查5张牌比较函数正误
//string tmp;
//int ans;
//ifstream fin("5cards.txt");
//for (int i = 0; i < 70; i++)
//{
//	vector<string> a, b;
//	for (int j = 0; j < 5; j++)
//	{
//		fin >> tmp;
//		a.push_back(tmp);
//	}
//	for (int j = 0; j < 5; j++)
//	{
//		fin >> tmp;
//		b.push_back(tmp);
//	}
//	/*for (int j = 0; j < 3; j++)
//	cout << a[j] << "   " << b[j] << endl;*/
//	fin >> ans;
//	//cout << ans << endl;
//	vector<card> aa(5), bb(5);
//	for (int j = 0; j < 5; j++)
//	{
//		aa[j].num = a[j][0];
//		aa[j].ch = a[j][1];
//	}
//	for (int j = 0; j < 5; j++)
//	{
//		bb[j].num = b[j][0];
//		bb[j].ch = b[j][1];
//	}
//
//	CompareCards aaa(aa), bbb(bb);
//	aaa.grade5is();
//	bbb.grade5is();
//	/*for (int j = 0; j <= 13; j++)
//	cout << aaa.cards_num[j];
//	cout << endl;
//	for (int j = 0; j <= 13; j++)
//	cout << bbb.cards_num[j];
//	cout << endl;*/
//	if (aaa.compare_5cards(bbb) != ans)
//		cout << "Wrong!" << endl;
//	cout << aaa.grade << ' ' << bbb.grade << endl;
//	cout << aaa.compare_5cards(bbb) << "  " << ans << endl << endl;
//


/*前5张卡读取
string tmp;
map<string, int> data;
ifstream fin("First5Cards1.txt");
clock_t start = clock();
vector<string> s;
string t;
for (int i = 0; i < 2598960; i++)
{
for (int j = 0; j < 5; j++)
{
fin >> tmp;
s.push_back(tmp);
}
sort(s.begin(), s.end());
t = "";
for (int i = 0; i < 5; i++)
t += s[i];
data[t] = i;
s.clear();
}
vector<string> test;
cout << "INPUT START" << endl;
while (1)
{
for (int i = 0; i < 5; i++)
{
cin >> tmp;
test.push_back(tmp);
}
start = clock();
sort(test.begin(), test.end());
t = "";
for (int i = 0; i < 5; i++)
t += test[i];
cout << t << "  " << data[t] << endl;
cout << float(clock() - start) / CLOCKS_PER_SEC << endl;
test.clear();
system("pause");
}
cout << float(clock() - start) / CLOCKS_PER_SEC << endl;
fin.close();*/