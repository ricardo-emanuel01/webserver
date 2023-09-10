# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "routes.h"


int max(int a, int b) {
    return (a > b) ? a : b;
}


int height(NodePtr node) {
    if (node == NULL) return 0;
    return node->height;
}


NodePtr newNode(char *key, char *templateName) {
    NodePtr node = (NodePtr)malloc(sizeof(Node));
    node->key = key;
    node->templateName = templateName;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}


NodePtr leftRotate(NodePtr x) {
    NodePtr y = x->right;
    NodePtr temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


NodePtr rightRotate(NodePtr x) {
    NodePtr y = x->left;
    NodePtr temp = y->right;

    y->right = x;
    x->left = temp;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


int getBalance(NodePtr node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}


NodePtr addRoute(NodePtr node, char *key, char *templateName) {
    if (node == NULL) return newNode(key, templateName);

    if (strcmp(key, node->key) > 0) node->right = addRoute(node->right, key, templateName);
    else if (strcmp(key, node->key) < 0) node->left = addRoute(node->left, key, templateName);
    else return node;

    node->height = max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);

    // Rebalance
    if (balance > 1) {
        if (strcmp(key, node->left->key) > 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        } else if (strcmp(key, node->left->key) < 0) return rightRotate(node);
    } else if (balance < -1) {
        if (strcmp(key, node->right->key) > 0) return leftRotate(node);
        else if (strcmp(key, node->right->key) < 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}


NodePtr deleteNode(NodePtr root, char *key) {
    if (root == NULL) return root;

    if (strcmp(key, root->key) > 0) root->right = deleteNode(root->right, key);
    else if (strcmp(key, root->key) < 0) root->left = deleteNode(root->left, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            NodePtr temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else *root = *temp;

            free(temp);
        } else {
            NodePtr temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL) return root;

    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = getBalance(root);

    if (balance > 1) {
        if (getBalance(root->left) >= 0) return rightRotate(root);
        else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    } else if (balance < -1) {
        if (getBalance(root->right) <= 0) return leftRotate(root);
        else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }

    return root;
}


NodePtr search(NodePtr root, char *key) {
    if (root == NULL) return NULL;

    if (strcmp(key, root->key) == 0) return root;
    else if (strcmp(key, root->key) > 0) return search(root->right, key);
    else if (strcmp(key, root->key) < 0) return search(root->left, key);
}


NodePtr minValueNode(NodePtr node) {
    NodePtr current = node;
    while (current->left != NULL) current = current->left;
    return current;
}


void inOrder(NodePtr root) {
    if (root != NULL) {
        inOrder(root->left);
        inOrder(root->right);
        printf("%s\n", root->templateName);
    }
}


void freeTree(NodePtr root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
