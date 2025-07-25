#include <iostream>
#include <string>
using namespace std;

struct Node {
    int player_id;
    int score;
    Node* left;
    Node* right;
    int height;

    Node(int id, int sc) {
        player_id = id;
        score = sc;
        left = right = nullptr;
        height = 1;
    }
};

int getHeight(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

class GameSystem {
private:
    Node* root;

    Node* insert(Node* node, int id, int score) {
        if (!node)
            return new Node(id, score);

        if (id < node->player_id)
            node->left = insert(node->left, id, score);
        else if (id > node->player_id)
            node->right = insert(node->right, id, score);
        else {
            cout << "Player ID already exists! Updating score.\n";
            node->score = score;
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && id < node->left->player_id)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && id > node->right->player_id)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && id > node->left->player_id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && id < node->right->player_id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* root, int id) {
        if (!root)
            return root;

        if (id < root->player_id)
            root->left = deleteNode(root->left, id);
        else if (id > root->player_id)
            root->right = deleteNode(root->right, id);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;

                delete temp;
            } else {
                Node* temp = minValueNode(root->right);
                root->player_id = temp->player_id;
                root->score = temp->score;
                root->right = deleteNode(root->right, temp->player_id);
            }
        }

        if (!root)
            return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        cout << "Player ID: " << node->player_id << " | Score: " << node->score << endl;
        inOrder(node->right);
    }

public:
    GameSystem() {
        root = nullptr;
    }

    void registerPlayer(int id, int score) {
        root = insert(root, id, score);
        cout << "Player registered or updated successfully.\n";
    }

    void displayLeaderboard() {
        cout << "\n--- Leaderboard (Sorted by Player ID) ---\n";
        if (!root)
            cout << "No players registered.\n";
        else
            inOrder(root);
    }

    void removePlayer(int id) {
        root = deleteNode(root, id);
        cout << "Player removed if existed.\n";
    }
};

int main() {
    GameSystem game;
    int choice, id, score;

    do {
        cout << "\nMultiplayer Game Menu:\n";
        cout << "1. Register Player\n";
        cout << "2. Display Leaderboard\n";
        cout << "3. Remove Player\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Player ID: ";
            cin >> id;
            cout << "Enter Score: ";
            cin >> score;
            game.registerPlayer(id, score);
            break;
        case 2:
            game.displayLeaderboard();
            break;
        case 3:
            cout << "Enter Player ID to remove: ";
            cin >> id;
            game.removePlayer(id);
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
