#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MIN 4

typedef struct node {
    int value;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
    struct node *root;
} tree;

typedef struct stack {
    int size;
    int top;
    node **array;
} stack;

node *construct_node(int val) {
    node *nd = (node*)malloc(sizeof(node));
    if (nd != NULL) {
        nd->value = val;
        nd->left = nd->right = NULL;
    }
    return nd;
}

tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    if (tr != NULL) {
        tr->root = NULL;
    }

    return tr;
}

stack *create_stack() {
    node **array = malloc(MIN*sizeof(node *));
    stack *stk = (stack*)malloc(sizeof(stack));

    if (array == NULL || stk == NULL) {
        return NULL;
    }

    stk->size = MIN;
    stk->top =-1;
    stk->array = array;

    return stk;
}

void free_node (node *nd) {
    if (nd == NULL){
        return;
    }

    free_node(nd->left);
    free_node(nd->right);

    printf("%d ", nd->value);
    free(nd);
}

void free_tree (tree *tr){
    if (tr == NULL) {
        return;
    }
    free_node(tr->root);
    printf("is removed. Tree is empty\n");
    free(tr);
}

node *add_node (node *root, int val) {
    if (root == NULL) {
        return construct_node(val);
    }

    if (root->value < val) {
        root->right = add_node(root->right, val);
    } else {
        root->left = add_node(root->left, val);
    }

    return root;
}

void add(tree *tr, int val) {
    if (tr == NULL) return;

    tr->root = add_node(tr->root, val);
}

void push(stack *stk, node *val) {
    if (stk->top == stk->size - 1){
        int size = stk->size * 2;
        stk->size = size;
        node **copy = malloc(size*sizeof(node *));
        if (copy == NULL) return;

        for (int i = 0; i <= stk->top; i++) {
            copy[i] = stk->array[i];
        }

        free(stk->array);
        stk->array = copy;
    }
    stk->top++;
    stk->array[stk->top] = val;
}

node *pop (stack *stk) {
    if (stk->top == -1) {
        printf("Stack underflow");
        return;
    }

    node *val = stk->array[stk->top];
    stk->top--;

    if (stk->top > 0 && stk->top == stk->size/4) {
        int size = stk->size/2;
        stk->size = size;
        node **copy = malloc(size*sizeof(node *));
        if (copy == NULL) return val;

        for (int i = 0; i<=stk->top; i++) {
            copy[i] = stk->array[i];
        }

        free(stk->array);
        stk->array = copy;
    }

    return val;
}

void print(tree *tr) {
    if (tr == NULL || tr->root == NULL) return;

    stack *stk = create_stack();
    if (stk == NULL) return;

    node *cur = tr->root;

    while (cur != NULL || stk->top != -1) {
        while (cur != NULL) {
        push(stk, cur);
        cur = cur->left;
    }

        cur = pop(stk);
        printf("%d ", cur->value);

        cur = cur->right;

    }

    printf("\n");
    free(stk);
}


int main() {
    tree *tr = construct_tree();
    for (int i = 0; i < 20; i++) {
        add(tr, rand() % 20);
    }

   print(tr);

    free_tree(tr);
    return 0;
}

