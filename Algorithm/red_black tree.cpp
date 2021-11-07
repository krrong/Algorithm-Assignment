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

	// �⺻ ������
	Node() {
		id = -1;
		name = "";
		file_size = 0;
		price = 0;
		color = BLACK;
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

		// �߰� �� doublie red üũ�ϸ鼭 ����
		reColoring(node);
		return;
	}

	void update(int id, string name, int file_size, int price) {
		Node* node = search(id);

		// ������Ʈ �Ϸ��� ��尡 ���� ���
		if (node == NULL) {
			cout << "NULL" << '\n';
		}
		
		// ������Ʈ �� ���� ���
		else {
			node->name = name;
			node->file_size = file_size;
			node->price = price;
			cout<<findDepth(id)<<'\n';
		}
	}

	void discount(int x, int y, int p) {
		Node* curNode = root;

		while (curNode != NULL && curNode->id > x) {

		}
	}

	void reColoring(Node* node) {
		// double red �߻��� ������
		while (node->parent != NULL && node->parent->color == RED) {
			Node* sibling = NULL;

			if (node->parent == node->parent->parent->left_child) {
				sibling = node->parent->parent->right_child;

				// ������ RED�� ���÷��� -> ������ ����� colorChange�� BLACK���� ���� �ٲ�����.
				if (sibling != NULL && sibling->color == RED) {
					node->parent->colorChange();
					sibling->colorChange();
					node->parent->parent->colorChange();
					node = node->parent->parent;
					continue;
				}

				// restructuring -> stop
				if (node == node->parent->right_child) {
					node = node->parent;

					rotateLeft(node);
				}

				node->parent->colorChange();
				node->parent->parent->colorChange();

				rotateRight(node->parent->parent);
				break;
			}
			else {
				sibling = node->parent->parent->left_child;

				// ���÷��� -> ����
				if (sibling != NULL && sibling->color == RED) {
					node->parent->colorChange();
					sibling->colorChange();
					node->parent->parent->colorChange();
					node = node->parent->parent;
					continue;
				}

				// restructuring -> stop
				if (node == node->parent->left_child) {
					node = node->parent;

					rotateRight(node);
				}

				node->parent->colorChange();
				node->parent->parent->colorChange();

				rotateLeft(node->parent->parent);
				break;
			}
		}
		// ��Ʈ�� ���� ������ ����
		root->color = BLACK;
	}

	// Ʈ���� �������� ȸ��
	void rotateLeft(Node* node) {
		// ��Ʈ �����
		if (node->parent == NULL) {
			Node* right = root->right_child;
			root->right_child = root->right_child->left_child;
			right->left_child = new Node();
			right->left_child->parent = root;
			root->parent = right;
			right->left_child = root;
			right->parent = NULL;
			root = right;
		}
		else {
			if (node == node->parent->left_child) {
				node->parent->left_child = node->right_child;
			}
			else {
				node->parent->right_child = node->right_child;
			}

			node->right_child->parent = node->parent;
			node->parent = node->right_child;

			if (node->right_child->left_child != NULL) {
				node->right_child->left_child->parent = node;
			}

			node->right_child = node->right_child->left_child;
			node->parent->left_child = node;
		}
	}

	// Ʈ���� ���������� ȸ��
	void rotateRight(Node* node) {
		if (node->parent == NULL) {
			Node* left = root->left_child;
			root->left_child = root->left_child->right_child;
			left->right_child = new Node();
			left->right_child->parent = root;
			root->parent = left;
			left->right_child = root;
			left->parent = NULL;
			root = left;
		}
		else {
			if (node == node->parent->left_child) {
				node->parent->left_child = node->left_child;
			}
			else {
				node->parent->right_child = node->left_child;
			}

			node->left_child->parent = node->parent;
			node->parent = node->left_child;

			if (node->left_child->right_child != NULL) {
				node->left_child->right_child->parent = node;
			}

			node->left_child = node->left_child->right_child;
			node->parent->right_child = node;
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