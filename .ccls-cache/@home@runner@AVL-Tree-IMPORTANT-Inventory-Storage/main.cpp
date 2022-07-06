#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<stdexcept>

#include "AVLTree.cpp"
using namespace std;


//-----------------------------------------------------
void PopulateInvAVL(AVL *tree, string fileName, string prodStatus, vector<string> *prodListErrors);
bool isValidProd(vector<string> prodID);
int findPrice(vector<string> prodID);
bool findinStock(vector<string> prodID, string prodStatus, int price);
void createDeleteCSV(ofstream &outF, AVL::node *prod);
//-----------------------------------------------------
int main(){
  //Initialize Vectors of Accuterm Naming Errors (or Others for Debugging Reasons)
  vector<string> *prodListErrors = new vector<string>;
  
  //Populate Discontinued/Closeout Items Tree
  AVL *disContTree = new AVL;
  PopulateInvAVL(disContTree, "Close Out.csv", "Discontinue", prodListErrors);
  PopulateInvAVL(disContTree, "Discontinued.csv", "Discontinue", prodListErrors);

  //Populate Running/Slow Seller Items Tree
  AVL *contTree = new AVL;
  PopulateInvAVL(contTree, "Running.csv", "Continue", prodListErrors);
  PopulateInvAVL(contTree, "Slow Seller.csv", "Continue", prodListErrors);

  //Populate Website's Items Tree
  AVL *webTree = new AVL;
  PopulateInvAVL(contTree, "Website Inventory.csv", "Continue", prodListErrors);

  //Write To "Items to Delete off Website.csv"
  ofstream outF;
  outF.open("Items to Delete off Website.csv");
  createDeleteCSV(outF, disContTree->root);
  outF.close();

  //Write To "Accuterm Items with Error Naming Conventions"
  if (prodListErrors->size() > 0) {
    outF.open("Accuterm Items with Error Naming Conventions.csv");
    for (unsigned i = 0; i < prodListErrors->size(); ++i) {
      outF << prodListErrors->at(i) << endl;
    }
    outF.close();
  }
}
//-----------------------------------------------------

void createDeleteCSV(ofstream &outF, AVL::node *prod) {
  if(prod==NULL) return;
    createDeleteCSV(outF, prod->left);
    if (!prod->inStock) {
      outF << prod->prodID << endl;
    }
    createDeleteCSV(outF, prod->right);
}

bool isValidProd(vector<string> prodRow){
  if (prodRow.at(5).find(" OF ") != string::npos) {return false;} //SubProdIds(Ex.AC117-1)
  else if (prodRow.at(4).find(" OF ") != string::npos) {return false;} //SubProdIds(Ex.AC117-1)
  else if (prodRow.at(1).empty() && prodRow.at(2).empty()) {return false;} //Empty Row
  return true;
}

int findPrice(vector<string> prodID) {
  if (prodID.at(7).empty()) {return 0;}
  return max(stoi(prodID.at(6)), stoi(prodID.at(7)));
}

bool findinStock(vector<string> prodID, string prodStatus, int price) {
  if (prodStatus == "Discontinue") {
    if ((stoi(prodID.at(10)) < 10)) {return false;}
    else if (price < 20) {return false;}
    return true;
  }
  else if (prodStatus == "Continue") {
    if ((stoi(prodID.at(10)) < 1)) {return false;}
    else if (price < 20) {return false;}
    return true;
  }
  else {cout << "Invalid Product Status Name" << endl; exit(-1);}
}

void PopulateInvAVL(AVL *tree, string ACFileName, string prodStatus, vector<string> *prodListErrors)
{
  // File pointer
  fstream fin;

  // Open an existing file
  fin.open(ACFileName, ios::in);
  if (!fin.is_open()) {
    cout << "COuldn't open" << endl;
    exit(-1);
  }

  // Read the Data from the file
  // as String Vector
  vector<string> row;
  int roll2;
  string line, word, temp;
  //Skip Headings/titles ("Product ID, Product Status, ...")
  while (getline(fin, line) && word != "Product Status" && word != "Type") {
    //row.clear();
    stringstream s(line);
    getline(s, word, ',');
    cout << word << " ";
    getline(s, word, ',');
    cout << word << endl;
  }

  stringstream s(line);
  while (getline(s, word, ',')) {row.push_back(word);}
  if (ACFileName != "Website Inventory.csv") {
    cout << row.at(0) << ": " << isValidProd(row) << "<- IsValid " <<  endl;
    if (isValidProd(row)) {
      cout << row.at(0) << " ";
      string prodID = row.at(0);
      int price = max(stoi(row.at(6)), stoi(row.at(7)));
      bool inStock = (stoi(row.at(10)) > 0);
      tree->insert(prodID, price, inStock, 0 , "hi");
    }
  }
  else {
    int ID = stoi(row.at(0));
    string SKU = row.at(2);
    string prodID = row.at(3);
    bool inStock = (stoi(row.at(4)) > 0);
    int price = stoi(row.at(5));
    tree->insert(prodID, price, inStock, ID, SKU);
  }

  while (getline(fin, line)) {
    row.clear();

    // read an entire row and store it in a string variable 'line'
    cout << line << endl;

    // used for breaking words
    stringstream s(line);

    // read every column data of a row and store it in a string variable, 'word'
    while (getline(s, word, ',')) {

      // add all the column data of a row to a vector
      row.push_back(word);
    }
    if (ACFileName != "Website Inventory.csv") {
      cout << row.at(0) << ": " << isValidProd(row) << "<- IsValid " <<  endl;
      //If doesn't contains " OF "
      if (isValidProd(row)) {
        cout << row.at(0) << " ";
        try {
          int price = findPrice(row);
          bool inStock = findinStock(row, prodStatus, price);
          tree->insert(row.at(0), price, inStock);
  
        }
        catch(invalid_argument const& ex) {
          cout << ex.what() << endl;
          prodListErrors->push_back(row.at(0));
        }
      }
    }
    else {
    int ID = stoi(row.at(0));
    string SKU = row.at(2);
    string prodID = row.at(3);
    bool inStock = (stoi(row.at(4)) > 0);
    int price = stoi(row.at(5));
    tree->insert(prodID, price, inStock, ID, SKU);
    }
  }
  fin.close();
}
