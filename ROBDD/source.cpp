// Author : Neptune
// Date : 2021/10/15
// Problem : ROBDD Generation

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
using namespace std;

struct Node
{
	char variable; // Node name.
	int else_edge, then_edge;
	bool replaced = false;
};

void ROBDD(ifstream& fin, string outFile);
void Print(vector<Node>& n);
void Save(string filename, vector<Node>& n);
void Reduce(vector <Node>& n);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "parameter error!" << endl;
		return -1;
	}
	else
	{
		ifstream fin(argv[1]);
		ROBDD(fin, argv[2]);
	}

	return 0;
}

void ROBDD(ifstream& fin, string outFile)
{
	string command;
	vector<Node> n;
	int input = 0, output = 0;
	char* ilb = NULL;
	char* ob = NULL;

	while (!fin.eof())
	{
		fin >> command;
		if (command == ".i")
		{
			// 輸入數量
			fin >> input;
			n.resize((int)pow(2, input) + 1);

			ilb = new char[input];
			for (int i = 1; i < pow(2, input - 1); i++)
			{
				n[i].else_edge = i * 2;
				n[i].then_edge = i * 2 + 1;
			}
			for (int i = (int)pow(2, input - 1); i < (int)pow(2, input); i++)
			{
				n[i].else_edge = 0;
				n[i].then_edge = 0;
			}
			n[0].variable = '0';
			n[(int)pow(2, input)].variable = '1';
		}
		else if (command == ".o")
		{
			// 輸出數量
			fin >> output;
			ob = new char[output];
		}
		else if (command == ".ilb")
		{
			// 輸入變數
			for (int i = 0; i < input; i++)
			{
				fin >> ilb[i];
			}
			for (int i = 1; i < pow(2, input); i++)
			{
				n[i].variable = ilb[(int)log2(i)];
			}
		}
		else if (command == ".ob")
		{
			// 輸出變數
			for (int i = 0; i < output; i++)
			{
				fin >> ob[i];
			}
		}
		else if (command == ".p")
		{
			// 依照輸入格式填點
			// 從最上點開始(1)，遇到1走右邊，0走左邊
			// 走完後清空走過的點，為求值1最後落點
			int row;
			string temp;
			vector <int> log, result;
			int value;
			fin >> row;

			for (int i = 0; i < row; i++)
			{
				result.clear();
				result.push_back(1);
				fin >> temp; // 11-
				fin >> value; // 1

				for (int i = 0; i < temp.length(); i++)
				{
					log = result;
					result.clear();
					for (int j = 0; j < log.size(); j++)
					{
						if (temp[i] == '1')
						{
							result.push_back(log[j] * 2 + 1);
						}
						else if (temp[i] == '0')
						{
							result.push_back(log[j] * 2);
						}
						else
						{
							result.push_back(log[j] * 2);
							result.push_back(log[j] * 2 + 1);
						}
					}
				}

				// 將走完後的點(會是儲存值1的Node)填回上一層的else/then edge
				for (int i = 0; i < result.size(); i++)
				{
					if (result[i] % 2 == 1)
					{
						n[result[i] / 2].then_edge = pow(2, input);
					}
					else
					{
						n[result[i] / 2].else_edge = pow(2, input);
					}
				}
			}
		}
		else if (command == ".e")
		{
			// 輸入結束
			Reduce(n);
			Print(n);
			Save(outFile, n);
		}
	}
}

void Print(vector<Node> &n)
{
	// 輸出
	cout << "digraph ROBDD {" << endl;

	int count = 1;

	for (int i = 1; i < n.size() - 1; i++)
	{
		cout << "\t{rank=same";
		while (i <= count)
		{
			if (!n[i].replaced)
				cout << " " << i;
			i++;
		}
		i--;
		cout << "}" << endl;
		count = count * 2 + 1;
	}
	cout << "\n\t0 [label=0, shape=box]" << endl;
	for (int i = 1; i < n.size(); i++)
	{
		if (i == n.size() - 1)
			cout << "\t" << i << " [label=1, shape=box]" << endl;
		else
			if (!n[i].replaced)
				cout << "\t" << i << " [label=\"" << n[i].variable << "\"]" << endl;
	}
	cout << endl;
	for (int i = 1; i < n.size() - 1; i++)
	{
		if (!n[i].replaced)
		{
			printf("\t%d -> %d [label=\"0\", style=dotted]\n", i, n[i].else_edge);
			printf("\t%d -> %d [label=\"1\", style=solid]\n", i, n[i].then_edge);
		}
	}
	cout << "}" << endl;
}

void Reduce(vector <Node> &n)
{
	// 把相同的Node及then else相同的Node隱藏起來
	bool isDoingThing = false;
	for (int i = 1; i < n.size() - 1; i++)
	{
		if (n[i].replaced) continue;
		if (n[i].else_edge == n[i].then_edge)
		{
			n[i].replaced = true;
			isDoingThing = true;

			for (int j = 1; j < n.size() - 1; j++)
			{
				if (n[j].else_edge == i)
					n[j].else_edge = n[i].else_edge;
				if (n[j].then_edge == i)
					n[j].then_edge = n[i].else_edge;
			}
		}
	}

	for (int i = 1; i < n.size() - 1; i++)
	{
		for (int j = i + 1; j < n.size() - 1; j++)
		{
			if (n[i].replaced) continue;
			if (n[j].replaced) continue;

			// 兩節點完全相同
			if (n[i].else_edge == n[j].else_edge &&
				n[i].then_edge == n[j].then_edge &&
				n[i].variable == n[j].variable)
			{
				n[j].replaced = true;
				isDoingThing = true;

				for (int k = 1; k < n.size()-1; k++)
				{
					if (n[k].else_edge == j)
						n[k].else_edge = i;
					if (n[k].then_edge == j)
						n[k].then_edge = i;
				}
			}
		}
	}

	if (isDoingThing)
		Reduce(n);
}

void Save(string filename, vector<Node>& n)
{
	// 存成檔案
	int count = 1;
	ofstream fout(filename);

	fout << "digraph ROBDD {" << endl;

	for (int i = 1; i < n.size() - 1; i++)
	{
		fout << "\t{rank=same";
		while (i <= count)
		{
			if (!n[i].replaced)
				fout << " " << i;
			i++;
		}
		i--;
		fout << "}" << endl;
		count = count * 2 + 1;
	}
	fout << "\n\t0 [label=0, shape=box]" << endl;
	for (int i = 1; i < n.size(); i++)
	{
		if (i == n.size() - 1)
			fout << "\t" << i << " [label=1, shape=box]" << endl;
		else
			if (!n[i].replaced)
				fout << "\t" << i << " [label=\"" << n[i].variable << "\"]" << endl;
	}
	fout << endl;
	for (int i = 1; i < n.size() - 1; i++)
	{
		if (!n[i].replaced)
		{
			fout << "\t" << i << " -> " << n[i].else_edge << " [label=\"0\", style=dotted]\n";
			fout << "\t" << i << " -> " << n[i].then_edge << " [label=\"1\", style=solid]\n";
		}
	}
	fout << "}" << endl;
}
