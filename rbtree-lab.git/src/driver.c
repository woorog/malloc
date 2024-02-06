#include "rbtree.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();
    int key;

    printf("-1 칠때까지 입력 받음: \n");
    while (scanf("%d", &key) && key != -1) {
        rbtree_insert(t, key);
    }

    printf("중회순회 결과: \n");
    inorder_traversal(t->root, t->nil);


    node_t *max_node = rbtree_max(t);
        if (max_node != NULL) {
            printf("\n최댓값 %d\n", max_node->key);
        } else {
            printf("\n트리가 비어 있음.\n");
        }


        // 최소값 출력
       node_t *min_node = rbtree_min(t);
        if (min_node != NULL) {
            printf("최소값: %d\n", min_node->key);
        } else {
            printf("트리가 비어 있음.\n");
        }

    delete_rbtree(t);
    //트리 구조체 메모리 삭제 작업.
    return 0;
}