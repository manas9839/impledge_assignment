 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>

class TrieNode {
public:
    char key;
    TrieNode* parent;
    std::unordered_map<char, TrieNode*> children;
    bool end;

    TrieNode(char key) : key(key), parent(nullptr), end(false) {}
};

class Trie {
public:
    Trie() {
        root = new TrieNode('\0');
    }

    void insert(std::string word) {
        TrieNode* node = root;

        for (size_t index = 0; index < word.length(); ++index) {
            char c = word[index];
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode(c);
                node->children[c]->parent = node;
            }
            node = node->children[c];

            if (index == word.length() - 1) {
                node->end = true;
            }
        }
    }

    std::vector<std::string> find(std::string prefix) {
        TrieNode* node = root;
        std::vector<std::string> output;

        for (size_t index = 0; index < prefix.length(); ++index) {
            char c = prefix[index];
            if (node->children.find(c) != node->children.end()) {
                node = node->children[c];
            } else {
                return output;
            }
        }
        findAllWords(node, output);

        return output;
    }

private:
    TrieNode* root;

    void findAllWords(TrieNode* node, std::vector<std::string>& arr) {
        if (node->end) {
            arr.insert(arr.begin(), node->getWord());
        }

        for (const auto& child : node->children) {
            findAllWords(child.second, arr);
        }
    }
};

bool wordChecker(const std::string& word, Trie& trie) {
    size_t index = 1;

    while (index <= word.length()) {
        const size_t length = trie.find(word.substr(0, index)).size();

        if (!(length > 1) && index == 1) {
            return false;
        } else if (!(length > 1)) {
            return wordChecker(word.substr(index - 1, word.length()), trie);
        } else {
            index++;
        }
    }
    return true;
}

std::string LongestString(const std::vector<std::string>& arr) {
    std::string longestString = *std::max_element(arr.begin(), arr.end(),
        [](const std::string& x, const std::string& y) { return x.length() < y.length(); });
    return longestString;
}

std::vector<std::string> searchConcatWords(const std::string& file, int noOfResults) {
    Trie trie;
    std::vector<std::string> result;

    std::ifstream inputFile(file);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            trie.insert(line);
        }
        inputFile.close();

        std::vector<std::string> text(trie.find(""));
        while (noOfResults > 0 && !text.empty()) {
            std::string longestString = LongestString(text);
            text.erase(std::remove(text.begin(), text.end(), longestString), text.end());

            if (wordChecker(longestString, trie)) {
                result.push_back(longestString);
                noOfResults--;
            }
        }
    }
    return result;
}

int main() {
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> result1 = searchConcatWords("./Input_01.txt", 2);
    auto end1 = std::chrono::high_resolution_clock::now();

    std::cout << result1[0] << ' ' << result1[1] << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << "ms" << '\n';

    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> result2 = searchConcatWords("./Input_02.txt", 2);
    auto end2 = std::chrono::high_resolution_clock::now();

    std::cout << result2[0] << ' ' << result2[1] << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << "ms" << '\n';

    return 0;
}