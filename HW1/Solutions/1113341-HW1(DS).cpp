#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

int isp(char x) /*in stack*/
{
	switch (x)
	{
	case '(':
		return 8;
	case '+':
	case '-':
		return 3;
	case '*':
	case '/':
		return 2;
	case '^':
		return 1;
	default:
		return 9;
	}
}

int icp(char x) /*in expression*/
{
	switch (x)
	{
	case '(':
		return 0;
	case '+':
	case '-':
		return 3;
	case '*':
	case '/':
		return 2;
	case '^':
		return 1;
	default:
		return 0;
	}
}

void toPost(vector<string>* post, string inorder)
{
	stack<char> operators;
	string charToString;
	bool neg = false, left = false; /*neg means negative; left means the '-' is on the left of '('*/
	string prev = ""; /*to distinguish subtraction and negative*/
	for (size_t i = 0; i < inorder.size(); i++)
	{
		size_t count = 0, temp = i;
		while (isdigit(inorder[i]))
			count++, i++;

		if (count && !neg) /*token is positive number*/
			post->push_back(inorder.substr(temp, count)), prev = inorder.substr(temp, count), i--;
		else if (count && neg) /*token is negative number*/
		{
			string str = inorder.substr(temp, count); 
			/*if the number is -(a+b), then let it be "-a -b +" in vector<string>* post*/
			str.insert(str.begin(), '-');
			post->push_back(str), prev = str, i--;

			/*if !left means only one number should be negative.
			  eg(1). left: -(a+b) = -a + -b
			  eg(2). !left: (-a+b) = -a + b */
			if (!left) neg = false;
		}
		else if (inorder[i] == ')') /*token is ')', store operators in () to vector<string>* post */
		{
			prev = inorder[i];
			for (; operators.top() != '('; operators.pop())
				charToString = operators.top(), post->push_back(charToString);
			operators.pop(); /*unstack '('*/
			left = false;
		}
		else if (inorder[i] == ' ' || inorder[i] == '[') /*token is ' ' or '[', ignore*/
			continue;
		else
		{
			if (prev == "" && inorder[i] == '-') /*if prev is None, '-' means minus instead of negative*/
			{
				neg = true;
				continue;
			}
			else if (neg == true && inorder[i] == '(')
				left = true;

			for (; !operators.empty() && isp(operators.top()) <= icp(inorder[i]); operators.pop())
				charToString = operators.top(), post->push_back(charToString);
			operators.push(inorder[i]);
			prev = ""; /*set prev to None as long as present token is not cases mentioned above*/
		}
	}

	for (; !operators.empty(); operators.pop())
		charToString = operators.top(), post->push_back(charToString);
}

double processElements(vector<string> post)
{
	double result = 0, a = 0, b = 0;
	stack<double> nums;
	for (size_t i = 0; i < post.size(); i++)
	{
		if (post[i] == "+")
			a = nums.top(), nums.pop(), b = nums.top(), nums.pop(), nums.push(b + a);
		else if (post[i] == "-")
			a = nums.top(), nums.pop(), b = nums.top(), nums.pop(), nums.push(b - a);
		else if (post[i] == "*")
			a = nums.top(), nums.pop(), b = nums.top(), nums.pop(), nums.push(b * a);
		else if (post[i] == "/")
			a = nums.top(), nums.pop(), b = nums.top(), nums.pop(), nums.push(b / a);
		else if (post[i] == "^")
			a = nums.top(), nums.pop(), b = nums.top(), nums.pop(), nums.push(pow(b, a));
		else
			nums.push(stod(post[i]));
	}
	return (result = nums.top());
}

vector<double> compResult(vector<double> left, vector<double> right, char op)
{
	size_t bigger = max(left.size(), right.size());
	vector<double> result(bigger);
	
	if (op == '+')
		for (size_t i = 0, j = 0, k = 0; k < bigger; i++, j++, k++)
			result[k] = left[i % left.size()] + right[j % right.size()];
	else if (op == '-')
		for (size_t i = 0, j = 0, k = 0; k < bigger; i++, j++, k++)
			result[k] = left[i % left.size()] - right[j % right.size()];
	else if (op == '*')
		for (size_t i = 0, j = 0, k = 0; k < bigger; i++, j++, k++)
			result[k] = left[i % left.size()] * right[j % right.size()];
	else if (op == '/')
		for (size_t i = 0, j = 0, k = 0; k < bigger; i++, j++, k++)
			result[k] = left[i % left.size()] / right[j % right.size()];

	return result;
}

int main(int argc, char* argv[]) {
	ifstream input(argv[1], ios::in);
	if (!input)
		cerr << "Fail to open " << argv[1] << endl, exit(3);

	vector <vector<double>> valuesInVec; /*valuesInVec[i][j] is the jth processed-element in the ith vector*/
	vector <double> answer;
	string line;
	while(getline(input, line, '\n')) /*read one line (one test case)*/
	{
		vector<char> vecOperator; /*operators between two vectors*/
		size_t startOfVec = line.find('['), endOfVec = 0, vectorOp = 0;
		string buf = line;

		/*read each vector in a case*/
		while ((endOfVec = line.find(']')) != string::npos)
		{
			string vec = line.substr(startOfVec, endOfVec + 1);
			double number = 0;
			stack<char> opForVecs; /*store operators between vectors and values in vectors*/
			size_t endOfEle = 0;
			vector<double> elements; /*store processed-elements in a vactor*/

			/*read each element in one vector*/
			while ((endOfEle = vec.find_first_of(",]")) != string::npos)
			{
				size_t firstElement = vec.find_first_of("(1234567890-");
				string str2 = vec.substr(firstElement, endOfEle - firstElement);
				vector<string> post;

				toPost(&post, str2); /*let infix notation be postfix*/
				double num = processElements(post); /*compute the final result of every element in vector*/
				elements.push_back(num); /*store the elements*/
				
				vec.erase(0, endOfEle + 1);
			}
			valuesInVec.push_back(elements); /*each index is a vector that consist of some processed elements*/

			line.erase(0, endOfVec + 1);
			vectorOp = line.find_first_of("+-*/");

			if (vectorOp != string::npos)
				vecOperator.push_back(line[vectorOp]);
			startOfVec = line.find('[');
		}

		while (valuesInVec.size() > 1)
		{
			bool does = false;

			for (size_t i = 0; i < vecOperator.size(); i++)
			{
				if (vecOperator[i] == '*')
					answer = compResult(valuesInVec[i], valuesInVec[i + 1], '*'), does = true;
				else if (vecOperator[i] == '/')
					answer = compResult(valuesInVec[i], valuesInVec[i + 1], '/'), does = true;
				if (does)
				{
					valuesInVec.erase(valuesInVec.begin() + i); /*erase the element before operator*/
					valuesInVec.erase(valuesInVec.begin() + i); /*erase the element after operator, (i is the same because it has been moved after erase)*/
					valuesInVec.insert(valuesInVec.begin() + i, answer);
					vecOperator.erase(vecOperator.begin() + i);
					i--;
					does = false;
				}
			}

			for (size_t i = 0; i < vecOperator.size(); i++)
			{
				if (vecOperator[i] == '+')
					answer = compResult(valuesInVec[i], valuesInVec[i + 1], '+'), does = true;
				else if (vecOperator[i] == '-')
					answer = compResult(valuesInVec[i], valuesInVec[i + 1], '-'), does = true;
				if (does)
				{
					valuesInVec.erase(valuesInVec.begin() + i); 
					valuesInVec.erase(valuesInVec.begin() + i); 
					valuesInVec.insert(valuesInVec.begin() + i, answer);
					vecOperator.erase(vecOperator.begin() + i);
					i--;
					does = false;
				}
			}
		}
		
		cout << setprecision(9) << '[' << round(answer[0] * 100) / 100;
		for (size_t i = 1; i < answer.size(); i++)
			cout << ',' << round((answer[i]) * 100) / 100;
		cout << ']' << endl;
		valuesInVec.clear(); /*do next test case so valuesInVec need to be cleared*/
	}
}