#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>


void inorder_traversal(node_t *node, node_t *nil) {
    if (node == nil) {
        return;
    }
    inorder_traversal(node->left, nil);
    printf("%d ", node->key);
    inorder_traversal(node->right, nil);
}



static node_t *create_node(key_t key) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
        return NULL; // 메모리 할당 실패
    }
    node->key = key;
    node->color = RBTREE_RED; // 새 노드는 기본적으로 빨강색
    node->left = node->right = node->parent = NULL;
    return node;
}


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = NULL; // 새 노드의 부모 노드를 추적하기 위한 포인터
    node_t *x = t->root; // 현재 노드를 나타내며, 트리를 순회하기 위한 포인터
    node_t *z = create_node(key); // 삽입할 새 노드 생성

    // 1. 삽입할 위치 찾기: 새 노드(z)가 삽입될 부모 노드(y)를 찾습니다.
    while (x != NULL) {
        y = x; // y는 삽입될 위치의 부모 노드를 가리키게 됩니다.
        if (z->key < x->key) {
            x = x->left; // 새 노드의 키가 현재 노드의 키보다 작으면 왼쪽으로 이동
        } else {
            x = x->right; // 그렇지 않으면 오른쪽으로 이동
        }
    }

    // 2. 새 노드의 부모 노드를 설정
    z->parent = y;

    // 3. 트리가 비어 있거나, y가 NULL인 경우 새 노드를 루트로 설정
    if (y == NULL) {
        t->root = z;
    } else if (z->key < y->key) { // 새 노드가 부모 노드보다 작으면 왼쪽 자식으로 설정
        y->left = z;
    } else { // 그렇지 않으면 오른쪽 자식으로 설정
        y->right = z;
    }

    // 4. 새 노드의 초기 색상을 빨간색으로 설정 (생성 시에 처리됨)
    //    새 노드는 기본적으로 빨간색으로 생성되며, 이는 `create_node` 함수에서 처리.

    // 5. Red-Black Tree의 속성을 유지하기 위해 추가 조정을 수행
    rbtree_insert_fixup(t, z);
    //이진 검색 트리 까지 구현한것.
    return z; // 삽입된 노드를 반환
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    node_t *current = t->root;
    // 트리가 비어 있거나, root가 nil 노드인 경우, NULL을 반환
    if (current == NULL || current == t->nil) {
        return NULL;
    }
    // 가장 왼쪽 노드로 이동
    while (current->left != t->nil) {
        current = current->left;
    }
    return current;
}


node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find 루트 노드부터 시작하여 가장 오른쪽 노드, 즉 최대값
  node_t *current = t->root;
        // 트리가 비어 있지 않은지 확인
        if (current == NULL || current == t->nil) {
            return NULL; // 트리가 비어있다면, NULL 반환
        }

        // 가장 오른쪽 노드 찾기
        while (current->right != t->nil) {
            current = current->right;
        }

        // 최대값을 가진 노드 반환
        return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  /*

  삭제할 노드가 리프 노드인 경우
    단순 삭제: 삭제할 노드가 리프 노드이고 레드인 경우, 그냥 삭제하면 됨
     이 경우 트리의 균형이 깨지지 않음
    블랙 리프 삭제 후 조정: 삭제할 노드가 블랙 리프 노드인 경우, 트리의 블랙 높이가 변경될 수 있기에
     특정 조정 작업을 수행

  삭제할 노드가 하나의 자식만 가지는 경우
    자식 노드 승격: 삭제할 노드의 유일한 자식을 삭제할 노드의 위치로 옮김.
     삭제할 노드가 블랙이고 자식이 레드인 경우, 자식을 블랙으로 변경하여 속성을 유지

  삭제할 노드가 두 개의 자식을 가지는 경우

  후계자 노드 찾기: 삭제할 노드의 직접 후계자(오른쪽 서브트리에서 가장 작은 노드) 또는
    직접 전임자(왼쪽 서브트리에서 가장 큰 노드)를 찾아, 삭제할 노드와 교체
    후계자는 리프 노드이거나 하나의 자식만 가질 수 있기에 위의 경우 1 또는 2를 적용

  삭제 후 조정
  삭제 과정에서 삭제로 인해 레드-블랙 트리의 속성이 위반된 경우,
   트리를 조정 조정은 주로 회전과 색 변경을 통해 이루어지고 다음과 같은 세부 케이스로 나눈다

    케이스 1: 형제 노드가 레드인 경우
    케이스 2: 형제 노드가 블랙이고, 형제의 두 자식도 모두 블랙인 경우
    케이스 3: 형제 노드가 블랙이고, 형제의 왼쪽 자식만 레드인 경우
    케이스 4: 형제 노드가 블랙이고, 형제의 오른쪽 자식만 레드인 경우

  */
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
    // 삽입 후 조정 로직 구현
}