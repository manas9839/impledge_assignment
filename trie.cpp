#include <iostream>
#include <map>
#include <vector>

/**
 * Nodes of the Trie
 */
class TrieNode {
public:
    char key;
    TrieNode* parent;
    std::map<char, TrieNode*> children;
    bool end;

    TrieNode(char key) : key(key), parent(nullptr), end(false) {}
};

/**
 * Trie
 */
class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    /**
     * Insertion Function
     */
    void insert(std::string word) {
        TrieNode* node = root;

        for (int index = 0; index < word.length(); index++) {
            if (node->children.find(word[index]) == node->children.end()) {
                node->children[word[index]] = new TrieNode(word[index]);
                node->children[word[index]]->parent = node;
            }
            node = node->children[word[index]];

            if (index == word.length() - 1) {
                node->end = true;
            }
        }
    }

    /**
     * Words Function
     */
    std::string getWord() {
        std::vector<char> output;
        TrieNode* node = this;

        while (node != nullptr) {
            output.insert(output.begin(), node->key);
            node = node->parent;
        }

        return std::string(output.begin(), output.end());
    }

    /**
     * Find All Words
     */
    void findAllWords(TrieNode* node, std::vector<std::string>& arr) {
        if (node->end) {
            arr.insert(arr.begin(), node->getWord());
        }

        for (const auto& child : node->children) {
            findAllWords(child.second, arr);
        }
    }

    /**
     * Find by prefix
     */
    std::vector<std::string> find(std::string prefix) {
        TrieNode* node = root;
        std::vector<std::string> output;

        for (int index = 0; index < prefix.length(); index++) {
            if (node->children.find(prefix[index]) != node->children.end()) {
                node = node->children[prefix[index]];
            } else {
                return output;
            }
        }
        findAllWords(node, output);

        return output;
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("appetizer");
    trie.insert("banana");
    trie.insert("bat");
    trie.insert("batman");

    std::vector<std::string> wordsWithPrefix = trie.find("bat");

    std::cout << "Words with prefix 'bat':" << std::endl;
    for (const std::string& word : wordsWithPrefix) {
        std::cout << word << std::endl;
    }

    return 0;
}