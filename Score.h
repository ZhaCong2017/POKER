#pragma once
#include"Cards.h"
using namespace std;
class Score
{
private:
	float score[9];
	float progression[9][3];
	float illegal;
public:
	Score();
	~Score();
	float one_grade(int s, int num, int position) const;
	float three_grade(vector<card>up, vector<card>mid, vector<card>down) const;
};

