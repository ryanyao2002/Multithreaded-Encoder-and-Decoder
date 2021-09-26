//
//  main.cpp
//
//
//  Created by Ryan Yao on 9/25/21.
//

#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <unordered_map>
#include <thread>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

struct CharNode{
    CharNode* left;
    CharNode* right;
    char data;
    int freq;
};

int frequencies[256]{};
unordered_map<string, char>convertToChar;
unordered_map<char, string>convertToString;
void countFrequencies(){
    fstream infile;
    string curr;
    infile.open("Words.txt", ios::in);
    while(getline(infile, curr)){
        frequencies['\n']++;
        for(char c : curr){
            frequencies[c]++;
        }
    }
    infile.close();
}
void convertCodes(CharNode* root, string str)
{
 
    if (!root){
        return;
    }
 
    if (root->data != '$'){
        convertToChar[str] = root->data;
        convertToString[root->data] = str;
    }
 
    convertCodes(root->left, str + "0");
    convertCodes(root->right, str + "1");
}
void buildTree(){
    priority_queue<CharNode*, vector<CharNode*>, greater<>>pq;
    for(int i = 0; i < 256; i++){
        if(frequencies[i] > 0){
            CharNode* newNode = new CharNode();
            newNode->data = (char)i;
            newNode->freq = frequencies[i];
            pq.push(newNode);
        }
    }
    while(pq.size() > 1){
        CharNode* left = pq.top();
        pq.pop();
        CharNode* right = pq.top();
        pq.pop();
        
        CharNode* newNode = new CharNode();
        newNode->data = '$';
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }
    convertCodes(pq.top(), "");
    pq.pop();
}

void encode(){
    fstream infile;
    fstream outfile;
    string curr;
    infile.open("Words.txt", ios::in);
    outfile.open("WordsEncoded.txt", ios::out);
    while(getline(infile, curr)){
        string convertedString;
        for(char c : curr){
            convertedString += convertToString[c];
        }
        outfile<<convertedString +'\n';
    }
    outfile.close();
    infile.close();
}
void decodeText(vector<string>& text, vector<string>&outText, int start, int end){
    string cr = "";
    string s = "";
    for(int i = start; i <= end; i++){
        for(char c : text[i]){
            cr += c;
            if(convertToChar.count(cr)){
                s += convertToChar[cr];
                cr = "";
            }
        }
        outText[i] = s;
        s = "";
    }
}

void decode1(){
    fstream infile;
    fstream outfile;
    string curr;
    infile.open("WordsEncoded.txt", ios::in);
    outfile.open("WordsDecoded.txt", ios::out);
    vector<string>text;
    while(getline(infile, curr)){
        text.push_back(curr);
    }
    infile.close();
    vector<string>outText(text.size());
    string cr = "";
    string newS = "";
    for(int i = 0; i < text.size(); i++){
        string s = text[i];
        for(char c : s){
            cr += c;
            if(convertToChar.count(cr)){
                newS+=convertToChar[cr];
                cr = "";
            }
        }
        outText[i] = newS;
        newS = "";
    }
    for(string s : outText){
        outfile<<s<<'\n';
    }
}
void decode(){
    fstream infile;
    fstream outfile;
    string curr;
    infile.open("WordsEncoded.txt", ios::in);
    outfile.open("WordsDecoded.txt", ios::out);
    vector<string>text;
    while(getline(infile, curr)){
        text.push_back(curr);
    }
    infile.close();
    int third = (int)text.size()/3;
    int twoThird = 2*third;
    vector<string>outText(text.size());

    thread t1(ref(decodeText),ref(text), ref(outText), 0, third);
    thread t2(ref(decodeText),ref(text), ref(outText), third+1, (int)twoThird);
    thread t3(ref(decodeText),ref(text), ref(outText), twoThird+1, (int)text.size()-1);

    t1.join();
    t2.join();
    t3.join();
    for(string s : outText){
        outfile<<s<<'\n';
    }
}

int main() {
    // insert code here...
    countFrequencies();
    buildTree();
    encode();
    clock_t t;
    t = clock();
    decode1();
    t = clock()-t;
    cout<<(float)t/CLOCKS_PER_SEC<<endl;
    t = clock();
    decode();
    t = clock()-t;
    cout<<(float)t/CLOCKS_PER_SEC<<endl;
    
    return 0;
}
