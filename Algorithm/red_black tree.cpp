#include <iostream>
#include <string>
#include <vector>
#define RED 1
#define BLACK 0
using namespace std;

// 노드 -> id, 이름, 용량, 가격, 컬러, left_child, right_child, parent 저장
class Node {
public:
	int id;
	string name;
	int file_size;
	int price;
	int color;
	Node* parent;		// 부모 링크
	Node* left_child;	// 왼쪽 자식 링크
	Node* right_child;	// 오른쪽 자식 링크

	// 생성자
	Node(int i, string n, int fs, int p) {
		id = i;
		name = n;
		file_size = fs;
		price = p;
		color = RED;
		parent = left_child = right_child = NULL;
	}

	// 왼쪽 자식 설정
	void setLeft(Node* node) {
		if (node == NULL) {
			this->left_child = NULL;
		}
		else {
			this->left_child = node;
			node->parent = this;
		}
	}

	// 오른쪽 자식 설정
	void setRight(Node* node) {
		if (node == NULL) {
			this->right_child = NULL;
		}
		else {
			this->right_child = node;
			node->parent = this;
		}
	}

	// 노드 색 변경
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

	// 생성자
	RedBlackTree() {
		root = NULL;
	}

	// 검색
	Node* search(int input_id) {
		Node* curNode = root;
		//int depth = -1;			// 깊이
		while (curNode != NULL) {
			//depth++;
			// 현재 노드의 id와 찾는 id가 같을 경우
			if (curNode->id == input_id) {
				//cout << depth << ' ';
				return curNode;
			}
			// 현재 노드의 id보다 찾는 id가 클 경우 -> 오른쪽으로 이동
			else if (curNode->id < input_id) {
				curNode = curNode->right_child;
			}
			// 현재 노드의 id보다 찾는 id가 작을경우 -> 왼쪽으로 이동
			else {
				curNode = curNode->left_child;
			}
		}
		// 없으면 NULL리턴
		return NULL;
	}

	void insert(Node* node) {
		// 트리가 비어있다면
		if (root == NULL) {
			root = node;

			// 루트면 노드 색 -> black
			node->color = BLACK;
			//return 0;
			return;
		}
		
		Node* parNode = NULL;
		Node* curNode = root;
		//int depth = 0;

		while (curNode != NULL) {
			//depth++;
			// 새로 입력하는 id가 더 큰 경우
			if (curNode->id < node->id) {
				parNode = curNode;
				curNode = curNode->right_child;
			}
			// 새로 입력하는 id가 더 작은 경우
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

		// 업데이트 하려는 노드가 없을 경우
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

		// 조부모, 부모, 삼촌 색 변경
		grandParent->colorChange();
		parent->colorChange();
		sibling->colorChange();

		// 조부모가 루트면 다시 black으로 변경
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

		// id 오름차순 정렬
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				if (v[j]->id > v[j + 1]->id) {
					Node* tmp = v[j + 1];
					v[j + 1] = v[j];
					v[j] = tmp;
				}
			}
		}

		// 생김새 파악
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

	// depth 리턴
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
		// 등록 (insert) -> while(doublered) -> 시블링 확인, 리컬러링, 리스트럭처링
		if (query == "I") {
			cin >> id >> name >> file_size >> price;
			Node* newNode = new Node(id, name, file_size, price);

			Node* node = tree.search(id);
			
			// 없을 경우 추가
			if (node == NULL) {
				tree.insert(newNode);

				// while(doubleRed(), node->parent != NULL)
				// if 부모의 sibling이 black 이면 리스트럭쳐링 -> return
				// if 부모의 sibling이 red 이면 리컬러링
			}

			// 없을 경우 추가하고 depth출력, 있을 경우 depth만 출력
			cout << tree.findDepth(id) << '\n';

		}
		// 검색 (search)
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
		// 업데이트 (update)
		else if (query == "R") {
			cin >> id >> name >> file_size >> price;
			tree.update(id, name, file_size, price);

			cout << tree.findDepth(id) << '\n';
		}
		// 할인 (discount)
		else if (query == "D") {
			int x, y, p;
			cin >> x >> y >> p;
			tree.discount(x, y, p);
		}
	}
	return 0;
}