//#include<iostream>
//#include<omp.h>
//#include<ctime>
//#include<fstream>
//#include<vector>
//#include<string>
//#include<set>
//#include<algorithm>
//#include<map>
//using namespace std;
//
//void test(int n)
//{
//	vector<int> num;
//	for (int i = 0; i < n; i++)
//		num.push_back(i);
//
//}
//
//
//void combine(vector<string> &data, int n, int m, vector<int> &temp, const int M, vector<vector<string> > &vec_res)
//{
//	for (int i = n; i >= m; i--)   // 注意这里的循环范围
//	{
//		temp[m - 1] = i - 1;
//		if (m > 1)
//			combine(data, i - 1, m - 1, temp, M, vec_res);
//		else                     // m == 1, 输出一个组合
//		{
//			vector<string> vec_temp;
//			for (int j = M - 1; j >= 0; j--) {
//				vec_temp.push_back(data[temp[j]]);
//			}
//			vec_res.push_back(vec_temp);
//		}
//	}
//}
//
//int main()
//{
//	vector<string> num = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
//	vector<string> ch = { "c", "d", "h", "s" };
//	string tmp;
//	vector<string> cards;
//	for (int i = 0; i < num.size(); i++)
//	{
//		for (int j = 0; j < ch.size(); j++)
//		{
//			tmp = num[i] + ch[j];
//			cards.push_back(tmp);
//		}
//	}
//	vector<string> s;
//	srand(time(0));
//	clock_t start = clock();
//
//
//	///*for (int step = 0; step < 1000; step++)
//	//{*/
//	//for (int i = 0; i < 5; i++)
//	//{
//	//	int rad = rand() % cards.size();
//	//	vector<string>::iterator it = cards.begin() + rad;
//	//	//for (int i = 0; i < rad; i++)
//	//	//	it++;
//	//	s.push_back(*it);
//	//	cards.erase(it);
//	//}
//	//for (int i = 0; i < s.size(); i++)
//	//{
//	//	cout << s[i] << "  ";
//	//	cards.push_back(s[i]);
//	//}
//	//cout << endl;
//	////s.clear();
//	////}
//
//
//	vector<int> tp(5);
//	vector<vector<string> > result;
//	combine(cards, 48, 1, tp, 1, result);
//	//ofstream fout("First5Cards.txt");
//	for (int i = 0; i < result.size(); i++)
//	{
//		for (int j = 0; j < result[i].size(); j++)
//			cout << result[i][j] << "  ";
//		cout << endl;
//	}
//	cout << result.size() << endl;
//	cout << float(clock() - start) / CLOCKS_PER_SEC << endl;
//	cout << cards.size() << endl;
//
//	system("pause");
//	return 0;
//}