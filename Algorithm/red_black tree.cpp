#include <iostream>
#include <string>
#include <vector>
#define RED 1
#define BLACK 0
using namespace std;

// ��� -> id, �̸�, �뷮, ����, �÷�, left_child, right_child, parent ����
class Node {
public:
	int id;
	string name;
	int file_size;
	int price;
	int color;
	Node* parent;		// �θ� ��ũ
	Node* left_child;	// ���� �ڽ� ��ũ
	Node* right_child;	// ������ �ڽ� ��ũ

	// ������
	Node(int i, string n, int fs, int p) {
		id = i;
		name = n;
		file_size = fs;
		price = p;
		color = RED;
		parent = left_child = right_child = NULL;
	}

	// ���� �ڽ� ����
	void setLeft(Node* node) {
		if (node == NULL) {
			this->left_child = NULL;
		}
		else {
			this->left_child = node;
			node->parent = this;
		}
	}

	// ������ �ڽ� ����
	void setRight(Node* node) {
		if (node == NULL) {
			this->right_child = NULL;
		}
		else {
			this->right_child = node;
			node->parent = this;
		}
	}

	// ��� �� ����
	void colorChange() {
		if (this->color == RED) {
			this->color = BLACK;
		}
		else {
			this->color = RED;
		}
	}

	friend class RedBlackTree;
};

class RedBlackTree {
public:
	Node* root;

	// ������
	RedBlackTree() {
		root = NULL;
	}

	// �˻�
	Node* search(int input_id) {
		Node* curNode = root;
		//int depth = -1;			// ����
		while (curNode != NULL) {
			//depth++;
			// ���� ����� id�� ã�� id�� ���� ���
			if (curNode->id == input_id) {
				//cout << depth << ' ';
				return curNode;
			}
			// ���� ����� id���� ã�� id�� Ŭ ��� -> ���������� �̵�
			else if (curNode->id < input_id) {
				curNode = curNode->right_child;
			}
			// ���� ����� id���� ã�� id�� ������� -> �������� �̵�
			else {
				curNode = curNode->left_child;
			}
		}
		// ������ NULL����
		return NULL;
	}

	void insert(Node* node) {
		// Ʈ���� ����ִٸ�
		if (root == NULL) {
			root = node;

			// ��Ʈ�� ��� �� -> black
			node->color = BLACK;
			//return 0;
			return;
		}
		
		Node* parNode = NULL;
		Node* curNode = root;
		//int depth = 0;

		while (curNode != NULL) {
			//depth++;
			// ���� �Է��ϴ� id�� �� ū ���
			if (curNode->id < node->id) {
				parNode = curNode;
				curNode = curNode->right_child;
			}
			// ���� �Է��ϴ� id�� �� ���� ���
			else {
				parNode = curNode;
				curNode = curNode->left_child;
			}
		}

		if (parNode->id > node->id) {
			parNode->setLeft(node);
		}
		else if (parNode->id < node->id) {
			parNode->setRight(node);
		}

		//return depth;
		return;
	}

	void update(int id, string name, int file_size, int price) {
		Node* node = search(id);

		// ������Ʈ �Ϸ��� ��尡 ���� ���
		if (node == NULL) {
			cout << "NULL";
		}
		
		else {
			node->name = name;
			node->file_size = file_size;
			node->price = price;
		}
	}

	void discount(int x, int y, int p) {

	}

	void reColoring(Node* child, Node* parent) {
		Node* grandParent = parent->parent;
		Node* sibling = NULL;

		if (grandParent->right_child == parent) {
			sibling = grandParent->left_child;
		}
		else {
			sibling = grandParent->right_child;
		}

		// ���θ�, �θ�, ���� �� ����
		grandParent->colorChange();
		parent->colorChange();
		sibling->colorChange();

		// ���θ� ��Ʈ�� �ٽ� black���� ����
		if (grandParent == root) {
			grandParent->colorChange();
		}
	}

	void reStructuring(Node* child, Node* parent) {
		Node* grandParent = parent->parent;
		Node* tmp;
		vector<Node*> v;
		v.push_back(grandParent);
		v.push_back(parent);
		v.push_back(child);

		// id �������� ����
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				if (v[j]->id > v[j + 1]->id) {
					Node* tmp = v[j + 1];
					v[j + 1] = v[j];
					v[j] = tmp;
				}
			}
		}

		// ����� �ľ�
		if (grandParent->left_child == parent) {
			if (parent->left_child == child) {	//		/'
												//	   /'

			}
			else {								//		/'
												//		\'

			}
		}
		else {									//		\'
			if (parent->left_child == child) {	//		/

			}
			else {								//		\'
												//		 \'

			}

		}
	}
	// double red check
	bool checkDoubleRed(Node* node) {
		if (node->parent->color == RED) {
			return true;
		}
		return false;
	}

	// depth ����
	int findDepth(int id) {
		Node* curNode = root;
		int depth = -1;

		while (curNode != NULL) {
			depth++;
			if (curNode->id == id) {
				return depth;
			}
			else if (curNode->id < id) {
				curNode = curNode->right_child;
			}
			else {
				curNode = curNode->left_child;
			}
		}
	}
};

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	RedBlackTree tree;
	int t, id, file_size, price;
	string name, query;

	cin >> t;
	while (t--) {
		cin >> query;
		// ��� (insert) -> while(doublered) -> �ú� Ȯ��, ���÷���, ����Ʈ��ó��
		if (query == "I") {
			cin >> id >> name >> file_size >> price;
			Node* newNode = new Node(id, name, file_size, price);

			Node* node = tree.search(id);
			
			// ���� ��� �߰�
			if (node == NULL) {
				tree.insert(newNode);

				// while(doubleRed(), node->parent != NULL)
				// if �θ��� sibling�� black �̸� ����Ʈ���ĸ� -> return
				// if �θ��� sibling�� red �̸� ���÷���
			}

			// ���� ��� �߰��ϰ� depth���, ���� ��� depth�� ���
			cout << tree.findDepth(id) << '\n';

		}
		// �˻� (search)
		else if (query == "F") {
			cin >> id;
			Node* node = tree.search(id);

			if (node == NULL) {
				cout << "NULL" << '\n';
			}
			else {
				cout << tree.findDepth(id) << ' ' << node->name << ' ' << node->file_size << ' ' << node->price << '\n';
			}
		}
		// ������Ʈ (update)
		else if (query == "R") {
			cin >> id >> name >> file_size >> price;
			tree.update(id, name, file_size, price);

			cout << tree.findDepth(id) << '\n';
		}
		// ���� (discount)
		else if (query == "D") {
			int x, y, p;
			cin >> x >> y >> p;
			tree.discount(x, y, p);
		}
	}
	return 0;
}