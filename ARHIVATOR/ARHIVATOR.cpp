#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <string> 
#include <sstream>

using namespace std;

FILE* f1;
FILE* f2;
FILE* f3;

struct Forest {
    char symbol;
    int weight;
    int root;
};
Forest forest[256];

struct Tree {
    char symbol = '/n';
    int left = -1;
    int right = -1;
    int parent = -1;
};
Tree tree[511];

//give back indexes of two least elements -> first for the least and second for next
pair <int, int> minimum(Forest* forest, int size) {
    long long min1 = 100000000000000 - 1;
    long long min2 = 100000000000000;
    int i2 = -1;
    int i1 = -1;
    for (int i = 0; i < size; i++) {
        if (forest[i].weight != -1) {
            if (forest[i].weight < min1) {
                min2 = min1;
                i2 = i1;
                min1 = forest[i].weight;
                i1 = i;
            }
            else if (forest[i].weight < min2) {
                min2 = forest[i].weight;
                i2 = i;
            }
        }
    }
    return pair <long long, long long> (i1, i2);
}

void deleting(Forest* forest, int i1, int i2, int& size_F, int& size_T) {
    forest[i1].weight += forest[i2].weight;
    forest[i1].root = size_T;
    forest[i2].weight = forest[size_F - 1].weight;
    forest[i2].root = forest[size_F - 1].root;
    forest[i1].symbol = '/n';
    forest[i2].symbol = forest[size_F - 1].symbol;
    size_F--;
    size_T++;
}
string finding_code(Tree* tree, int i, string& res) {
    if (tree[i].parent != -1) {
        if (i == tree[tree[i].parent].right) res.append("0");
        if (i == tree[tree[i].parent].left) res.append("1");
        finding_code(tree, tree[i].parent, res);
    }
    return res;
}

int main() {
    int freq[256];
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
    }

    f1 = fopen("abcde.txt", "rb");
    unsigned char ch;
   
    while (fscanf(f1, "%c", &ch) != -1) {
        freq[ch]++;
    }
    int size_forest = 0;
    int k = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            forest[k].root = k;
            forest[k].weight = freq[i];
            forest[k].symbol = i;
            size_forest++;
            k++;
        }
    }

    for (int i = 0; i < size_forest; i++) {
        cout << forest[i].symbol << " " << forest[i].root << " " << forest[i].weight << endl;
    }
    cout << endl;

    int size_tree = size_forest;
    long long index1, index2;
    while (size_forest > 1) {
        index1 = minimum(forest, size_forest).first;
        index2 = minimum(forest, size_forest).second;
        cout << forest[index1].weight << " " << forest[index2].weight << endl;

        tree[forest[index1].root].parent = size_tree;
        tree[forest[index1].root].symbol = forest[index1].symbol;

        tree[forest[index2].root].parent = size_tree;
        tree[forest[index2].root].symbol = forest[index2].symbol;

        tree[size_tree].right = forest[index1].root;
        tree[size_tree].left = forest[index2].root;
        tree[size_tree].parent = -1;
        tree[size_tree].symbol = '/n';

        deleting(forest, index1, index2, size_forest, size_tree);
    }

    for (int i = 0; i < size_tree; i++) {
        cout << tree[i].symbol << " " << tree[i].left << " " << tree[i].right << " " << tree[i].parent << endl;
    }
    
    map <char, string> word_to_code;
    map <string, char> code_to_word;
    string code = "";
    char key;
    string result = "";
    int c = 0;
    while (tree[c].symbol != '/n') {
        key = tree[c].symbol;
        code = finding_code(tree, c, result);
        reverse(code.begin(), code.end());
        word_to_code[key] = code;
        code_to_word[code] = key;

        c++;
        result = "";
    }

    f2 = fopen("zip.txt", "wb");
    while (fscanf(f1, "%c", &ch) != -1) {
        const char* a = word_to_code[ch].c_str();
        fputs(a, f2);
        fputs(" ", f2);
    }
    
    char uncoded[512];
    string word_decoded;
    f2 = fopen("zip.txt", "rb");
    f3 = fopen("unzip.txt", "wb");
    while (fgets(uncoded, sizeof(uncoded), f2)) {
        istringstream iss(uncoded);
        string word;
        while (iss >> word) {
            word_decoded = code_to_word[word];
            fputs(word_decoded.c_str(), f3);
        }
    }
    fclose(f2);
    fclose(f3);
    fclose(f1);
}
