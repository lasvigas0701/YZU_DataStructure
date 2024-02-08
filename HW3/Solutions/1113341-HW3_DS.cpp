#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Tree {
public:
	Tree* parentNode = nullptr;
	string element;
	int index = 0;
	Tree* leftNode = nullptr;
	Tree* rightNode = nullptr;
	
	Tree* insert(Tree* parent, string where, string value)
	{
		Tree* newNode = new Tree;
		if (where == "left")
		{
			newNode->element = value;
			newNode->parentNode = parent;
			newNode->index = parent->index * 2;
			parent->leftNode = newNode;
		}
		else /*where == "right"*/
		{
			newNode->element = value;
			newNode->parentNode = parent;
			newNode->index = parent->index * 2 + 1;
			parent->rightNode = newNode;
		}
		return newNode;
	}
};

/*inorder:1 preorder:2 postorder:3*/
int orderNum(string orderType)
{
	if (orderType == "Inorder")
		return 1;
	else if (orderType == "Preorder")
		return 2;
	else return 3;
}

/*store every element from input to each index of vector*/
void token(vector<string>& elements, string line)
{
	size_t elementStart = line.find('(', 0), elementEnd = line.find(')', 0);
	while (elementStart != string::npos)
	{
		string str = line.substr(elementStart + 1, elementEnd - elementStart - 1);
		elements.push_back(str);
		elementStart = line.find('(', elementStart + 1);
		elementEnd = line.find(')', elementEnd + 1);
	}
}

int findPos(string target, vector<string> sourceLine)
{
	for (int i = 0; i < sourceLine.size(); i++)
		if (sourceLine[i] == target)
			return i;
	return 0;
}

/*build tree by two order*/
void buildTree(vector<string> inorder, vector<string> anotherOrder, int orderType,
	vector<pair<string, int>>& treeArray)
{
	int total = inorder.size();
	Tree tree;
	if (orderType == 2) /*Preorder*/
	{
		int count = 0;
		tree.element = anotherOrder[count];
		tree.index = 1;
		int rootIn = findPos(anotherOrder[count], inorder);
		Tree* root = &tree;
		Tree* nowRoot = &tree; /*record the parent of the new node*/
		int nowNodeIn = findPos(nowRoot->element, inorder), prevNodeIn = findPos(nowRoot->element, inorder);
		treeArray.push_back(pair<string, int>(nowRoot->element, nowRoot->index));
		count++;
		while (count != total) /*when count == total, the tree is established*/
		{
			Tree* move = root;
			int nextNodeIn = findPos(anotherOrder[count], inorder);
			for (int i = 0; i < count; i++)
			{
				if (nextNodeIn < findPos(move->element, inorder))
				{
					if (!move->leftNode)
					{
						nowRoot = tree.insert(move, "left", anotherOrder[count]);
						break;
					}
					move = move->leftNode;
				}
				else
				{
					if (!move->rightNode)
					{
						nowRoot = tree.insert(move, "right", anotherOrder[count]);
						break;
					}
					move = move->rightNode;
				}
			}
			
			treeArray.push_back(pair<string, int>(nowRoot->element, nowRoot->index));
			count++;
		}
	}
	else
	{
		int count = total - 1;
		tree.element = anotherOrder[count];
		tree.index = 1;
		int rootIn = findPos(anotherOrder[count], inorder);
		Tree* root = &tree;
		Tree* nowRoot = &tree; /*record the parent of the new node*/
		int nowNodeIn = findPos(nowRoot->element, inorder), prevNodeIn = findPos(nowRoot->element, inorder);
		treeArray.push_back(pair<string, int>(nowRoot->element, nowRoot->index));
		count--;
		while (count >= 0) /*when count < 0, the tree is established*/
		{
			Tree* move = root;
			int nextNodeIn = findPos(anotherOrder[count], inorder);
			for (int i = anotherOrder.size() - 1; i > count; i--)
			{
				if (nextNodeIn < findPos(move->element, inorder))
				{
					if (!move->leftNode)
					{
						nowRoot = tree.insert(move, "left", anotherOrder[count]);
						break;
					}
					move = move->leftNode;
				}
				else
				{
					if (!move->rightNode)
					{
						nowRoot = tree.insert(move, "right", anotherOrder[count]);
						break;
					}
					move = move->rightNode;
				}
			}

			treeArray.push_back(pair<string, int>(nowRoot->element, nowRoot->index));
			count--;
		}
	}
}

int main(int argc, char** argv) {
	fstream inFile(argv[1], ios::in);
	if (!inFile)
		cout << "Fail to open " << argv[1], exit(-1);
	string line1, line2;
	while (getline(inFile, line1, '\n'))
	{
		vector<string> first, second;
		token(first, line1);
		getline(inFile, line2, '\n');
		token(second, line2);
		
		int order1 = orderNum(line1.substr(0, line1.find(':')));
		int order2 = orderNum(line2.substr(0, line2.find(':')));
		vector<pair<string, int>> tree;
		if (order1 == 1)
			buildTree(first, second, order2, tree); /*1:inorder 2:postorder/preorder*/
		else
			buildTree(second, first, order1, tree); /*2:inorder 1:postorder/preorder*/
		sort(tree.begin(), tree.end(),
			[](const auto& x, const auto& y) {return x.second < y.second; });

		for (int i = 0; i < tree.size(); i++)
			cout << tree[i].first << ':' << tree[i].second << endl;
	}
}