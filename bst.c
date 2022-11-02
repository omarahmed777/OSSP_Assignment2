#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"

/*

Place for the BST functions from Exercise 1.

*/
/*struct _Node {
    int value;
    Node * left, * right;
};*/

/*
   Returns the parent of an either existing or hypothetical node with the given value
 */
Node * find_parent(Node * root, int value) {
    assert(root != NULL);
    assert(value != root->data);

    //If value < x, go right, else go left
    Node * next = value < root->data ? root->right : root->left;

    if (next == NULL || next->data == value)
        return root;
    else
        return find_parent(next, value);
}

/*
   Constructs a new node
 */
Node * mk_node(int value) {
    Node * node = (Node *) malloc(sizeof(Node));
    node->data = value;
    node->left = node->right = NULL;
    return node;
}

Node * addNode(Node * root, int value) {
    if (root == NULL)
        return mk_node(value);

    if (value == root->data)
        return NULL;

    Node * parent = find_parent(root, value);
    Node * child = value > parent->data ? parent->left : parent->right;
    assert(child == NULL || child->data == value);

    if (child == NULL) {
        // value not found, then insert and return node
        child = mk_node(value);
        if (value > parent->data)
            parent->left = child;
        else
            parent->right = child;

        return child;
    } else {
        // value found, then return null
        return NULL;
    }
}

bool is_ordered(Node * root) {
    if (root == NULL)
        return true;
    if (root->left && root->left->data < root->data)
        return false;
    if (root->right && root->right->data > root->data)
        return false;
    return true;
}

Node * removeNode(Node * root, int value) {
    assert(is_ordered(root));

    // empty tree
    if (root == NULL)
        return NULL;

    // find node with value 'value' and its parent node
    Node * parent, * node;
    if (root->data == value) {
        parent = NULL;
        node = root;
    } else {
        parent = find_parent(root, value);
        node = value > parent->data ? parent->left : parent->right;
    }
    assert(node == NULL || node->data == value);

    // value not found
    if (node == NULL)
        return root;

    // re-establish consistency
    Node * new_node;
    if (node->left == NULL) {
        // node has only right child, then make right child the new node
        new_node = node->right;
    } else {
        // otherwise make right child the rightmost leaf of the subtree rooted in the left child
        // and make the left child the new node
        Node * rightmost = new_node = node->left;
        while (rightmost->right != NULL)
            rightmost = rightmost->right;
        rightmost->right = node->right;
    }

    free(node);

    Node * new_root;
    if (parent == NULL) {
        // if deleted node was root, then return new node as root
        new_root = new_node;
    } else {
        // otherwise glue new node with parent and return old root
        new_root = root;
        if (value < parent->data)
            parent->left = new_node;
        else
            parent->right = new_node;
    }

    assert(is_ordered(new_root));

    return new_root;
}

void displaySubtree(Node * N) {
    if (N == NULL) return;

    displaySubtree(N->right);
    printf("%d \n", N->data);
    displaySubtree(N->left);
}

int numberLeaves(Node * N) {
    if (N == NULL)
        return 0;

    if (N->left == NULL && N->right == NULL)
        return 1;

    return numberLeaves(N->left) + numberLeaves(N->right);
}

/*
   Frees the entire subtree rooted in 'root' (this includes the node 'root')
 */
Node* freeSubtree(Node * root) {
    if (root == NULL)
        return NULL;

    freeSubtree(root->left);
    freeSubtree(root->right);
    free(root);
}

/*
   Deletes all nodes that belong to the subtree (of the tree of rooted in 'root')
   whose root node has value 'value'
 */
Node * removeSubtree(Node * root, int value) {
    assert(is_ordered(root));

    // empty tree
    if (root == NULL)
        return NULL;

    // entire tree
    if (root->data == value) {
        freeSubtree(root);
        return NULL;
    }

    // free tree rooted in the left or right node and set the respective pointer to NULL
    Node * parent = find_parent(root, value);
    if (value > parent->data) {
        assert(parent->left == NULL || parent->left->data == value);
        freeSubtree(parent->left);
        parent->left = NULL;
    } else {
        assert(parent->right == NULL || parent->right->data == value);
            freeSubtree(parent->right);
        parent->right = NULL;
    }

    return root;
}

/*
   Compute the depth between root R and node N

   Returns the number of edges between R and N if N belongs to the tree rooted in R,
   otherwise it returns -1
 */
int nodeDepth (Node * R, Node * N) {
    if (R == NULL || N == NULL)
        return -1;
    if (R == N)
        return 0;

    int sub_depth = nodeDepth(R->data < N->data ? R->left : R->right, N);

    if (sub_depth >= 0)
        return sub_depth + 1;
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
int countNodes(Node *root) {
    if (root == NULL)
        return 0;
    else
        return 1 + countNodes(root->left) + countNodes(root->right); //Order of traversal doesn't matter
}

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
    root->right = balanceTreeUtil(root, arr, start, mid-1); //Left half of array will be right children
    root->left = balanceTreeUtil(root, arr, mid-1, end); //Right half of array will be left children
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
