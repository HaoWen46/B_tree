#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define degree 3

typedef struct node {
    int n, keys[(degree << 1) - 1];
    struct node *children[degree << 1];
    bool is_leaf;
}Node;

Node *new_node() {
    Node *node = malloc(sizeof(node));
    memset(node, 0, sizeof(node));
    node->is_leaf = true;
    return node;
}

void traverse(Node *node) {
    if (node == NULL) return;
    int i = 0;
    for (i = 0; i < node->n; i++) {
        if (!node->is_leaf) traverse(node->children[i]);
        printf("%d ", node->keys[i]);
    }
    if (!node->is_leaf) traverse(node->children[i]);
}

Node *search(Node *node, int key) {
    int i = 0;
    while (i < node->n && key > node->keys[i]) i++;
    if (node->keys[i] == key) return node;
    if (node->is_leaf) return NULL;
    return search(node->children[i], key);
}

// Node y must be full when this function is called
void split_child(Node *x, Node *y, int i) {
    // Node z is going to store the right (degree - 1) keys of Node y
    Node *z = new_node();
    z->is_leaf = y->is_leaf;
    z->n = degree - 1;
    // copying the right half of keys from y to z
    for (int j = 0; j < degree - 1; j++) {
        z->keys[j] = y->keys[j + degree];
    }
    if (!y->is_leaf) {
        for (int j = 0; j < degree; j++) {
            z->children[j] = y->children[j + degree];
        }
    }
    // Node y will store the left (degree - 1) keys
    y->n = degree - 1;
    for (int j = x->n; j > i; j--) {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;
    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[degree - 1];
    x->n++;
}

void insert_non_full(Node *node, int key) {
    int i = node->n - 1;
    if (node->is_leaf) {
        for (; i >= 0 && node->keys[i] > key; i--) {
            node->keys[i + 1] = node->keys[i];
        }
        node->keys[i + 1] = key;
        node->n++;
    }
    else {
        while (i >= 0 && node->keys[i] > key) i--;
        if (node->children[i + 1]->n + 1 == degree << 1) {
            split_child(node, node->children[i + 1], i + 1);
            if (node->keys[i + 1] < key) i++;
        }
        insert_non_full(node->children[i + 1], key);
    }
}

Node *insert(Node *node, int key) {
    if (node == NULL) {
        node = new_node();
        node->keys[0] = key;
        node->n = 1;
    }
    else if (node->n + 1 == degree << 1) {
        // Node s will become the new root
        Node *s = new_node();
        s->is_leaf = false;
        s->children[0] = node;
        split_child(s, node, 0);
        int i = (s->keys[0] < key)? 1: 0;
        insert_non_full(s->children[i], key);
        node = s;
    }
    else insert_non_full(node, key);
    return node;
}

int main() {
    Node *root = NULL;
    int x;
    char action[8];
    while (1) {
        printf("\nEnter your operations:\n");
        printf("(insert x // delete x // search x // display // exit)\n");
        scanf("%s", action);
        if (!strcmp(action, "insert")) {
            scanf("%d", &x);
            root = insert(root, x);
        }
        else if (!strcmp(action, "search")) {
            scanf("%d", &x);
            Node *tmp = search(root, x);
            if (tmp != NULL) printf("value %d is found!\n", x);
            else printf("value %d is not present in the tree\n", x);
        }
        else if (!strcmp(action, "display")) {
            traverse(root);
            printf("\n");
        }
        else if (!strcmp(action, "exit")) break;
        else printf("Please try again\n");
    }
    return 0;
}