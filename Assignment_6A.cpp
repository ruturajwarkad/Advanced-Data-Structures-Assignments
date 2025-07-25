#include <iostream>
#include <queue>
using namespace std;

const int T = 2;  // Minimum degree (Order 3 -> T = 2)

class BTreeNode {
public:
    int keys[2 * T - 1];
    BTreeNode* children[2 * T];
    int n;
    bool leaf;

    BTreeNode(bool leaf);
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode* y);
    void traverseLevelOrder();
    void remove(int k);
    int findKey(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx);
    int getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    friend class BTree;
};

class BTree {
    BTreeNode* root;
public:
    BTree() { root = nullptr; }
    void insert(int k);
    void levelOrder();
    void remove(int k);
};

// ----------------- BTreeNode Implementation -----------------------

BTreeNode::BTreeNode(bool leaf1) {
    leaf = leaf1;
    n = 0;
    for (int i = 0; i < 2 * T; i++) children[i] = nullptr;
}

void BTree::insert(int k) {
    if (!root) {
        root = new BTreeNode(true);
        root->keys[0] = k;
        root->n = 1;
    }
    else {
        if (root->n == 2 * T - 1) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0] < k) i++;
            s->children[i]->insertNonFull(k);
            root = s;
        }
        else root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    }
    else {
        while (i >= 0 && keys[i] > k)
            i--;
        if (children[i + 1]->n == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k)
                i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->keys[j] = y->keys[j + T];

    if (!y->leaf) {
        for (int j = 0; j < T; j++)
            z->children[j] = y->children[j + T];
    }

    y->n = T - 1;
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[T - 1];
    n++;
}

void BTree::levelOrder() {
    if (!root) {
        cout << "Tree is empty.\n";
        return;
    }

    queue<BTreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        while (sz--) {
            BTreeNode* node = q.front(); q.pop();
            cout << "[ ";
            for (int i = 0; i < node->n; i++)
                cout << node->keys[i] << " ";
            cout << "] ";
            if (!node->leaf)
                for (int i = 0; i <= node->n; i++)
                    if (node->children[i]) q.push(node->children[i]);
        }
        cout << endl;
    }
}

// ----------------- Delete Logic -----------------------

int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;
    return idx;
}

void BTree::remove(int k) {
    if (!root) {
        cout << "Tree is empty.\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode* tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];
        delete tmp;
    }
}

void BTreeNode::remove(int k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
        if (leaf) removeFromLeaf(idx);
        else removeFromNonLeaf(idx);
    }
    else {
        if (leaf) {
            cout << "Key " << k << " not found.\n";
            return;
        }

        bool flag = (idx == n);
        if (children[idx]->n < T)
            fill(idx);
        if (flag && idx > n)
            children[idx - 1]->remove(k);
        else
            children[idx]->remove(k);
    }
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    if (children[idx]->n >= T) {
        int pred = getPred(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    }
    else if (children[idx + 1]->n >= T) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    }
    else {
        merge(idx);
        children[idx]->remove(k);
    }
}

int BTreeNode::getPred(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf)
        cur = cur->children[cur->n];
    return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->n >= T)
        borrowFromPrev(idx);
    else if (idx != n && children[idx + 1]->n >= T)
        borrowFromNext(idx);
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf)
        for (int i = child->n; i >= 0; --i)
            child->children[i + 1] = child->children[i];

    child->keys[0] = keys[idx - 1];
    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];
    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys[child->n] = keys[idx];
    if (!child->leaf)
        child->children[child->n + 1] = sibling->children[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf)
        for (int i = 1; i <= sibling->n; ++i)
            sibling->children[i - 1] = sibling->children[i];

    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys[T - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + T] = sibling->keys[i];
    if (!child->leaf)
        for (int i = 0; i <= sibling->n; ++i)
            child->children[i + T] = sibling->children[i];

    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    for (int i = idx + 2; i <= n; ++i)
        children[i - 1] = children[i];

    child->n += sibling->n + 1;
    n--;

    delete sibling;
}

// ----------------- MAIN FUNCTION FOR TESTING -----------------------

int main() {
    BTree t;

    int choice, val;
    do {
        cout << "\n--- B-Tree Menu ---\n";
        cout << "1. Insert\n2. Delete\n3. Level Order Display\n4. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter value to insert: ";
            cin >> val;
            t.insert(val);
            break;
        case 2:
            cout << "Enter value to delete: ";
            cin >> val;
            t.remove(val);
            break;
        case 3:
            cout << "Level order display:\n";
            t.levelOrder();
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
