#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct node {
    int value;
    struct node *right;
    struct node *left;
} node;

typedef struct tree {
    node *root;
} tree;

tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    if (tr != NULL) {
        tr->root = NULL;
    }

    return tr;
}

node *construct_node(int val){
    node *nd = (node*)malloc(sizeof(node));
    if (nd != NULL) {
        nd->value = val;
        nd->left = NULL;
        nd->right = NULL;
    }
    return nd;
}


void free_node(node *nd) {
    if (nd == NULL) {
        return;
    }

    free_node(nd->left);
    free_node(nd->right);

    printf("%d ", nd->value);
    free(nd);
}

void free_tree(tree *tr){
    if (tr == NULL) {
        return;
    }
   free_node(tr->root);
   printf("is removed. Tree is empty\n");
   free(tr);

}

node *node_add(node *root, int val) {
    if (root == NULL){
        return construct_node(val);
    }

    if (val > root->value) {
        root->right = node_add(root->right, val);
    } else if (val < root->value) {
        root->left = node_add(root->left, val);
    }

    return root;
}

bool search(node *root, int val) {

    if (root == NULL) {
        printf("%d not found\n", val);
        return false;
    }
    if (root->value == val) {
        printf("%d found.\n", val);
        return true;
    }

     if (val > root->value) {
        search(root->right, val);
    }else {
        search(root->left, val);
    }
}

bool lookup(tree *tr, int val) {
    if (tr == NULL || tr->root == NULL) {
        return false;
    }

    return search(tr->root, val);
}

void add(tree *tr, int val) {
    if (tr == NULL) return;

    tr->root = node_add(tr->root, val);
} 

static void print(node *nd){
    if (nd != NULL) {
        print(nd->left);
        printf("%d ", nd->value);
        print(nd->right);
    }
}

void print_tree(tree *tr) {
    if (tr->root != NULL) {
        printf("Here are all the elements in the tree, inorder:\n");
        print(tr->root);
    }
    printf("\n");
}

int main() {
    tree *tr = construct_tree();
    for (int i = 0; i < 20; i++) {
        add(tr, rand() % 20);
    }

   print_tree(tr);

    free_tree(tr);
    return 0;
}