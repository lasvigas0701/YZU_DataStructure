#include <iostream>
#include <string>
using namespace std;

int main() {
	int n, count = 0;
	cin >> n;
	string str;
	cin >> str;
	string ori(str);
	do
	{
		str.insert(str.begin(), str.back());
		str.erase(str.end() - 1);
		count++;
		//cout << str << endl;
	} while (str != ori);
	cout << count << endl;
}