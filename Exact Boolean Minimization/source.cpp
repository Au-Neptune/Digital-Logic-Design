// Author : Neptune
// Date : 2021/11/17
// Problem : Exact Boolean Minimization

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct _pair {
	vector<string> prime;
	vector<vector<int>> prime_minterm;
};

vector<int>dontcare;
vector<string> list;

int Bin2Dec(string str)
{
	int ans = 0, times = 1;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		ans += (str[i] - '0') * times;
		times *= 2;
	}
	return ans;
}

vector<int> Dec2Bin(int num, int n)
{
	vector<int> a;
	int i;
	for (i = 0; num > 0; i++)
	{
		a.push_back(num % 2);
		num /= 2;
	}
	for (i; i < n; i++)
		a.push_back(0);
	return a;
}

void push(string str)
{
	string temp;
	bool flag = false;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '-')
		{
			flag = true;
			temp = str;
			temp[i] = '0'; push(temp);
			temp[i] = '1'; push(temp);
		}
	}
	if (!flag && find(list.begin(), list.end(), str) == list.end())
		list.push_back(str);
}

int count(string a)
{
	int num = 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] == '1')
			num++;
	}
	return num;
}

bool canReduce(string a, string b)
{
	int flag = 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] != b[i])
			flag++;
	}
	return (flag == 1);
}

bool in_minterm(vector<vector<int>> &prime_minterm, int i, int num)
{
	for (int j = 0; j < prime_minterm[i].size(); j++)
		if (num == prime_minterm[i][j])
			return true;
	return false;
}

_pair Qunie_McCluskey()
{
	sort(list.begin(), list.end());

	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 0; j < list.size() - 1; j++)
		{
			int n1 = count(list[j]);
			int n2 = count(list[j + 1]);
			if (n1 > n2)
			{
				string temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
			}
		}
	}

	vector<vector<int>> minterm;
	for (int i = 0; i < list.size(); i++)
	{
		vector<int>temp;
		temp.push_back(Bin2Dec(list[i]));
		minterm.push_back(temp);
	}

	vector<string> prime;
	vector<vector<int>> prime_minterm;
	while (!list.empty())
	{
		vector<string> temp;
		vector<vector<int>> tmp;
		vector<bool> flag(list.size());
		for (int i = 0; i < flag.size(); i++)
			flag[i] = false;
		for (int i = 0; i < list.size(); i++)
		{
			for (int j = i + 1; j < list.size(); j++)
			{
				if (canReduce(list[i], list[j]))
				{
					string reduce = list[i];
					for (int k = 0; k < list[i].length(); k++)
						if (list[i][k] != list[j][k])
							reduce[k] = '-';
					if (find(temp.begin(), temp.end(), reduce) == temp.end()) // not find
					{
						vector<int> a = minterm[i];
						a.insert(a.end(), minterm[j].begin(), minterm[j].end()); // merge minterm
						temp.push_back(reduce);
						tmp.push_back(a);
					}
					flag[i] = true;
					flag[j] = true;
				}
			}
			if (!flag[i])
			{
				prime.push_back(list[i]);
				prime_minterm.push_back(minterm[i]);
			}
		}

		list.clear();
		list = temp;
		minterm.clear();
		minterm = tmp;

		/*for (int i = 0; i < list.size(); i++)
		{
			cout << list[i] << "  ";
			for (int j = 0; j < minterm[i].size(); j++)
				cout << minterm[i][j] << " ";
			cout << endl;
		}
		cout << endl;*/
	}

	/*for (int i = 0; i < prime.size(); i++)
	{
		cout << prime[i] << "  ";
		for (int j = 0; j < prime_minterm[i].size(); j++)
			cout << prime_minterm[i][j] << " ";
		cout << endl;
	}*/

	_pair finale = { prime, prime_minterm };
	return finale;
}

vector<string> Petrick_Method(_pair a)
{
	vector<string> last;
	vector<string> prime = a.prime;
	vector<vector<int>> prime_minterm = a.prime_minterm;
	vector<vector<int>> table(prime.size());

	int max = 0;
	for (int i = 0; i < prime_minterm.size(); i++)
		for (int j = 0; j < prime_minterm[i].size(); j++)
			if (prime_minterm[i][j] > max)
					max = prime_minterm[i][j];

	for (int i = 0; i < prime.size(); i++)
	{
		table[i].resize(max + 1);
		for (int j = 0; j <= max; j++)
		{
			if (find(dontcare.begin(), dontcare.end(), j) != dontcare.end())
				continue;
			if (in_minterm(prime_minterm, i, j))
				table[i][j] = 1;
			else
				table[i][j] = 0;
		}
	}

	int row_size = table.size();
	int col_size = table[0].size();
	for (int i = 0; i < col_size; i++)
	{
		int count = 0, index;
		for (int j = 0; j < row_size; j++)
		{
			if (table[j][i] == 1)
			{
				count++;
				index = j;
			}
		}
		if (count == 1)
		{
			last.push_back(prime[index]);
			for (int i = 0; i < table[index].size(); i++)
				if (table[index][i] == 1)
					for (int j = 0; j < table.size(); j++)
						table[j][i] = 0;
		}
	}

	/*for (int i = 0; i < last.size(); i++)
		cout << last[i] << endl;

	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table[i].size(); j++)
			cout << table[i][j] << " ";
		cout << endl;
	}*/

	vector<int> col, row;
	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table[i].size(); j++)
		{
			if (table[i][j] == 1)
			{
				if (find(col.begin(), col.end(), j) == col.end())
					col.push_back(j);
				if (find(row.begin(), row.end(), i) == row.end())
					row.push_back(i);
			}
		}
	}

	int target = 0;
	for (auto& s : col)
		target += s;

	bool flag = false;

	for (int i = 1; i < pow(2, row.size()); i++)
	{
		int tol = 0, times = 2;
		vector<int>check;
		vector<string> pair;
		vector<int> com = Dec2Bin(i, row.size());

		for (int j = 0; j < com.size(); j++)
		{
			if (com[j] == 1)
			{
				for (auto& s : col)
				{
					if (table[row[j]][s] == 1 && (find(check.begin(), check.end(), s) == check.end()))
					{
						tol += s;
						check.push_back(s);
						if (find(pair.begin(), pair.end(), prime[row[j]]) == pair.end())
							pair.push_back(prime[row[j]]);
					}
				}
			}
		}
		if (tol == target)
		{
			//last.push_back(prime[row[i]]);
			last.insert(last.end(), pair.begin(), pair.end());
			break;
		}
	}
	return last;
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		cout << "command error!" << endl;
		return -1;
	}

	ifstream fin(argv[1]);
	string outfile = argv[2];

	if (!fin)
	{
		cout << "cant find the file!" << endl;
		return -1;
	}
	else
	{
		vector<string>last;
		string command;
		int input = 0, output = 0;
		char ilb[10];
		char ob[10];

		while (fin >> command)
		{
			if (command == ".i")
				fin >> input;
			else if (command == ".o")
				fin >> output;
			else if (command == ".ilb")
				for (int i = 0; i < input; i++)
					fin >> ilb[i];
			else if (command == ".ob")
				for (int i = 0; i < output; i++)
					fin >> ob[i];
			else if (command == ".p")
			{
				int r;
				fin >> r;
				for (int i = 0; i < r; i++)
				{
					string variable, symbol;
					fin >> variable;
					fin >> symbol;
					push(variable);
					if (symbol == "-")
						dontcare.push_back(Bin2Dec(variable));
				}

				_pair recive = Qunie_McCluskey();
				last = Petrick_Method(recive);
			}
			else if (command == ".e")
			{
				cout << "Total number of terms: " << last.size() << endl;
				int literals = 0;
				for (int i = 0; i < last.size(); i++)
				{
					for (int j = 0; j < last[i].length(); j++)
					{
						if (last[i][j] != '-')
							literals++;
					}
				}
				cout << "Total number of literals: " << literals << endl;
				ofstream fout(outfile);
				fout << ".i " << input << endl;
				fout << ".o " << output << endl;
				fout << ".ilb ";
				for (int i = 0; i < input; i++)
					fout << ilb[i] << " ";
				fout << endl << ".ob ";
				for (int i = 0; i < output; i++)
					fout << ob[i] << " ";
				fout << endl << ".p " << last.size() << endl;
				for (int i = 0; i < last.size(); i++)
					fout << last[i] << " 1" << endl;
				fout << ".e";
				fout.close();
			}
		}
		fin.close();
	}
}
