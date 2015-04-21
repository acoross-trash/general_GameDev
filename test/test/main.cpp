#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<Windows.h>

using namespace std;

vector<int> foo()
{
	return vector<int>(10, 10);
}

int main()
{
	void* ptr = ::operator new(10);
	operator delete(ptr);

	deque<int> dq;
	dq.push_back(1);

	vector<int> v1;

	int i = GetTickCount();
	for (int i = 0; i < 1000000; ++i)
	{
		v1.push_back(i);
	}

	int j = GetTickCount() - i;

	cout << j << " ms" << endl;

	system("pause");

	return 0;
}