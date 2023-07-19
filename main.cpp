#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <list>


class node {
    int key;
    node *left, *right;

  public:
    node(int keyValue = 0, node *leftSubTree = nullptr, node *rightSubTree = nullptr) {
      key = keyValue;
      left = leftSubTree;
      right = rightSubTree;
    }

    friend class searchTree;
};

class searchTree {
    node *root;

    void deleteTree(node *);
    void printTree(node *, int);
    node *copyTree(node *);
    int numberOfNodes(node *);
    void traversalInDepth(node *, int *, int &);

  public:
    searchTree(int numberOfNodes = 0);
    searchTree(const searchTree &);
    searchTree operator = (const searchTree &);
    ~searchTree();
    node *addNode(int);
    void printTree();
    int getRoot() {
      return root -> key;
    };
    node *searchKey(int);
    int min();
    int max();

    void keyDeleting(int);
    node *nodeDeleting(node *);

    int numberOfNodes();
    void traversalInDepth(int *);
    int traversalInWidth(int *);
};

searchTree::searchTree(int numberOfNodes) {
    if(numberOfNodes < 0) throw "Error in the number of vertices";
    root = nullptr; 
    if(numberOfNodes == 0) { 
        return;
    }

    for(int i = 0; i < numberOfNodes; i++) {
        root = addNode(rand() % 100);
    }
}

node *searchTree::addNode(int keyValue) {
  if(!root) {
    root = new node(keyValue);
    return root;
  }

  node *currentNode = root;
  node *parentNode;
  
  while(currentNode) {
    if(keyValue < currentNode -> key) { parentNode = currentNode; currentNode = currentNode -> left; }
    else if(keyValue > currentNode -> key) { parentNode = currentNode; currentNode = currentNode -> right; }
    else if(keyValue == currentNode -> key) return root;
  }

  if(keyValue < parentNode -> key) parentNode -> left = new node(keyValue);
  if(keyValue > parentNode -> key) parentNode -> right  = new node(keyValue);

  return root;
}

searchTree::~searchTree(){
  deleteTree(root);
}

void searchTree::deleteTree(node *currentRoot) {
  if(!currentRoot) return;
  deleteTree(currentRoot -> left);
  deleteTree(currentRoot -> right);
  delete currentRoot;
}

void searchTree::printTree() {
  if(!root) return;
  std::cout << std::endl;
  printTree(root, 1);
}

void searchTree::printTree(node *currentRoot, int indent) {
  if(!currentRoot) return;
  if(currentRoot -> right) printTree(currentRoot -> right, indent + 3);
  for(int i = 0; i < indent; i++) std::cout << "-";
  std::cout.width(2);
  std::cout << currentRoot -> key << std::endl;
  if(currentRoot -> left) printTree(currentRoot -> left, indent + 3);  
}

searchTree::searchTree(const searchTree &copiedTree) {
    if(!copiedTree.root) { 
        root = nullptr;
        return;
    }

    root = copyTree(copiedTree.root);
}
    
node *searchTree::copyTree(node *copiedTree) {
  if(copiedTree == nullptr) return nullptr;

    node *newTree = new node(copiedTree -> key);
    newTree -> left = copyTree(copiedTree -> left);
    newTree -> right = copyTree(copiedTree -> right);
    return newTree;
}

searchTree searchTree::operator = (const searchTree &copiedTree) {
  if(this == &copiedTree)
    return *this;

  deleteTree(root);
  root = copyTree(copiedTree.root);
  return *this;
}

node *searchTree::searchKey(int keyValue) {
  if(root == nullptr) return nullptr;

  node *currentNode = root;

  while(currentNode) {
    if(currentNode -> key == keyValue) return currentNode;
    if(currentNode -> key > keyValue) currentNode = currentNode -> left;
    else currentNode = currentNode -> right;
  }
  return nullptr;
}

int searchTree::min() {
  if(root == nullptr) return -1;

  node *currentNode = root;

  while(currentNode -> left) {
    currentNode = currentNode -> left;
  }

  return currentNode -> key;
}

int searchTree::max() {
 if(root == nullptr) return -1;

  node *currentNode = root;

  while(currentNode -> right) {
    currentNode = currentNode -> right;
  }

  return currentNode -> key;
}

void searchTree::keyDeleting(int keyToDelete) {
  if(root == nullptr) return;
  
  node *parentNode, *currentNode = root;
  int flag = true;

  if(root -> key == keyToDelete) {
    root = nodeDeleting(root);
    return;
  }

  while(currentNode && flag) {
    if(currentNode -> key == keyToDelete) flag = false;
    else if(currentNode -> key > keyToDelete) {
      parentNode = currentNode;
      currentNode = currentNode -> left;
    }
    else if(currentNode -> key < keyToDelete) {
      parentNode = currentNode;
      currentNode = currentNode -> right;
    }
  }

  if(flag) return;

  if(parentNode -> key > keyToDelete) parentNode -> left = nodeDeleting(currentNode);
  else parentNode -> right = nodeDeleting(currentNode);

} 

node *searchTree::nodeDeleting(node *nodeToDelete) {
  node *currentNode, *parentNode;

  if(!nodeToDelete -> left) {
    currentNode = nodeToDelete -> right;
    delete nodeToDelete;
    return currentNode;
  }

  if(!nodeToDelete -> right) {
    currentNode = nodeToDelete -> left;
    delete nodeToDelete;
    return currentNode;
  }

  currentNode = nodeToDelete -> right;

  if(!currentNode -> left) {
    nodeToDelete -> key = currentNode -> key;
    nodeToDelete -> right = currentNode -> right;
    delete currentNode;
    return nodeToDelete;
  }

  while(currentNode -> left) {
    parentNode = currentNode;
    currentNode = currentNode -> left;
  }

  nodeToDelete -> key = currentNode -> key;
  parentNode -> left = currentNode -> right;
  delete currentNode;
  return nodeToDelete;

}   

int searchTree::numberOfNodes() {
  return numberOfNodes(root);
}

int searchTree::numberOfNodes(node *currentRoot) {
  if(currentRoot == nullptr) return 0;
  return numberOfNodes(currentRoot -> left) + 1 + numberOfNodes(currentRoot -> right);
}

void searchTree::traversalInDepth(int *traversalInDepthResult) {
  int numberOfNodes = 0;
  traversalInDepth(root, traversalInDepthResult, numberOfNodes);
  return;
}

void searchTree::traversalInDepth(node *currentRoot, int *traversalInDepthResult, int &numberOfNodes) {
  if(!currentRoot) return;
  traversalInDepth(currentRoot -> left, traversalInDepthResult, numberOfNodes);
  traversalInDepthResult[numberOfNodes++] = currentRoot -> key;
  traversalInDepth(currentRoot -> right, traversalInDepthResult, numberOfNodes);
}

int searchTree::traversalInWidth(int *traversalInWidth) {
  std::list <node *> listOfNodes;
  listOfNodes.push_back(root);
  node *currentNode;
  int i = 0;

  while(!listOfNodes.empty()) {
    currentNode = listOfNodes.front();
    if(currentNode -> left) listOfNodes.push_back(currentNode -> left);
    if(currentNode -> right) listOfNodes.push_back(currentNode -> right);
    traversalInWidth[i++] = currentNode -> key;
    listOfNodes.pop_front();  
  }
  return i;
}


int main() {
  srand(time(0));
  
  ///////////////////////////
  // Checking constructor and tree output
  // Проверка конструктора и вывода дерева
  //
  // We create a search tree of 10 keys (or less if the same values are encountered). 
  // Keys are random non-negative integers ranging from 0 to 99.
  // Создаем дерево поиска из 10 ключей (или меньше, если встретились одинаковые значения). 
  // Ключи - случайные целые неотрицательные числа в диапазоне от 0 до 99.
  searchTree tree1(10);
  std::cout << "Tree_1: \n";
  tree1.printTree();
  std::cout << std::endl;

  ///////////////////////////
  // Checking the copy constructor
  // Проверка конструктора копирования
  /*
  searchTree tree2(tree1), tree3;
  std::cout << "Tree_2 - copy of Tree_1: \n";
  tree2.printTree(); 
  std::cout << std::endl;
  */

  ///////////////////////////
  // Checking the Assignment Operation
  // Проверка операции присваивания
  /*
  tree3 = tree1;
  std::cout << "Tree_3 equal to Tree_1: \n";
  tree3.printTree();
  std::cout << std::endl;
  std::cout << "Tree_3 equal to itself: \n";
  tree3 = tree3;
  tree3.printTree();
  std::cout << std::endl;
  */

  ///////////////////////////
  // Checking the search of a key in the tree
  // Проверка поиска ключа в дереве

  int keyValue;
  std::cout << "Input a key to search, please: ";
  std::cin >> keyValue;

  node *node1;
  node1 = tree1.searchKey(keyValue);

  if(node1) std::cout << "There is such a key in the tree )\n";
  else std::cout << "There is no such key in the tree (\n";

  ///////////////////////////
  // Finding the minimum and maximum in a tree
  // Поиск минимума и максимума в дереве

  std::cout << "\nMinimum value in the tree: " << tree1.min();
  std::cout << "\nMaximum value in the tree: " << tree1.max() << std::endl;

///////////////////////////
// Checking the deletion of a key in the tree
// Проверка удаления ключа в дереве

  std::cout << "Input a key to delete, please: ";
  std::cin >> keyValue;

  tree1.keyDeleting(keyValue);

  std::cout << "Tree after deleting key " << keyValue << std::endl;
  tree1.printTree();

  ///////////////////////////
  // Tree traversal in depth
  // Обход дерева в глубину (центрированный)

  int nodesNumber, *traversalResult;
  nodesNumber = tree1.numberOfNodes();
  traversalResult = new int[nodesNumber];
  tree1.traversalInDepth(traversalResult);

  std::cout << "\nResult of the tree traversal in depth: \n";
  for(int i = 0; i < nodesNumber; i++) {
    std::cout << traversalResult[i] << " ";
  }

  ///////////////////////////
  // Tree traversal in width
  // Обход дерева в ширину


  std::cout << "\nResult of the tree traversal in width: \n";
  tree1.traversalInWidth(traversalResult);

  for(int i = 0; i < nodesNumber; i++) {
    std::cout << traversalResult[i] << " ";
  }
  std::cout << std::endl;
  delete []traversalResult;

}
