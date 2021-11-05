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
		int depth = -1;			// ����
		while (curNode != NULL) {
			depth++;
			// ���� ����� id�� ã�� id�� ���� ���
			if (curNode->id == input_id) {
				cout << depth << ' ';
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

	int insert(Node* node) {
		// Ʈ���� ����ִٸ�
		if (root == NULL) {
			root = node;
			return 0;
		}
		
		Node* parNode = NULL;
		Node* curNode = root;
		int depth = 0;

		while (curNode != NULL) {
			depth++;
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
		return depth;
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
		cout << '\n';
	}

	void discount(int x, int y, int p) {

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
		// ��� (insert)
		if (query == "I") {
			cin >> id >> name >> file_size >> price;
			Node* newNode = new Node(id, name, file_size, price);

			Node* node = tree.search(id);
			
			// ���� ��� �߰�
			if (node == NULL) {
				cout << tree.insert(newNode);
			}
			cout << '\n';
		}
		// �˻� (search)
		else if (query == "F") {
			cin >> id;
			Node* node = tree.search(id);

			if (node == NULL) {
				cout << "NULL" << '\n';
			}
			else {
				cout << node->name << ' ' << node->file_size << ' ' << node->price << '\n';
			}
		}
		// ������Ʈ (update)
		else if (query == "R") {
			cin >> id >> name >> file_size >> price;
			tree.update(id, name, file_size, price);
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