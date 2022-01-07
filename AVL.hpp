#ifndef AVL_HPP_
#define AVL_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "User.hpp"

using namespace std;

//User AVL Implementation
class AVL{
    public:
    User U;
    AVL *left, *right;
    int height;

    AVL(){
        U = User();
        left = right = NULL;
        height = 0;
    }

    AVL(User u){
        U = u;
        left = right = NULL;
        height = 1;
    }

    int max(int a, int b){
        return (a > b)? a : b;
    }

    int get_height(AVL *N){
        if (N == NULL) return 0;
        return N->height;
    }

    AVL *rightRotate(AVL *y){
        AVL *x = y->left;
        AVL *T2 = x->right;
    
        x->right = y;
        y->left = T2;
    
        y->height = max(get_height(y->left), get_height(y->right)) + 1;
        x->height = max(get_height(x->left), get_height(x->right)) + 1;
    
        return x;
    }

    AVL *leftRotate(AVL *x){
        AVL *y = x->right;
        AVL *T2 = y->left;
    
        y->left = x;
        x->right = T2;
    
        x->height = max(get_height(x->left), get_height(x->right)) + 1;
        y->height = max(get_height(y->left), get_height(y->right)) + 1;
    
        return y;
    }

    int getBalance(AVL *N){
        if (N == NULL) return 0;
        return get_height(N->left) - get_height(N->right);
    }

    AVL *minValueNode(AVL* node){
        AVL *current = node;
    
        while (current->left != NULL) current = current->left;
        return current;
    }

    AVL *insert(AVL* Node, User *key){
        if (Node == NULL)
            return(new AVL(*key));
    
        if (key->ID < Node->U.ID)
            Node->left = insert(Node->left, key);
        else if (key->ID > Node->U.ID)
            Node->right = insert(Node->right, key);
        else return Node;
    
        Node->height = 1 + max(get_height(Node->left), get_height(Node->right));

        int balance = getBalance(Node);
    
        if (balance > 1 && key->ID < Node->left->U.ID)
            return rightRotate(Node);

        if (balance < -1 && key->ID > Node->right->U.ID)
            return leftRotate(Node);

        if (balance > 1 && key->ID > Node->left->U.ID){
            Node->left = leftRotate(Node->left);
            return rightRotate(Node);
        }

        if (balance < -1 && key->ID < Node->right->U.ID){
            Node->right = rightRotate(Node->right);
            return leftRotate(Node);
        }

        return Node;
    }

    AVL *deleteNode(AVL* root, int key){
        if (root == NULL) return root;

        if (key < root->U.ID)
            root->left = deleteNode(root->left, key);
        else if(key > root->U.ID)
            root->right = deleteNode(root->right, key);
        else{
            if( (root->left == NULL) || (root->right == NULL) ){
                AVL *temp = root->left ? root->left : root->right;
    
                if (temp == NULL){
                    temp = root;
                    root = NULL;
                }
                else *root = *temp;
                delete temp;
            }
            else{
                AVL* temp = minValueNode(root->right);
                root->U = temp->U;
    
                root->right = deleteNode(root->right, temp->U.ID);
            }
        }

        if (root == NULL) return root;
    
        root->height = 1 + max(get_height(root->left), get_height(root->right));
    
        int balance = getBalance(root);
    
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
    
        if (balance > 1 && getBalance(root->left) < 0){
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
    
        if (balance < -1 && getBalance(root->right) > 0){
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    
        return root;
    }

    AVL *Find(AVL *root, int key){
        if (root == NULL) return root;
        if (key < root->U.ID)
            return Find(root->left, key);
        else if(key > root->U.ID)
            return Find(root->right, key);
        else return root;
        return root;
    }

    void PrintInOrder(AVL *root){
        if(root == NULL) return;
        PrintInOrder(root->left);
        cout << root->U.ID << " ";
        PrintInOrder(root->right);
    }
};

#endif