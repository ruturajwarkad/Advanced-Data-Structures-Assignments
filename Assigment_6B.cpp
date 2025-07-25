#include <iostream>
#include <string>
using namespace std;

enum Color { RED, BLACK };

struct DirectoryNode {
    string name;
    Color color;
    DirectoryNode *left, *right, *parent;

    DirectoryNode(string n) {
        name = n;
        color = RED;
        left = right = parent = nullptr;
    }
};

class FileSystem {
private:
    DirectoryNode* root;
    DirectoryNode* NIL;

    void leftRotate(DirectoryNode* x);
    void rightRotate(DirectoryNode* y);
    void insertFix(DirectoryNode* z);
    void deleteFix(DirectoryNode* x);
    void transplant(DirectoryNode* u, DirectoryNode* v);
    DirectoryNode* treeMinimum(DirectoryNode* x);
    void inorder(DirectoryNode* node);
    DirectoryNode* search(DirectoryNode* node, const string& name);

public:
    FileSystem();
    void createDirectory(string name);
    void deleteDirectory(string name);
    void displayDirectories();
};

FileSystem::FileSystem() {
    NIL = new DirectoryNode("");
    NIL->color = BLACK;
    root = NIL;
}

void FileSystem::leftRotate(DirectoryNode* x) {
    DirectoryNode* y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void FileSystem::rightRotate(DirectoryNode* y) {
    DirectoryNode* x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

void FileSystem::createDirectory(string name) {
    DirectoryNode* z = new DirectoryNode(name);
    z->left = z->right = NIL;

    DirectoryNode* y = nullptr;
    DirectoryNode* x = root;

    while (x != NIL) {
        y = x;
        if (z->name < x->name)
            x = x->left;
        else if (z->name > x->name)
            x = x->right;
        else {
            cout << "Directory already exists.\n";
            return;
        }
    }

    z->parent = y;
    if (y == nullptr)
        root = z;
    else if (z->name < y->name)
        y->left = z;
    else
        y->right = z;

    z->color = RED;
    insertFix(z);
    cout << "Directory '" << name << "' created.\n";
}

void FileSystem::insertFix(DirectoryNode* z) {
    while (z->parent && z->parent->color == RED) {
        DirectoryNode* gp = z->parent->parent;
        if (z->parent == gp->left) {
            DirectoryNode* y = gp->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                gp->color = RED;
                rightRotate(gp);
            }
        } else {
            DirectoryNode* y = gp->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                gp->color = RED;
                leftRotate(gp);
            }
        }
    }
    root->color = BLACK;
}

DirectoryNode* FileSystem::treeMinimum(DirectoryNode* x) {
    while (x->left != NIL)
        x = x->left;
    return x;
}

void FileSystem::transplant(DirectoryNode* u, DirectoryNode* v) {
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

void FileSystem::deleteDirectory(string name) {
    DirectoryNode* z = search(root, name);
    if (z == NIL) {
        cout << "Directory '" << name << "' not found.\n";
        return;
    }

    DirectoryNode* y = z;
    DirectoryNode* x;
    Color yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = treeMinimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK)
        deleteFix(x);

    delete z;
    cout << "Directory '" << name << "' deleted.\n";
}

void FileSystem::deleteFix(DirectoryNode* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            DirectoryNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            DirectoryNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->color = BLACK;
}

DirectoryNode* FileSystem::search(DirectoryNode* node, const string& name) {
    if (node == NIL || name == node->name)
        return node;

    if (name < node->name)
        return search(node->left, name);
    else
        return search(node->right, name);
}

void FileSystem::inorder(DirectoryNode* node) {
    if (node != NIL) {
        inorder(node->left);
        cout << node->name << (node->color == RED ? " [R]" : " [B]") << endl;
        inorder(node->right);
    }
}

void FileSystem::displayDirectories() {
    if (root == NIL) {
        cout << "File system is empty.\n";
        return;
    }
    cout << "Directory Structure (Inorder):\n";
    inorder(root);
}

// ----------------- MAIN FUNCTION -----------------------

int main() {
    FileSystem fs;
    int choice;
    string name;

    do {
        cout << "\n--- File System Menu ---\n";
        cout << "1. Create Directory\n2. Delete Directory\n3. Display Structure\n4. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter directory name: ";
            getline(cin, name);
            fs.createDirectory(name);
            break;
        case 2:
            cout << "Enter directory name to delete: ";
            getline(cin, name);
            fs.deleteDirectory(name);
            break;
        case 3:
            fs.displayDirectories();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
