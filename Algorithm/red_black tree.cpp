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

	// 기본 생성자
	Node() {
		id = -1;
		name = "";
		file_size = 0;
		price = 0;
		color = BLACK;
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

		// 추가 후 doublie red 체크하면서 수정
		reColoring(node);
		return;
	}

	void update(int id, string name, int file_size, int price) {
		Node* node = search(id);

		// 업데이트 하려는 노드가 없을 경우
		if (node == NULL) {
			cout << "NULL" << '\n';
		}
		
		// 업데이트 후 깊이 출력
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
		// double red 발생할 때마다
		while (node->parent != NULL && node->parent->color == RED) {
			Node* sibling = NULL;

			if (node->parent == node->parent->parent->left_child) {
				sibling = node->parent->parent->right_child;

				// 삼촌이 RED면 리컬러링 -> 문제가 생기면 colorChange를 BLACK으로 직접 바꿔주자.
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

				// 리컬러링 -> 지속
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
		// 루트의 색은 블랙으로 고정
		root->color = BLACK;
	}

	// 트리를 왼쪽으로 회전
	void rotateLeft(Node* node) {
		// 루트 노드라면
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

	// 트리를 오른쪽으로 회전
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