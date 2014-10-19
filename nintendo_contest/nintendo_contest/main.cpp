#include <iostream>
#include <string>

using namespace std;

int main()
{
	int size;
	//cin >> size;

	size = 32;

	unsigned int* a = new unsigned int[size / 16]; // <- input tab to encrypt
	unsigned int* b = new unsigned int[size / 16]; // <- output tab

	a[0] = 0x000073af;
	a[1] = 0x00000001;

	//for (int i = 0; i < size / 16; i++) {   // Read size / 16 integers to a
	//	cin >> hex >> a[i];
	//}

	unsigned int* a_bit = new unsigned int[size * 2];
	unsigned int* b_bit = new unsigned int[size * 2];
	unsigned int* c_bit = new unsigned int[size * 2];
	char** b_str = new char*[size * 2];

	for (int i = 0; i < size * 2; i++)
	{
		a_bit[i] = (a[i / 32] >> (i % 32)) & 1;
		printf("%d", a_bit[i]);
		if (i % 4 == 3)
			printf("\t");
		if (i % 32 == 31)
			printf("\n");

		b_bit[i] = 0;
		c_bit[i] = 0;
		b_str[i] = new char[1000];
		memset(b_str[i], 0, sizeof(char)* 1000);

	}
	printf("\n\n");
	
	cout << hex << a[0] << " ";
	cout << hex << a[1] << " ";

	printf("\n\n");

	//{
	//	for (int i = 0; i < size / 16; i++) {   // Write size / 16 zeros to b
	//		b[i] = 0;
	//	}

	//	for (int i = 0; i < size; i++)
	//	for (int j = 0; j < size; j++)
	//		b[(i + j) / 32] ^= ((a[i / 32] >> (i % 32)) &
	//		(a[j / 32 + size / 32] >> (j % 32)) & 1) << ((i + j) % 32);   // Magic centaurian operation

	//	for (int i = 0; i < size / 16; i++)
	//		cout << hex << b[i] << " ";       // print result

	//	printf("\n\n");
	//}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			b_bit[i + j] ^= a_bit[i] & a_bit[size + j];
			char tmp[100];
			sprintf_s(tmp, " (+) a[%d]^a[%d]", i, size + j);
			strcat_s(b_str[i + j], 1000, tmp);
		}
	}

	for (int i = 0; i < 2 * size; i++)
	{
		printf("%d", b_bit[i]);
		if (i % 4 == 3)
		{
			printf("\t");
		}
		if (i % 32 == 31)
		{
			printf("\n");
		}
	}

	// 다른 방법으로 iteration.
	/*{
		for (int k = 0; k < 2 * size; ++k)
		{
			for (int i = 0; i < size; i++)
			{
				if (i <= k && i >= k - size + 1)
				{
					c_bit[k] ^= a_bit[i] & a_bit[size + k - i];
				}
			}
		}

		printf("\n\n");

		for (int i = 0; i < 2 * size; i++)
		{
			printf("%d", c_bit[i]);
			if (i % 4 == 3)
			{
				printf("\t");
			}
			if (i % 32 == 31)
			{
				printf("\n");
			}
		}
	}*/

	printf("\n\n");

	for (int i = 0; i < size * 2; ++i)
	{
		printf("%d: %s\n", i, b_str[i]);
	}

	printf("\n\n");

	int tmp_all = 0;
	int tmp_a1 = 0;
	int tmp_a2 = 0;

	for (int i = 0; i < 2 * size - 1; i++)
	{
		tmp_all ^= b_bit[i];
	}

	for (int i = 0; i < size; i++)
	{
		tmp_a1 ^= a_bit[i];
		tmp_a2 ^= a_bit[size + i];
	}

	cout << tmp_all;

	printf("\n\n");
	cout << tmp_a1;

	printf("\n\n");
	cout << tmp_a2;

	printf("\n\n");
	cout << (tmp_all == (tmp_a1 & tmp_a2)) ? 1 : 0;

	printf("\n\n");

	system("pause");

	return 0;
}