//#include"Cards.h"
#include"Score.h"
#include<algorithm>
using namespace std;

class OFC
{
public:
	vector<string> poker_can_use;
	vector<string> up;
	vector<string> mid;
	vector<string> down;
	float simu_score;
	int simu_num;
	int card_num;

	OFC();
	~OFC();
	bool operator < (const OFC &b) const;
	void scoreis(Score &s);
	float simulation(Score &s, vector<string> poker_can_use);
	void combine(vector<string> &data, int n, int m, vector<int> &temp, const int M, vector<vector<string> > &vec_res);
	void print();
};

