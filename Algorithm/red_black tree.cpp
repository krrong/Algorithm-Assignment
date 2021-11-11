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

	friend class RedBlackTree;
};

// leaf 노드

class RedBlackTree {
public:
	Node* root;
	Node* NIL;

	// 생성자
	RedBlackTree() {
		NIL = new Node();
		root = NIL;
	}

	// 검색
	Node* search(int input_id) {
		Node* curNode = root;
		while (curNode != NIL) {
			// 현재 노드의 id와 찾는 id가 같을 경우
			if (curNode->id == input_id) {
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
		node->parent = NIL;
		node->left_child = NIL;
		node->right_child = NIL;

		// 트리가 비어있다면
		if (root == NIL) {
			root = node;

			// 루트면 노드 색 -> black
			node->color = BLACK;
			return;
		}

		/*
		Node* parNode = NULL;
		Node* curNode = root;

		while (curNode != NIL) {
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
		*/

		Node* parent = root;
		while (true) {
			// 부모보다 더 크면 우측으로 이동
			if (parent->id < node->id) {
				// 만약 left노드라면 해당 위치에 삽입
				if (parent->right_child == NIL) {
					parent->right_child = node;
					node->parent = parent;
					break;
				}
				else {
					parent = parent->right_child;
				}
			}
			// 부모보다 더 작으면 좌측으로 이동
			else {
				// 만약 left노드라면 해당 위치에 삽입
				if (parent->left_child == NIL) {
					parent->left_child = node;
					node->parent = parent;
					break;
				}
				else {
					parent = parent->left_child;
				}
			}
		}

		// 추가 후 double red 체크하면서 수정
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
			cout << findDepth(id) << '\n';
		}
	}

	// inorder traversal
	void discount(Node* node, int x, int y, int p) {
		if (node == NIL) {
			return;
		}

		discount(node->left_child, x, y, p);

		// 범위 안에 존재하면 할인
		if (node->id >= x && node->id <= y) {
			node->price = node->price * (100 - p) / 100;
		}

		discount(node->right_child, x, y, p);
	}

	void reColoring(Node* node) {
		// double red 체크
		while (node->parent != NIL && node->parent->color == RED) {
			Node* sibling = NIL;

			// 부모가 조부모의 왼쪽 자식일 경우
			if (node->parent == node->parent->parent->left_child) {
				// 삼촌 노드
				sibling = node->parent->parent->right_child;

				// 삼촌이 RED면 recoloring
				if (sibling != NIL && sibling->color == RED) {
					node->parent->color = BLACK;
					sibling->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
					continue;
				}

				// 삼촌이 RED가 아니라면 restructuring -> double red stop
				// 현재 노드가 부모의 오른쪽 자식이라면
				if (node == node->parent->right_child) {
					node = node->parent;

					// 왼쪽으로 회전
					rotateLeft(node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;

				rotateRight(node->parent->parent);
				break;
			}
			// 부모가 조부모의 오른쪽 자식일 경우
			else {
				// 삼촌 노드
				sibling = node->parent->parent->left_child;

				// 삼촌이 RED면 recoloring
				if (sibling != NIL && sibling->color == RED) {
					node->parent->color = BLACK;
					sibling->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
					continue;
				}

				// 삼촌이 RED가 아니라면 restructuring -> double red stop
				// 현재 노드가 부모의 왼쪽 자식이라면
				if (node == node->parent->left_child) {
					node = node->parent;

					// 오른쪽으로 회전
					rotateRight(node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;

				rotateLeft(node->parent->parent);
				break;
			}
		}
		// 루트의 색은 블랙으로 고정
		root->color = BLACK;
	}

	// 트리를 왼쪽으로 회전
	// 오른쪽 자식의 왼쪽 자식을 부모의 오른쪽에 연결
	void rotateLeft(Node* node) {
		// 루트 노드라면
		if (node->parent == NIL) {
			Node* right = root->right_child;
			root->right_child = right->left_child;
			right->left_child->parent = root;
			root->parent = right;
			right->left_child = root;
			right->parent = NIL;
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

			if (node->right_child->left_child != NIL) {
				node->right_child->left_child->parent = node;
			}

			node->right_child = node->right_child->left_child;
			node->parent->left_child = node;
		}
	}

	// 트리를 오른쪽으로 회전
	// 왼쪽 자식의 오른쪽 자식을 부모의 왼쪽에 연결
	void rotateRight(Node* node) {
		// 루트 노드라면
		if (node->parent == NIL) {
			Node* left = root->left_child;
			root->left_child = root->left_child->right_child;
			left->right_child->parent = root;
			root->parent = left;
			left->right_child = root;
			left->parent = NIL;
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

			if (node->left_child->right_child != NIL) {
				node->left_child->right_child->parent = node;
			}

			node->left_child = node->left_child->right_child;
			node->parent->right_child = node;
		}
	}

	// depth 리턴
	int findDepth(int id) {
		Node* curNode = root;
		int depth = -1;

		while (curNode != NIL) {
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
		return 0;
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
			tree.discount(tree.root, x, y, p);
		}
	}
	return 0;
}