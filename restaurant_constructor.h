#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <cmath>
#include <time.h>


//Hello! Ashley Peters here again! This is where most of the behind-the-scenes magic happens.
//Ok not really but hey, red black trees!

using namespace std;

//Restaurant struct
struct restStruct {
  string title;
  string state;
  string cuisine;
  string diet;
  string county;
  string price;
  int rating;
  bool fitFilters;
  bool favorited;
};

//Map struct with the map key being the title
struct mapStruct {
  string title;
  int color;

  restStruct restaurant;

  mapStruct *parent;
  mapStruct *left;
  mapStruct *right;
};

//Struct set with no real "key" since we just insert
//in the order items are read in.
struct setStruct{
  int color;
  int order_num;

  restStruct restaurant;

  setStruct *parent;
  setStruct *left;
  setStruct *right;
};

//Map tree class
class mapTree {
  //As far as I'm aware, I had to put everything in publicly
  //because I was getting errors otherwise. Sorry! No hidden secrets today
  public:
    //Declare the root
    mapStruct* root;
    mapTree() {
      root = nullptr;
    }

    //Functions!
    void mapBalance(mapStruct* node);
    void mapRotateR(mapStruct* node);
    void mapRotateL(mapStruct* node);
    void mapInorder(mapStruct* root, int &count);
    void mapInsert(string title, restStruct restaurant);

    void filteredInorderPrint(mapStruct* root, int &count);
    void favoritesInorderPrint(mapStruct* root, int &count);
    void searchedPrint(string name, mapStruct* root, int &count);

    void updateFitFilters(mapStruct* root, vector<pair<string, string>> &filters);
    void setFavorite(mapStruct* node, int index, int &count);
};

//Set tree class
class setTree {
  public:
    //Declare the root
    setStruct* root;
    setTree() {
      root = nullptr;
    }

    //More functions!
    void setBalance(setStruct* node);
    void setRotateR(setStruct* node);
    void setRotateL(setStruct* node);
    void setInorder(setStruct* root);
    void setInsert(int order_num, restStruct restaurant);

    void filteredInorderPrint(setStruct* root, int &count);
    void favoritesInorderPrint(setStruct* root, int &count);
    void searchedPrint(string name, setStruct* root, int &count);

    void updateFitFilters(setStruct* root, vector<pair<string, string>> &filters);
    void setFavorite(setStruct* node, int index, int &count);
};

//Moving forward, only the logic for the map Red-Black tree is going to be commented
//Since the set Red-Black tree is exactly the same except it compares the order number
//instead of the restaurant name.

//Balance function. Referenced Module 4: Balanced Trees, slide 145
void mapTree::mapBalance(mapStruct* node) {
  //If the parent is null, then set the color to black
  //Roots must always be black!
  if (node->parent == nullptr) {
    node->color = 0;
    return;
  }
  //If the color of the parent is black, then
  //since the nodes are initialized to red, we're good!
  if (node->parent->color == 0) {
    return;
  }

  //Grab the parent and the grandparent. We'll grab Uncle Oscar in a second
  mapStruct* parent = node->parent;
  mapStruct* grand = parent->parent;
  mapStruct* uncle;

  //If the parent is the grandparent's left child,
  //then the uncle will be the grandparent's right child
  if (parent == grand->left) {
    uncle = grand->right;
  }
  //Otherwise, the uncle is the left child of the parent
  else if (parent == grand->right) {
    uncle = grand->left;
  }
  //Now that we have Uncle Oscar, if he isn't null and his color is red,
  //we need to change our parent and his color to black
  //and then color Grandma red.
  //Afterwards, we need to do a balance on Grandma. She lost her cane!
  if (uncle != nullptr && uncle->color == 1) {
    parent->color = uncle->color = 0;
    grand->color = 1;
    mapBalance(grand);
    return;
  }
  //If the current node is the parent's right child and the parent is the
  //grandparent's left child, then we need a left rotation on the parent
  if (node == parent->right && parent == grand->left) {
    mapRotateL(parent);
    node = parent;
    parent = node->parent;
  }
  //If the node is the parent's left child and the parent is
  //the grandparent's right child, we need a right rotation and all that jazz
  else if (node == parent->left && parent == grand->right) {
    mapRotateR(parent);
    node = parent;
    parent = node->parent;
  }
  //Set the parent's color to black
  //And grandma's color to red
  parent->color = 0;
  grand->color = 1;
  //If the node is the parent's left child, we need a right rotation
  //On grandma. Otherwise, it's a left rotation
  if (node == parent->left) {
    mapRotateR(grand);
  }
  else {
    mapRotateL(grand);
  }
}

//Right rotation time!
void mapTree::mapRotateR(mapStruct* node) {
  //The new parent is going to be the current node's left child
  mapStruct* newParent = node->left;
  //The node's left child becomes the new parent's right child
  node->left = newParent->right;
  //If the new parent's new right child is not null,
  //then the new parent's right child's new... well, parent, becomes our node
  if (newParent->right != nullptr) {
    newParent->right->parent = node;
  }
  //The new parent's parent becomes the node's parent
  newParent->parent = node->parent;
  //If the node's parent is null,
  //Then the new parent becomes the root. KING OF THE HILL!
  if (node->parent == nullptr) {
    root = newParent;
  }
  //Otherwise, if the node is the parent's right child,
  //then the parent's new right child is the new parent. How the hecc does that work?
  else if (node == node->parent->right) {
    node->parent->right = newParent;
  }
  //Other-otherwise, the parent's left child becomes the new parent - 
  //... Maybe I shouldn't have called it the new parent...
  else {
    node->parent->left = newParent;
  }
  //The new parent's right child is the node
  //And the node's parent is the new parent. IT IS THE NEW PARENT
  //Do you ever look at a word and, the more you look at it, the weirder it looks?
  //Me with "parent".
  newParent->right = node;
  node->parent = newParent;
}

//Left rotate time! ... Do I really need to comment all this out?
//Just swap right and left from the above function and there you go!
void mapTree::mapRotateL(mapStruct* node) {
  mapStruct* newParent = node->right;
  node->right = newParent->left;
  if (newParent->left != nullptr) {
    newParent->left->parent = node;
  }
  newParent->parent = node->parent;
  if (node->parent == nullptr) {
    root = newParent;
  }
  else if (node == node->parent->left) {
    node->parent->left = newParent;
  }
  else {
    node->parent->right = newParent;
  }
  newParent->left = node;
  node->parent = newParent;
}

//In order!
void mapTree::mapInorder(mapStruct* node, int &count) {
  if (node == nullptr) {
    return;
  }
  mapInorder(node->left, count);
  count++;
    cout << count << ". " << node->restaurant.title;
    if (node->restaurant.favorited) {
        cout << ": Favorited!";
    }
    cout << endl;
    for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
        cout << "-"; //essentially underlines title
    }
    int num = count;
    while (num > 0) {
        num /= 10;
        cout << "-"; //essentially underlines each digit in the count
    }
    cout << endl;

    cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

    cout << "Pricing: ";
    if (node->restaurant.price == "B") {
        cout << "Budget Friendly" << endl;
    }
    else if (node->restaurant.price == "A") {
        cout << "Average" << endl;
    }
    else if (node->restaurant.price == "E") {
        cout << "Expensive" << endl;
    }

    cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

    cout << "Dietary Accomodations: ";
    if (node->restaurant.diet == "V") {
        cout << "Vegan options available" << endl;
    }
    else if (node->restaurant.diet == "P") {
        cout << "Plant-based (vegetarian) options available" << endl;
    }
    else if (node->restaurant.diet == "K") {
        cout << "Keto options available" << endl;
    }
    else if (node->restaurant.diet == "PV") {
        cout << "Vegan and Plant-based (vegetarian) options available" << endl;
    }
    else if (node->restaurant.diet == "PK") {
        cout << "Plant-based (vegetarian) and Keto options available" << endl;
    }
    else if (node->restaurant.diet == "VK") {
        cout << "Vegan and Keto options available" << endl;
    }
    else if (node->restaurant.diet == "PVK") {
        cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
    }

    cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

    cout << endl;
    mapInorder(node->right, count);
}

void mapTree::filteredInorderPrint(mapStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    filteredInorderPrint(node->left, count);
    if (node->restaurant.fitFilters) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    filteredInorderPrint(node->right, count);
}

void mapTree::favoritesInorderPrint(mapStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    favoritesInorderPrint(node->left, count);
    if (node->restaurant.favorited) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    favoritesInorderPrint(node->right, count);
}

void mapTree::searchedPrint(string name, mapStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    searchedPrint(name, node->left, count);
    if (node->restaurant.title.find(name) != -1) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    searchedPrint(name, node->right, count);
}

void mapTree::setFavorite(mapStruct* node, int index, int &count) {
    //using a given index, traverse until at the index to set it as a favorite
    if (node == nullptr) {
        return;
    }
    setFavorite(node->left, index, count);
    count++;
    if (count == index) {
        node->restaurant.favorited = true;
    }
    setFavorite(node->right, index, count);
}

void mapTree::updateFitFilters(mapStruct* node, vector<pair<string, string>> &filters) {
    //checks all nodes to assure they fit all filters (state, county, price, diet, cuisine)
    if (node == nullptr) {
        return;
    }
    updateFitFilters(node->left, filters);
    for (int i = 0; i < filters.size(); i++) {
        if (filters.at(i).first == "State") {
            if (node->restaurant.state != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "County") {
            if (node->restaurant.county != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Diet") {
            if (node->restaurant.diet.find(filters.at(i).second) == -1) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Price") {
            if (node->restaurant.price != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Cuisine") {
            if (node->restaurant.cuisine != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
    }
    if (filters.size() == 0) {
        node->restaurant.fitFilters = true;
    }
    updateFitFilters(node->right, filters);
}


//Insert time!
void mapTree::mapInsert(string title, restStruct restaurant) {
  mapStruct* parent;
  mapStruct* prev;
  mapStruct* newNode = new mapStruct;

  //Assign all the values properly and set the color to red!
  newNode->title = title;
  newNode->color = 1;
  newNode->restaurant = restaurant;
  newNode->left = nullptr;
  newNode->right = nullptr;
  parent = root;
  prev = nullptr;

  //If the root is null, then the new node is the root and there is no parent!
  //... Oh wait that sounds sad...
  if (root == nullptr) {
    root = newNode;
    newNode->parent = nullptr;
  }
  //Otherwise,
  else {
    //While the parent isn't null
    while (parent != nullptr) {
      //The previous node is the parent
      prev = parent;
      //If the parent's title is less than the new node's title alphabetically,
      //Go to the right
      if (parent->title < newNode->title) {
        parent = parent->right;
      }
      //Otherwise, go to the left
      else {
        parent = parent->left;
      }
    }
    //If we're here, then the previous node is the last not-null node
    //So the new node's parent is the previous node.
    newNode->parent = prev;
    //If the previous node's title is less than the new node's title alphabetically,
    //Then the new node is the previous node's right child
    if (prev->title < newNode->title) {
      prev->right = newNode;
    }
    //Otherwise, the new node is the previous node's left child
    else {
      prev->left = newNode;
    }
  }
  //It's balance time!
  mapBalance(newNode);
}

//And then after this is just everything but for the set
//For the insert, change the title comparison to the order comparison and we're done.
void setTree::setBalance(setStruct* node) {
  if (node->parent == nullptr) {
    node->color = 0;
    return;
  }
  if (node->parent->color == 0) {
    return;
  }
  setStruct* parent = node->parent;
  setStruct* grand = parent->parent;
  setStruct* uncle;
  if (parent == grand->left) {
    uncle = grand->right;
  }
  else if (parent == grand->right) {
    uncle = grand->left;
  }
  if (uncle != nullptr && uncle->color == 1) {
    parent->color = uncle->color = 0;
    grand->color = 1;
    setBalance(grand);
    return;
  }
  if (node == parent->right && parent == grand->left) {
    setRotateL(parent);
    node = parent;
    parent = node->parent;
  }
  else if (node == parent->left && parent == grand->right) {
    setRotateR(parent);
    node = parent;
    parent = node->parent;
  }
  parent->color = 0;
  grand->color = 1;
  if (node == parent->left) {
    setRotateR(grand);
  }
  else {
    setRotateL(grand);
  }
}

void setTree::setRotateR(setStruct* node) {
  setStruct* newParent = node->left;
  node->left = newParent->right;
  if (newParent->right != nullptr) {
    newParent->right->parent = node;
  }
  newParent->parent = node->parent;
  if (node->parent == nullptr) {
    root = newParent;
  }
  else if (node == node->parent->right) {
    node->parent->right = newParent;
  }
  else {
    node->parent->left = newParent;
  }
  newParent->right = node;
  node->parent = newParent;
}

void setTree::setRotateL(setStruct* node) {
  setStruct* newParent = node->right;
  node->right = newParent->left;
  if (newParent->left != nullptr) {
    newParent->left->parent = node;
  }
  newParent->parent = node->parent;
  if (node->parent == nullptr) {
    root = newParent;
  }
  else if (node == node->parent->left) {
    node->parent->left = newParent;
  }
  else {
    node->parent->right = newParent;
  }
  newParent->left = node;
  node->parent = newParent;
}

void setTree::setInorder(setStruct* node) {
  if (node == nullptr) {
    return;
  }
  setInorder(node->left);
  cout << node->restaurant.title << endl;
  setInorder(node->right);
}

void setTree::filteredInorderPrint(setStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    filteredInorderPrint(node->left, count);
    if (node->restaurant.fitFilters) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    filteredInorderPrint(node->right, count);
}

void setTree::favoritesInorderPrint(setStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    favoritesInorderPrint(node->left, count);
    if (node->restaurant.favorited) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    favoritesInorderPrint(node->right, count);
}

void setTree::searchedPrint(string name, setStruct* node, int &count) {
    if (node == nullptr) {
        return;
    }
    searchedPrint(name, node->left, count);
    if (node->restaurant.title.find(name) != -1) {
        count++;
        cout << count << ". " << node->restaurant.title;
        if (node->restaurant.favorited) {
            cout << ": Favorited!";
        }
        cout << endl;
        for (int i = 0; i < node->restaurant.title.size() + 2; i++) {
            cout << "-"; //essentially underlines title
        }
        int num = count;
        while (num > 0) {
            num /= 10;
            cout << "-"; //essentially underlines each digit in the count
        }
        cout << endl;

        cout << "Cuisine Type: " << node->restaurant.cuisine << endl;

        cout << "Pricing: ";
        if (node->restaurant.price == "B") {
            cout << "Budget Friendly" << endl;
        }
        else if (node->restaurant.price == "A") {
            cout << "Average" << endl;
        }
        else if (node->restaurant.price == "E") {
            cout << "Expensive" << endl;
        }

        cout << "Location: " << node->restaurant.state << ", " <<  node->restaurant.county << " County" << endl;

        cout << "Dietary Accomodations: ";
        if (node->restaurant.diet == "V") {
            cout << "Vegan options available" << endl;
        }
        else if (node->restaurant.diet == "P") {
            cout << "Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "K") {
            cout << "Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PV") {
            cout << "Vegan and Plant-based (vegetarian) options available" << endl;
        }
        else if (node->restaurant.diet == "PK") {
            cout << "Plant-based (vegetarian) and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "VK") {
            cout << "Vegan and Keto options available" << endl;
        }
        else if (node->restaurant.diet == "PVK") {
            cout << "Plant-based (vegetarian), Vegan, and Keto options available" << endl;
        }

        cout << "Rating: " << node->restaurant.rating << " star(s)" << endl;

        cout << endl;
    }
    searchedPrint(name, node->right, count);
}

void setTree::setFavorite(setStruct* node, int index, int &count) {
    //using a given index, traverse until at the index to set it as a favorite
    if (node == nullptr) {
        return;
    }
    setFavorite(node->left, index, count);
    count++;
    if (count == index) {
        node->restaurant.favorited = true;
    }
    setFavorite(node->right, index, count);
}

void setTree::updateFitFilters(setStruct* node, vector<pair<string, string>> &filters) {
    //checks all nodes to assure they fit all filters (state, county, price, diet, cuisine)
    if (node == nullptr) {
        return;
    }
    updateFitFilters(node->left, filters);
    for (int i = 0; i < filters.size(); i++) {
        if (filters.at(i).first == "State") {
            if (node->restaurant.state != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "County") {
            if (node->restaurant.county != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Diet") {
            if (node->restaurant.diet.find(filters.at(i).second) == -1) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Price") {
            if (node->restaurant.price != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
        else if (filters.at(i).first == "Cuisine") {
            if (node->restaurant.cuisine != filters.at(i).second) {
                node->restaurant.fitFilters = false;
            }
        }
    }
    if (filters.size() == 0) {
        node->restaurant.fitFilters = true;
    }
    updateFitFilters(node->right, filters);
}

void setTree::setInsert(int order_num, restStruct restaurant) {
  setStruct* parent;
  setStruct* prev;
  setStruct* newNode = new setStruct;
  newNode->order_num = order_num;
  newNode->color = 1;
  newNode->restaurant = restaurant;
  newNode->left = nullptr;
  newNode->right = nullptr;
  parent = root;
  prev = nullptr;
  if (root == nullptr) {
    root = newNode;
    newNode->parent = nullptr;
  }
  else {
    while (parent != nullptr) {
      prev = parent;
      if (parent->order_num < newNode->order_num) {
        parent = parent->right;
      }
      else {
        parent = parent->left;
      }
    }
    newNode->parent = prev;
    if (prev->order_num < newNode->order_num) {
      prev->right = newNode;
    }
    else {
      prev->left = newNode;
    }
  }
  setBalance(newNode);
}

//Restaurant data class
class RestaurantData {
  public:
    //This vector stores all our 50 states' abbreviations
    vector<string> states{"AL","AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY"};
    //"P" = Plant-based/vegetarian, "V" = Vegan, and "K" = Keto
    vector<string> dietaries{"P","V","K","PV","PK","VK","PVK"};
    //"B" = Budget, "A" = Average, "E" = Expensive
    vector<string> prices{"B","A","E"};

  //Find functions to find any of those stored values
  //They return true if found, and false if not.
  bool findState(string word);
  bool findDiet(string word);
  bool findPrice(string word);
};

bool RestaurantData::findState(string word) {
  if (find(states.begin(), states.end(), word) != states.end()) {
    return true;
  }
  return false;
}

bool RestaurantData::findDiet(string word) {
  if (find(dietaries.begin(), dietaries.end(), word) != dietaries.end()) {
    return true;
  }
  return false;
}

bool RestaurantData::findPrice(string word) {
  if (find(prices.begin(), prices.end(), word) != prices.end()) {
    return true;
  }
  return false;
}