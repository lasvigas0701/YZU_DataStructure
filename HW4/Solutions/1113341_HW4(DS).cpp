#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

vector<int> available, availableConst;

struct Process {
	vector<int> allocate;
	vector<int> max;
	vector<int> need;
};

void addAvailable(vector<int> newAvailable)
{
	for(int i = 0; i < available.size(); i++)
		available[i] += newAvailable[i];
}

vector<int> extraValue(string cmd)
{
	vector<int> value;
	size_t pos = cmd.find(',');
	while (pos != string::npos)
	{
		string num = cmd.substr(0, pos);
		value.push_back(stoi(num));
		cmd.erase(0, pos + 1);
		pos = cmd.find(',');
	}
	value.push_back(stoi(cmd));
	return value;
}

bool safeForProcess(vector<int> need, vector<int> temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		if (need[i] > temp[i] || need[i] < 0)
			return false;
	}
	return true;
}

bool isSafe(map<string, Process> p, string pid, vector<int> temp, vector<int> temp2)
{
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] < 0)
			return false;
	}
	for (auto x : p)
	{
		if (x.first == pid && safeForProcess(temp2, temp)) return true;
		else if (safeForProcess(x.second.need, temp)) return true;
	}
	return false;
}

bool canOut(Process p)
{
	for (int i = 0; i < available.size(); i++)
	{
		if (p.need[i] > available[i])
			return false;
	}
	return true;
}

void request(map<string, Process>& processes, string p, vector<int> value)
{
	if (!processes.count(p)) return;
	for (int i = 0; i < available.size(); i++)
		if (processes[p].need[i] < value[i]) return;

	map<string, bool> safe;
	vector<int> temp(available), temp2(processes[p].need);

	for (int i = 0; i < available.size(); i++)
	{
		temp[i] -= value[i];
		temp2[i] -= value[i];
	}

	int safeCount = 0;
	vector<int> tempBuf(temp);

	for (auto it = processes.begin(); it != processes.end(); it++)
	{
		if (isSafe(processes, (*it).first, tempBuf, temp2) && !safe[(*it).first])
		{
			safe[(*it).first] = true;
			safeCount++;
			for (int j = 0; j < available.size(); j++)
				tempBuf[j] += processes[(*it).first].allocate[j];
			it = processes.begin();
		}
	}

	if (safeCount == processes.size())
	{
		for (int i = 0; i < available.size(); i++)
		{
			processes[p].need[i] -= value[i];
		}
		for (int i = 0; i < available.size(); i++)
		{
			processes[p].allocate[i] += value[i];
		}
		available = temp;
	}
}

int main(int argc, char**argv) {
	fstream inFile(argv[1], ios::in);
	string first;
	getline(inFile, first, '\n');
	available = extraValue(first);
	availableConst = available;
	string cmd;
	map<string, Process> processes;
	while (getline(inFile, cmd, '\n'))
	{
		string p = cmd.substr(0, cmd.find(' ')), str;
		if (processes.count(p))
		{
			cmd.erase(0, cmd.find(' ') + 1);
			str = cmd.substr(0, cmd.find(' '));
			if (str == "request")
			{
				cmd.erase(0, cmd.find(' ') + 1);
				vector<int> requestValue = extraValue(cmd);
				request(processes, p, requestValue);
			}
			else if (str == "want")
			{
				cmd.erase(0, cmd.find(' ') + 1); /*erase "want"*/
				cmd.erase(0, cmd.find(' ') + 1); /*erase "out"*/
				if (canOut(processes[p]))
				{
					addAvailable(processes[p].allocate);
					processes.erase(p);
				}
			}
		}
		else
		{
			cmd.erase(0, cmd.find(' ') + 1);
			str = cmd.substr(0, cmd.find(' '));
			if (str == "in")
			{
				cmd.erase(0, cmd.find(' '));
				vector<int> init = extraValue(cmd);
				bool can = true;
				for(int i = 0; i < available.size(); i++)
					if (init[i] > availableConst[i])
					{
						can = false;
						break;
					}
				if (can)
				{
					processes[p].allocate.resize(available.size());
					fill(processes[p].allocate.begin(), processes[p].allocate.end(), 0);
					processes[p].max = init;
					processes[p].need = init;
				}
			}
		}
	}

	for (auto x : processes)
	{
		cout << x.first << ' ';
		cout << x.second.allocate[0];
		for (int i = 1; i < available.size(); i++)
			cout << ',' << x.second.allocate[i];
		cout << endl;
	}
}