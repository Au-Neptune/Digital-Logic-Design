// Author : Neptune
// Date : 2021/12/22
// Problem : State Minimization

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

struct my_state
{
	string after, input, output;
};

struct node
{
	string name;
	vector<my_state> state;
	bool reduce = false;
};

void MINI(ifstream& fin, string kiss_format, string dot_format);
void reduce(vector<node>& list);
void change(string target, string replace, vector<node>& list);
void kiss_format(int input_len, int output_len, string begin, vector<node>& list, string filename);
void dot_format(string begin, vector<node>& list, string filename);

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "parameter error!" << endl;
		return -1;
	}
	else
	{
		ifstream fin(argv[1]);

		if (!fin)
		{
			cout << "cant find the file!" << endl;
			return -1;
		}
		else
		{
			MINI(fin, argv[2], argv[3]);
		}
	}
	return 0;
}

void MINI(ifstream& fin, string kiss_name, string dot_name)
{
	vector<node> list;
	string command, begin;
	int input_len = 0, output_len = 0, row = 0, s = 0;

	while (fin >> command) {
		if (command == ".start_kiss")
			continue;
		else if (command == ".i")
			fin >> input_len;
		else if (command == ".o")
			fin >> output_len;
		else if (command == ".p")
			fin >> row;
		else if (command == ".s")
			fin >> s;
		else if (command == ".r") {
			fin >> begin;
			for (int i = 0; i < s; i++) {
				string before, after, input, output;
				node t;
				t.state.resize(int(pow(2, input_len)));
				fin >> input >> before >> after >> output;
				t.name = before;
				t.state[0].after = after;
				t.state[0].input = input;
				t.state[0].output = output;
				for (int j = 1; j < pow(2, input_len); j++)
				{
					fin >> input >> before >> after >> output;
					if (t.name == before) {
						t.state[j].after = after;
						t.state[j].input = input;
						t.state[j].output = output;
					}
				}
				list.push_back(t);
			}
		}
		else if (command == ".end_kiss") {
			dot_format(begin, list, "input.dot");
			reduce(list);
			kiss_format(input_len, output_len, begin, list, kiss_name);
			dot_format(begin, list, dot_name);
		}
	}
}

void reduce(vector<node>& list)
{
	bool flag = false;

	for (int i = 0; i < list.size(); i++) {
		for (int j = i + 1; j < list.size(); j++) {
			if (list[j].reduce)
				continue;
			bool check1 = true, check2 = true;
			// 第一種
			for (int k = 0; k < list[i].state.size(); k++){
				if (list[i].state[k].input == list[j].state[k].input &&
					list[i].state[k].after == list[j].state[k].after &&
					list[i].state[k].output == list[j].state[k].output)
					check1 = check1 && true;
				else
					check1 = check1 && false;
			}
			// 第二種
			int cross_index = -1;
			for (int k = 0; k < list[i].state.size(); k++) {
				if (list[i].state[k].input == list[j].state[k].input &&
					list[i].state[k].after == list[j].name &&
					list[i].name == list[j].state[k].after &&
					list[i].state[k].output == list[j].state[k].output)
				{
					cross_index = k;
					break;
				}
			}
			for (int k = 0; k < list[i].state.size(); k++) {
				if (k == cross_index)
					continue;
				if (list[i].state[k].input == list[j].state[k].input &&
					list[i].state[k].after == list[j].state[k].after &&
					list[i].state[k].output == list[j].state[k].output)
					check2 = check2 && true;
				else
					check2 = check2 && false;
			}
			if (check1 || check2) {
				flag = true;
				list[j].reduce = true;
				change(list[j].name, list[i].name, list);
			}
		}
	}
	if (flag)
		reduce(list);
}

void change(string target, string replace, vector<node>& list)
{
	for (int i = 0; i < list.size(); i++) {
		for (int j = 0; j < list[i].state.size(); j++) {
			if (list[i].state[j].after == target)
				list[i].state[j].after = replace;
		}
	}
}

void kiss_format(int input_len, int output_len, string begin, vector<node>& list, string filename)
{
	ofstream fout(filename);
	fout << ".start_kiss" << endl;
	fout << ".i " << input_len << endl;
	fout << ".o " << output_len << endl;
	int p = 0, s = 0;
	for (int i = 0; i < list.size(); i++) {
		if (!list[i].reduce) {
			s++;
			for (int j = 0; j < list[i].state.size(); j++)
				p++;
		}
	}

	fout << ".p " << p << endl;
	fout << ".s " << s << endl;
	fout << ".r " << begin << endl;
	for (int i = 0; i < list.size(); i++) {
		if (!list[i].reduce) {
			for (int j = 0; j < list[i].state.size(); j++) {
				fout << list[i].state[j].input << " " << list[i].name << " " << list[i].state[j].after << " " << list[i].state[j].output << endl;
			}
		}
	}
	fout << ".end_kiss" << endl;
	fout.close();
}

void dot_format(string begin, vector<node>& list, string filename)
{
	ofstream fout(filename);
	fout << "digraph STG {\n\trankdir = LR;\n\n\tINIT [shape=point];" << endl;
	for (int i = 0; i < list.size(); i++) {
		if (!list[i].reduce)
			fout << "\t" << list[i].name << " [label=\"" << list[i].name << "\"];" << endl;
	}

	fout << "\n\tINIT -> " << begin << ";" << endl;

	for (int i = 0; i < list.size(); i++) {
		if (!list[i].reduce) {
			vector<bool> check(list[i].state.size());
			for (int j = 0; j < check.size(); j++)
				check[j] = false;

			for (int j = 0; j < list[i].state.size(); j++) {
				if (check[j])
					continue;

				fout << "\t" << list[i].name << " -> " << list[i].state[j].after << " [label=\"" << list[i].state[j].input << "/" << list[i].state[j].output;
				for (int k = j + 1; k < list[i].state.size(); k++) {
					if (list[i].state[k].after == list[i].state[j].after) {
						check[k] = true;
						fout << "," << list[i].state[k].input << "/" << list[i].state[k].output;
					}
				}
				fout << "\"];" << endl;
			}
		}
	}
	fout << "}" << endl;
	fout.close();
}
