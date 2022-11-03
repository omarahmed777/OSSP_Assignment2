#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/
struct _Node {
    int value;
    struct _Node* left;
    struct _Node* right;
};

Node* addNode(Node *root, int value)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    Node *temp;
    Node *prev = NULL;

    temp = root;
    while(temp) {
        prev = temp;
        if (temp->data > newNode->data)
            temp = temp->right;
        else if (temp->data < newNode->data)
            temp = temp->left;
        else {
            free(newNode);
            return NULL;
        }
    }
    if (prev==NULL)
        root = newNode;
    else {
        if (prev->data > newNode->data)
            prev->right = newNode;
        else
            prev->left = newNode;
    }

    return newNode;
}

Node* freeSubtree(Node *N) {
    if (N == NULL)
        return NULL;
    freeSubtree(N->left);
    freeSubtree(N->right);
    free(N);
}

void removeSubtreeWorker(Node *R, int value) {
    if (R == NULL)
        return;

    if (R->left && R->left->data == value) {
        freeSubtree(R->left);
        R->left = NULL;
        return;
    }

    if (R->right && R->right->data == value) {
        freeSubtree(R->right);
        R->right = NULL;
        return;
    }

    if (R->data < value)
        removeSubtreeWorker(R->left, value);
    else
        removeSubtreeWorker(R->right, value);
}

Node* removeSubtree(Node *R, int value) {
    if (R == NULL)
        return NULL;

    if (R->data == value) {
        freeSubtree(R);
        return NULL;
    }
    removeSubtreeWorker(R, value);
    return R;
}

void displaySubtree(Node *N)
{
    if (N==NULL)
    {
        return ;
    }
    displaySubtree(N->right);
    printf("%d\n", N->data);
    displaySubtree(N->left);
}

int countNodes(Node *N)
{
    int count=0;
    if(N==NULL)
        return 0;
    count = count + countNodes(N->right);
    count = count + countNodes(N->left);
    count = count + 1;
    return count;
}

// this is the most complicated task
Node* removeNode(Node* root, int value)
{
    // Ensure root isn't null.
    if (root == NULL) {
        return NULL;
    }

    if (value > root->data) {  // Value is in the left sub-tree.
        root->left = removeNode(root->left, value);
    } else if (value < root->data) { // Value is in the right sub-tree.
        root->right = removeNode(root->right, value);
    } else { // Found the correct node with value
        // Check the three cases - no child, 1 child, 2 child...
        // No Children
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
            // 1 child (on the right)
        else if (root->left == NULL) {
            Node *temp = root; // save current node
            root = root->right;
            free(temp);
        }
            // 1 child (on the left)
        else if (root->right == NULL) {
            Node *temp = root; // save current node
            root = root->left;
            free(temp);
        }
            // Two children
        else {
            // find minimal value of right subtree
            Node *temp = root->left;
            while(temp->right != NULL) {
                temp = temp->right;
            }
            root->data = temp->data; // duplicate the node
            root->left = removeNode(root->left, root->data); // delete the duplicate node
        }
    }
    return root; // parent node can update reference
}

int numberLeaves(Node *N) {
    if (N == NULL)
        return 0;

    if (N->left == NULL && N->right == NULL)
        return 1;

    return numberLeaves(N->left) + numberLeaves(N->right);
}

int nodeDepth(Node *R, Node *N) {
    if (R == NULL || N == NULL)
        return -1;

    if (R == N)
        return 0;

    int subDepth = nodeDepth(R->data < N->data ? R->left : R->right, N);

    if (subDepth >= 0)
        return subDepth + 1;
    else
        return -1;
}


/*
 * ------------------------------ EXERCISE 2 ------------------------------
 */

/*
 * avgSubtree Helper Function
 * Returns sum of all nodes in tree
 */
int sum(Node *N) {
    if (N == NULL)
        return 0;
    else return N->data + sum(N->left) + sum(N->right); //Order of traversal doesn't matter
}

/*
 * Returns total number of nodes in tree
 * Recursion atm, could take a while potentially with large trees?
 */
/*int countNodes(Node *root) {
    if (root == NULL)
        return 0;
    else
        return 1 + countNodes(root->left) + countNodes(root->right); //Order of traversal doesn't matter
}*/

float avgSubtree(Node *N)
{
	// TODO: Implement this function
    if (N == NULL) //Base case
        return -1;
    float avg = (float)sum(N) / (float)countNodes(N);
    return avg;
}
/*
 * Helper function for balanceTree
 * Converts tree into sorted array
 */
void treeToArr(Node *N, int arr[], int i) {
    if (N == NULL)
        return;
    //Traverse fully to the right
    treeToArr(N->right, arr, i);
    //Store current node into array
    arr[i] = N->data;
    i++;
    //Traverse fully to the left
    treeToArr(N->left, arr, i);
}
/*
 * balanceTree Helper Function
 * Takes node, array, start, and end
 * Recursively runs through tree, taking middle of array each time
 * Correctly balances inverted tree
 */
Node* balanceTreeUtil(Node* root, int arr[], int start, int end) {
    if (start > end) //Base case to break recursion
        return NULL;
    int mid = (start+end) / 2; //Get middle elem of arr
    root->data = arr[mid]; //Middle elem of sorted array is root
    root->right = balanceTreeUtil(root->right, arr, start, mid-1); //Left half of array will be right children
    root->left = balanceTreeUtil(root->left, arr, mid-1, end); //Right half of array will be left children
    return root;
}
// This functions converts an unbalanced BST to a balanced BST
Node* balanceTree(Node* root)
{
	// TODO: Implement this function
    int sortedArr[countNodes(root)]; //Space for every node in tree
    // Fill sortedArr with nodes
    treeToArr(root, sortedArr, 0);
    int length = sizeof(sortedArr)/sizeof(sortedArr[0]); //Computes actual length of array, not allocated mem
    root = balanceTreeUtil(root, sortedArr, 0, length-1);
    return root;

}
