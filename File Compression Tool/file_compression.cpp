#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
using namespace std;

// Huffman Tree Node
struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode *left, *right;
    
    HuffmanNode(char data, unsigned freq) : 
        data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparison for priority queue
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// Utility function to print Huffman codes
void printCodes(HuffmanNode* root, string str, unordered_map<char, string> &huffmanCode) {
    if (!root) return;
    
    if (root->data != '\0') {
        huffmanCode[root->data] = str;
    }
    
    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

// Build Huffman Tree and return root
HuffmanNode* buildHuffmanTree(const unordered_map<char, unsigned> &freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;
    
    // Create leaf nodes and push to min heap
    for (auto pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }
    
    // Build Huffman Tree
    while (minHeap.size() != 1) {
        HuffmanNode *left = minHeap.top(); minHeap.pop();
        HuffmanNode *right = minHeap.top(); minHeap.pop();
        
        HuffmanNode *top = new HuffmanNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    
    return minHeap.top();
}

// Generate Huffman codes
unordered_map<char, string> generateHuffmanCodes(HuffmanNode* root) {
    unordered_map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);
    return huffmanCode;
}

// Calculate frequency of each character in file
unordered_map<char, unsigned> calculateFrequencies(const string &filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    
    unordered_map<char, unsigned> freqMap;
    char ch;
    while (file.get(ch)) {
        freqMap[ch]++;
    }
    file.close();
    return freqMap;
}

// Compress file using Huffman coding
void compressFile(const string &inputFile, const string &outputFile) {
    // Step 1: Calculate frequencies
    auto freqMap = calculateFrequencies(inputFile);
    
    // Step 2: Build Huffman Tree
    HuffmanNode* root = buildHuffmanTree(freqMap);
    
    // Step 3: Generate Huffman codes
    auto huffmanCode = generateHuffmanCodes(root);
    
    // Step 4: Write compressed data
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    
    // Write header (frequency map for reconstruction)
    outFile << freqMap.size() << "\n";
    for (auto pair : freqMap) {
        outFile << (int)pair.first << " " << pair.second << "\n";
    }
    
    // Write compressed data
    string encodedStr = "";
    char ch;
    while (inFile.get(ch)) {
        encodedStr += huffmanCode[ch];
    }
    inFile.close();
    
    // Pad the string to make its length multiple of 8
    int padding = 8 - (encodedStr.length() % 8);
    for (int i = 0; i < padding; i++) {
        encodedStr += "0";
    }
    
    // Write padding info
    outFile << padding << "\n";
    
    // Convert binary string to bytes and write
    for (size_t i = 0; i < encodedStr.length(); i += 8) {
        string byteStr = encodedStr.substr(i, 8);
        bitset<8> bits(byteStr);
        char byte = static_cast<char>(bits.to_ulong());
        outFile.put(byte);
    }
    
    outFile.close();
    
    cout << "File compressed successfully!" << endl;
    cout << "Original size: " << encodedStr.length()/8 - padding/8 << " bytes" << endl;
    cout << "Compressed size: " << encodedStr.length()/8 << " bytes" << endl;
}

// Decompress file using Huffman coding
void decompressFile(const string &inputFile, const string &outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    
    // Read header (frequency map)
    int size;
    inFile >> size;
    inFile.ignore(); // Skip newline
    
    unordered_map<char, unsigned> freqMap;
    for (int i = 0; i < size; i++) {
        int ch;
        unsigned freq;
        inFile >> ch >> freq;
        inFile.ignore(); // Skip newline
        freqMap[static_cast<char>(ch)] = freq;
    }
    
    // Read padding
    int padding;
    inFile >> padding;
    inFile.ignore(); // Skip newline
    
    // Rebuild Huffman Tree
    HuffmanNode* root = buildHuffmanTree(freqMap);
    
    // Read compressed data
    string encodedStr = "";
    char byte;
    while (inFile.get(byte)) {
        encodedStr += bitset<8>(byte).to_string();
    }
    inFile.close();
    
    // Remove padding
    encodedStr = encodedStr.substr(0, encodedStr.length() - padding);
    
    // Decode using Huffman Tree
    HuffmanNode* current = root;
    for (char bit : encodedStr) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (current->left == nullptr && current->right == nullptr) {
            outFile.put(current->data);
            current = root;
        }
    }
    
    outFile.close();
    cout << "File decompressed successfully!" << endl;
}

// Display menu
void displayMenu() {
    cout << "\n📁 File Compression Tool" << endl;
    cout << "=======================" << endl;
    cout << "1. Compress File" << endl;
    cout << "2. Decompress File" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice (1-3): ";
}

int main() {
    int choice;
    string inputFile, outputFile;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                cout << "Enter input file name: ";
                getline(cin, inputFile);
                cout << "Enter output file name: ";
                getline(cin, outputFile);
                compressFile(inputFile, outputFile);
                break;
                
            case 2:
                cout << "Enter compressed file name: ";
                getline(cin, inputFile);
                cout << "Enter decompressed file name: ";
                getline(cin, outputFile);
                decompressFile(inputFile, outputFile);
                break;
                
            case 3:
                cout << "Exiting program..." << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
    } while (choice != 3);
    
    return 0;
}
