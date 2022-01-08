#include <iostream>
#include <list>
#include <iomanip>
#include <string>
using namespace std;

struct Node {
	int start;
	int end;
	bool occupied;
	int size() {
		return end - start;
	}
	void set(int start, int end, bool occupied = 0) {
		this->start = start;
		this->end = end;
		this->occupied = occupied;
	}
	void printNode() {
		cout << "start:" << start << " end:" << end << " size:" << this->size() << endl;
	}
	Node(int start, int end, bool occupied = 0) {
		this->start = start;
		this->end = end;
		this->occupied = occupied;
	}
	Node() {}
	bool operator < (Node& a)
	{
		return this->size() < a.size();
	}
	bool operator > (Node& a)
	{
		return this->size() > a.size();
	}

};

bool comStart(Node& n1, Node& n2)
{
	return n1.start < n2.start;
}

int last = 0;
bool requestFF(list<Node>*, int, bool); //采用首次适应算法
bool requestNF(list<Node>*, int, bool); //采用循环适应算法
bool requestBF(list<Node>*, int, bool); //采用最佳适应算法
bool release(list<Node>*, int);
void printList(list<Node>*);
void mergeList(list<Node>*);


//bool类型请求函数 如果请求成功会返回1
bool requestFF(list<Node>* l, int need, bool flag_add) {
	if (l->size() == 0) return 0;
	for (list<Node>::iterator iter = l->begin(); iter != l->end(); iter++) {
		if (iter->size() >= need && iter->occupied == 0) {
			//如果找到了合适的位置 则将原iter处一分为二，在前半部分放need，occupied置一
			if (flag_add == 1) {
				Node t_rest(iter->start, iter->start + need, 1); //采用首次适应算法 按顺序找到第一个可以用的
				list<Node>::iterator iter1;
				iter1 = l->insert(iter, t_rest);
				iter->start = iter->start + need;
				last = iter->start;
			}
			return 1;
		}
	}
	return 0;
}

bool requestNF(list<Node>* l, int need, bool flag_add)
{

	if (l->size() == 0) return 0;
	for (list<Node>::iterator iter = l->begin(); iter != l->end(); iter++) {
		if (iter->start == last)
		{

			if (iter->size() >= need && iter->occupied == 0) {
				//如果找到了合适的位置 则将原iter处一分为二，在前半部分放need，occupied置一
				Node t_rest(iter->start, iter->start + need, 1);
				list<Node>::iterator iter1;
				iter1 = l->insert(iter, t_rest);
				iter->start = iter->start + need;
				last = iter->start;
				return 1;
			}
			else {
				last = iter->end;
			}

		}
	}
	return 0;
}

bool requestBF(list<Node>* l, int need, bool flag_add)
{
	if (l->size() == 0) return 0;
	l->sort();
	for (list<Node>::iterator iter = l->begin(); iter != l->end(); iter++) {
		if (iter->size() >= need && iter->occupied == 0)
		{

			Node t_rest(iter->start, iter->start + need, 1);
			list<Node>::iterator iter1;
			iter1 = l->insert(iter, t_rest);
			iter->start = iter->start + need;
			last = iter->start;
			l->sort(comStart);
			return 1;
		}
	}
	return 0;
}

bool release(list<Node>* l, int index) {
	if (l->size() == 0) return 0;
	int i = 0;
	for (list<Node>::iterator iter = l->begin(); iter != l->end(); iter++) {
		if (i == index && iter->occupied == 1) {
			iter->occupied = 0;
			mergeList(l);
			return 1;
		}
		else i++;
	}
	return 0;
}

//将连续的空区域合并
void mergeList(list<Node>* l) {
	if (l->size() <= 1) return;
	list<Node>::iterator foot1 = l->begin();
	list<Node>::iterator foot2 = l->begin();
	++foot2; //foot2指向第二个
	while (true) {
		if (l->size() == 1) break;
		if (foot1->occupied == 0 && foot2->occupied == 0) {
			foot1->end += foot2->size();
			if (foot2 == l->end()) {
				l->pop_back();
				break;
			}
			else if (foot2 != l->end()) foot2 = l->erase(foot2);
			else break;
		}
		else {
			foot1++;
			foot2++;
		}
		if (foot2 == l->end()) break;
	}
}

//打印整个“内存”
void printList(list<Node>* l) {
	list<Node>::iterator iter = l->begin();
	for (int i = 0; i < l->size(); i++) {
		cout << "No." << i << "  /  ";
		cout << "start:" << setw(4) << iter->start << "  /  end:" << setw(4) << iter->end << "  /  used:" << iter->occupied << endl;
		iter++;
	}
}

int main() {
	/*list<Node> M;
	Node initialize_m(0, 1024);
	M.push_back(initialize_m);*/
	list<Node> M;
	Node init0(0, 5, 1), init1(5, 10, 1), init2(10, 14, 1), init3(14, 26, 0), init4(26, 32, 1), init5(32, 128, 0);
	M.push_back(init0);
	M.push_back(init1);
	M.push_back(init2);
	M.push_back(init3);
	M.push_back(init4);
	M.push_back(init5);
	cout << "//Memory Management System//" << endl;
	cout << "Memory size overall: 1024K" << endl;
	while (true) {
		cout << "///" << endl;
		cout << "PrintMemoryList:" << endl;
		printList(&M);
		cout << "Enter 1 to request memory(First Fit)..." << endl;
		cout << "Enter 2 to request memory(Next Fit)..." << endl;
		cout << "Enter 3 to request memory(Best Fit)..." << endl;
		cout << "Enter 4 to release memory..." << endl;
		cout << "Enter 0 to EXIT..." << endl;
		int i = 0;
		cin >> i;
		if (i == 0) {
			cout << "MMS EXIT with Code 0" << endl;
			break;
		}
		else if (i == 1) {
			cout << "Enter memory size you request..." << endl;
			int need;
			cin >> need;
			if (requestFF(&M, need, 1)) cout << "SUCCEEDED!" << endl;
			else cout << "FAILED!" << endl << endl;
		}
		else if (i == 2) {
			cout << "Enter memory size you request..." << endl;
			int need;
			cin >> need;
			if (requestNF(&M, need, 1)) cout << "SUCCEEDED!" << endl;
			else cout << "FAILED!" << endl << endl;
		}
		else if (i == 3) {
			cout << "Enter memory size you request..." << endl;
			int need;
			cin >> need;
			if (requestBF(&M, need, 1)) cout << "SUCCEEDED!" << endl;
			else cout << "FAILED!" << endl << endl;
		}
		else if (i == 4) {
			cout << "Enter memory index you want to release..." << endl;
			int index;
			cin >> index;
			if (release(&M, index)) cout << "SUCCEEDED!" << endl;
			else cout << "FAILED!" << endl << endl;
		}
		else {
			cout << "MMS EXIT with Code -1" << endl;
			break;
		}
		cout << "Press the ENTER key to continue..." << endl;
		getchar();
		getchar();
	}

}


