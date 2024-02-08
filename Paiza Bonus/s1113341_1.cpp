#include <iostream>
using namespace std;

int main() {
	int n, m, a, x = 0;
	int num = 0;
	cin >> n >> m >> a;
	for (int i = 0; i < n; i++)
	{
		cin >> x;
		if (x <= a)
		{
			int buf = m / x;
			num += buf;
			m -= x * buf;
		}
		else if (x > a)
		{
			m += num * x;
			num = 0;
		}
		//cout << num << ' ' << x << ' ' << m << endl;
	}
	cout << num * x + m << endl;
}