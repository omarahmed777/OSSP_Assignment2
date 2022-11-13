#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/
Node *addNode(Node *root, int value)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    Node *temp;
    Node *prev = NULL;

    temp = root;
    while (temp) {
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
    if (prev == NULL)
        root = newNode;
    else {
        if (prev->data > newNode->data)
            prev->right = newNode;
        else
            prev->left = newNode;
    }

    return root;
}

Node *freeSubtree(Node *N)
{
    if (N == NULL)
        return NULL;
    freeSubtree(N->left);
    freeSubtree(N->right);
    free(N);
    return NULL;
}

void removeSubtreeWorker(Node *R, int value)
{
    if (R == NULL)
        return;

    if (R->left && R->left->data == value)
    {
        freeSubtree(R->left);
        R->left = NULL;
        return;
    }

    if (R->right && R->right->data == value)
    {
        freeSubtree(R->right);
        R->right = NULL;
        return;
    }

    if (R->data < value)
        removeSubtreeWorker(R->left, value);
    else
        removeSubtreeWorker(R->right, value);
}

Node *removeSubtree(Node *R, int value)
{
    if (R == NULL)
        return NULL;

    if (R->data == value)
    {
        freeSubtree(R);
        return NULL;
    }
    removeSubtreeWorker(R, value);
    return R;
}

void displaySubtree(Node *N)
{
    if (N == NULL)
    {
        return;
    }
    displaySubtree(N->right);
    printf("%d\n", N->data);
    displaySubtree(N->left);
}

int countNodes(Node *N)
{
    int count = 0;
    if (N == NULL)
        return 0;
    count = count + countNodes(N->right);
    count = count + countNodes(N->left);
    count = count + 1;
    return count;
}

// this is the most complicated task
Node *removeNode(Node *root, int value)
{
    // Ensure root isn't null.
    if (root == NULL)
    {
        return NULL;
    }

    if (value > root->data)
    {  // Value is in the left sub-tree.
        root->left = removeNode(root->left, value);
    } else if (value < root->data)
    { // Value is in the right sub-tree.
        root->right = removeNode(root->right, value);
    } else
    { // Found the correct node with value
        // Check the three cases - no child, 1 child, 2 child...
        // No Children
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            root = NULL;
        }
            // 1 child (on the right)
        else if (root->left == NULL)
        {
            Node *temp = root; // save current node
            root = root->right;
            free(temp);
        }
            // 1 child (on the left)
        else if (root->right == NULL)
        {
            Node *temp = root; // save current node
            root = root->left;
            free(temp);
        }
            // Two children
        else
        {
            // find minimal value of right subtree
            Node *temp = root->left;
            while (temp->right != NULL)
            {
                temp = temp->right;
            }
            root->data = temp->data; // duplicate the node
            root->left = removeNode(root->left, root->data); // delete the duplicate node
        }
    }
    return root;
}

int numberLeaves(Node *N)
{
    if (N == NULL)
        return 0;

    if (N->left == NULL && N->right == NULL)
        return 1;

    return numberLeaves(N->left) + numberLeaves(N->right);
}

int nodeDepth(Node *R, Node *N)
{
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
int sum(Node *N)
{
    if (N == NULL)
        return 0;
    else return N->data + sum(N->left) + sum(N->right); //Order of traversal doesn't matter
}

float avgSubtree(Node *N)
{
    // TODO: Implement this function
    if (N == NULL) //Base case
        return -1;
    double avg = (float) sum(N) / (float) countNodes(N);
    return (float) avg;
}

/*
 * Helper function for balanceTree
 * Converts tree into sorted array
 */
int treeToArr(Node *N, int *arr, int i)
{
    if (N == NULL)
        return i;
    i = treeToArr(N->right, arr, i);
    arr[i++] = N->data; //Then root
    i = treeToArr(N->left, arr, i);
    return i;
}

/*
 * balanceTree Helper Function
 * Takes node, array, start, and end
 * Recursively runs through tree, taking middle of array each time
 * Correctly balances inverted tree
 */
Node *balanceTreeUtil(int arr[], int start, int end)
{
    if (start > end) //Base case to break recursion
        return NULL;

    int mid = (start + end) / 2; //Get middle elem of arr
    Node *N = (Node *) malloc(sizeof(Node));
    N->data = arr[mid];
    N->right = balanceTreeUtil(arr, start, mid - 1); //Left half of array will be right children
    N->left = balanceTreeUtil(arr, mid + 1, end); //Right half of array will be left children
    return N;
}

// This functions converts an unbalanced BST to a balanced BST
Node *balanceTree(Node *root)
{
    // TODO: Implement this function
    int *sortedArr;
    int length = countNodes(root);
    if ((sortedArr = (int *) malloc(sizeof(int) * length)) == NULL)
    {
        printf("malloc failed");
        exit(-1);
    }
    // Fill sortedArr with nodes
    treeToArr(root, sortedArr, 0);
    //removeSubtree(root, root->data); //-- caused seg faults
    root = balanceTreeUtil(sortedArr, 0, length - 1);
    //displaySubtree(root); //-- for debugging
    free(sortedArr);
    return root;

}

/*
 * ------------------------------ ARCHIVE ------------------------------
 */

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

//This doesn't recurse down the tree properly, but could be faster since it uses iteration
/*void treeToArr(Node *N, int arr[], int length)
{
    int *p = arr;
    if (N == NULL)
        return;
    for (int i=0; i<length; i++)
    {
        while (N->right)
        {
            N = N->right;
            arr[*p+i] = N->data;
        }
        arr[*p+i] = N->data;
        while (N->left)
        {
            N = N->left;
            arr[*p+i] = N->data;
        }
    }
    return;
}*/
