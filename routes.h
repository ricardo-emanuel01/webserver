# include <stdlib.h>
# include <string.h>


typedef struct Node{
    int height;
    char *key, *templateName;
    struct Node *left;
    struct Node *right;
} Node, *NodePtr;

int max(int, int);

int height(NodePtr);

NodePtr newNode(char *, char *);

NodePtr leftRotate(NodePtr);

NodePtr rightRotate(NodePtr);

int getBalance(NodePtr);

NodePtr addRoute(NodePtr, char *, char *);

NodePtr search(NodePtr, char *);

NodePtr deleteNode(NodePtr, char *);

NodePtr minValueNode(NodePtr);

void inOrder(NodePtr);

void freeTree(NodePtr);
