#include<iostream>
#include<string>
using namespace std;

class AVL{
    public:
        class node{
            public:
                string prodID;
                double price;
                bool inStock;
                int height;
                node * left;
                node * right;
                node(string prodID, double price, bool inStock){
                    height = 1;
                    this->prodID = prodID;
                    this->price = price;
                    this->inStock = inStock;
                    left = NULL;
                    right = NULL;
                }
        };
        node * root = NULL;
        int n;
        void insert(string prodID, double price, bool inStock){
            root=insertUtil(root, prodID, price, inStock);
        }
        void remove(string prodID){
            root=removeUtil(root, prodID);
        }
        node * search(string prodID){
            return searchUtil(root, prodID);
        }
        void inorder(){
            inorderUtil(root);
            cout<<endl;
        }
    private:
        int height(node * head){
            if(head==NULL) return 0;
            return head->height;
        }
        node * rightRotation(node * head){
            node * newhead = head->left;
            head->left = newhead->right;
            newhead->right = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }

        node * leftRotation(node * head){
            node * newhead = head->right;
            head->right = newhead->left;
            newhead->left = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }

        void inorderUtil(node * head){
            if(head==NULL) return ;
            inorderUtil(head->left);
            cout<<head->prodID<<" "<<head->price<<" "<<head->inStock<<endl;
            inorderUtil(head->right);
        }

        node * insertUtil(node * head, string prodID, double price, bool inStock){
            if(head==NULL){
                n+=1;
                node * temp = new node(prodID, price, inStock);
                return temp;
            }
            if(prodID < head->prodID) head->left = insertUtil(head->left, prodID, price, inStock);
            else if(prodID > head->prodID) head->right = insertUtil(head->right, prodID, price, inStock);
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(prodID < head->left->prodID){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal<-1){
                if(prodID > head->right->prodID){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * removeUtil(node * head, string prodID){
            if(head==NULL) return NULL;
            if(prodID < head->prodID){
                head->left = removeUtil(head->left, prodID);
            }else if(prodID > head->prodID){
                head->right = removeUtil(head->right, prodID);
            }else{
                node * r = head->right;
                if(head->right==NULL){
                    node * l = head->left;
                    delete(head);
                    head = l;
                }else if(head->left==NULL){
                    delete(head);
                    head = r;
                }else{
                    while(r->left!=NULL) r = r->left;
                    head->prodID = r->prodID;
                    head->right = removeUtil(head->right, r->prodID);
                }
            }
            if(head==NULL) return head;
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(height(head->left) >= height(head->right)){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal < -1){
                if(height(head->right) >= height(head->left)){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * searchUtil(node * head, string prodID){
            if(head == NULL) return NULL;
            string prodIDSearch = head->prodID; //Unneccessary Code?
            if(prodIDSearch == prodID) return head;
            if(prodIDSearch > prodID) return searchUtil(head->left, prodID);
            if(prodIDSearch < prodID) return searchUtil(head->right, prodID);
        }
};

int main() {
  AVL tree;
  tree.insert("AC12", 14, true);
  
  return 0;
}