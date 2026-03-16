#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MIN 4

typedef struct node {
    char character;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
    node *root;
} tree;

typedef struct queue {
    int first;
    int last;
    int size;
    int elements;
    node **array;
} queue;

typedef struct sequence {
    queue *hold;
} sequence;


tree *construct_tree() {
    tree *tr = (tree*)malloc(sizeof(tree));
    if (tr != NULL) {
        tr->root = NULL;
    }

    return tr;
}

node *construct_node(char c) {
    node *nd = (node*)malloc(sizeof(node));
    if (nd != NULL) {
        nd->character = c;
        nd->left = nd->right = NULL;
    }

    return nd;
}

queue *create_queue() {
    node **array = malloc(MIN*sizeof(node *));
    queue *q = (queue*)malloc(sizeof(queue));

    if (q != NULL && array != NULL) {
        q->elements = q->first = q->last = 0;
        q->size = MIN;
        q->array = array;
    }

    return q;
}


void free_node (node *nd){
    if (nd == NULL) {
        return;
    }

    free_node(nd->left);
    free_node(nd->right);

    free(nd);
}

void free_tree(tree *tr) {
    if (tr == NULL) return;
    free_node(tr->root);
    free(tr);
}

node *add_node(node *root, char c) {
    if (root == NULL)
        return construct_node(c);

    if (c < root->character)
        root->left = add_node(root->left, c);
    else if (c > root->character)
        root->right = add_node(root->right, c);

    return root;
}

void add(tree *tr, char c) {
    if (tr == NULL) return;

    tr->root = add_node(tr->root, c);
}

void enqueue (queue *q, node *nd) {
    if (q->elements == q->size) {
        int size = q->size * 2;
        node **copy = malloc(size*sizeof(node *));
        for (int i = 0; i < q->elements; i++) {
            copy[i] = q->array[q->first + i];
        }

        free(q->array);
        q->array = copy;
        q->size = size;
        q->first = 0;
        q->last = q->elements;
    }

    q->array[q->last] = nd;
    q->last = q->last + 1;
    q->elements++;
}

node *dequeue(queue *q) {
    if (q == NULL || q->elements == 0) {
        printf("Queue underflow\n");
        return NULL;
    }

    node *nd = q->array[q->first];
    q->first = q->first + 1;
    q->elements--;

    if (q->elements > 0 && q->elements <= q->size/4) {
        int size = q->size/2;
        node **copy = malloc(size*sizeof(node *));

        for (int i = 0; i < q->elements; i++) {
            copy[i] = q->array[q->first + i];
        }

        free(q->array);
        q->array = copy;
        q->size = size;
        q->first = 0;
        q->last = q->elements;
    }

    return nd;
}

void print (tree *tr) {
    if (tr == NULL || tr->root == NULL) return;

    queue *q = create_queue();
    if (q == NULL) return;

     enqueue(q, tr->root);
    

    while (q->elements > 0) {
        node *cur = dequeue(q);
        printf("%c ", cur->character);

        if (cur->left != NULL){
            enqueue(q, cur->left);
        }
        
        if (cur->right != NULL) {
            enqueue(q, cur->right);
        }

    }

    printf("\n");

}

sequence *create_sequence (tree *tr) {
    if (tr == NULL || tr->root == NULL) {
        return NULL;
    }


    sequence *seq = malloc(sizeof(sequence));
    if (seq == NULL) {
        return NULL;
    }

    seq->hold = create_queue();
    if (seq->hold == NULL) {
        free(seq);
        return NULL;
    }

    enqueue(seq->hold, tr->root);
    return seq;
} 



char next(sequence *seq){
    if (seq == NULL || seq->hold->elements == 0) {
        return 'c';
    }

    node *cur = dequeue(seq->hold);

    if (cur->left !=NULL) {
        enqueue(seq->hold, cur->left);
    }

    if (cur->right != NULL) {
        enqueue(seq->hold, cur->right);
    }

    return cur->character;
}


int main() {
    tree *tr = construct_tree();

    for (int i = 0; i < 40; i++ ) {
        add(tr, 'a' + rand()%15);
    }

    sequence *seq = create_sequence(tr);

    char a = next(seq);
    char b = next(seq);
    char c = next(seq);

    char d = next(seq);
    char e = next(seq);

    printf("%c, %c, %c, %c, %c\n", a,b,c,d,e);

    print(tr);

    free_tree(tr);
    return 0;
}