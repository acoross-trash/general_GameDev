#include <iostream>
#include <string>
#include <sstream>

#include"Parser.h"

using namespace std;

int main()
{
//	cout << sizeof(long long int) << endl;

	string input_str;
	string output_str;
	long long int input_num;

	cout << "Input your number: ";
	cin >> input_str;
	stringstream str(input_str);
	str >> input_num;

	cout << "Your number is: " << input_num << endl;
	
	Parser parser;
	parser.Number2Words(input_num);

	cout << parser.GetResultStr() << endl;

	system("pause");

	return 0;
}