#include "Score.h"
#include<fstream>
//#include"Cards.h"



Score::Score()
{
	std::ifstream fin("score.txt");
	for (int i = 0; i < 10; i++)
		fin >> score[i];
	fin.close();
	illegal = -6;
	/*for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fin >> progression[i][j];
		}
	}*/
}


Score::~Score()
{
}

float Score:: one_grade(int s, int num, int position) const
{
	float result = 0;
	switch (position)
	{
	case 0:
		if (num == 0)
			num = 13;
		if (s == 1)
		{
			if (num >= 5)
				result = score[s] + num - 5;
			switch (num)
			{
			case 13:
				result += 55;
				break;
			case 12:
				result += 20;
				break;
			case 11:
				result += 6;
				break;
			}
		}
		else if (s == 3)
			result = score[s] + num + 110;
		break;
	case 1:
		if (s < 4)
			break;
		result = score[s];
		/*for (int i = 0; i < 3; i++)
			result += num[i] * progression[s][i];*/
		break;
	case 2:
		if (s < 4)
			break;
		result = score[s] / 2;

		/*for (int i = 0; i < 3; i++)
			result += num[i] * progression[s][i] / 2;*/
	}
	return result;
}

float Score:: three_grade(vector<card>up, vector<card>mid, vector<card>down) const
{
	CompareCards _up(up), _mid(mid), _down(down);
	int num = _up.grade3is();
	_mid.grade5is();
 	_down.grade5is();
	if (_mid.compare_5cards(_down) == 1 || _up.compare_35cards(num, _mid) == 1)
		return illegal;
	else
	{
		/*cout << _up.grade << " " << _mid.grade << " " << _down.grade << endl;
		cout << one_grade(9 - _up.grade, num, 0) << "  " << one_grade(9 - _mid.grade, 0, 1) << "  " << one_grade(9 - _down.grade, 0, 2) << endl;*/
		return one_grade(9 - _up.grade, num, 0) + one_grade(9 - _mid.grade, 0, 1) + one_grade(9 - _down.grade, 0, 2);
	}
}