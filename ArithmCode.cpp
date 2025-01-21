#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

// const int mx_value = 65535;
// const int First_qrt = (mx_value + 1) / 4;
// const int Half = First_qrt * 2;
// const int Third_qrt = First_qrt *3;

#define mx_value    65536
#define First_qrt   16384         
#define Half        32768
#define Third_qrt   49152
#define BREAK_SYMBOL    '$'

vector<int> freq_table; // Таблица частот
vector<char> alphabet; // неупорядочное множество

vector<char> MakeAlphabet(const string& DataFile) {
    vector<char> alphabet;
    
    for (auto c : DataFile) {
        if (find(alphabet.begin(), alphabet.end(), c) == alphabet.end()) {
            alphabet.push_back(c);
        }
    }
    alphabet.push_back(BREAK_SYMBOL);
    return alphabet;
}

vector<int> MakeFrequency(const string& DataFile){
    vector<char> alphabet = MakeAlphabet(DataFile);
    vector<int> freq_table(alphabet.size());
    
    for (int i = 0; i < alphabet.size(); i++){
        for (auto c : DataFile){
            if (alphabet[i] == c) freq_table[i]++;
        }
        if (i > 0) freq_table[i] += freq_table[i-1];
    }
    
    return freq_table;
}

// void BitsPlusFollow(string& data, bool bit, int bits_to_follow) {
//     data += to_string(bit);
    
//     while (bits_to_follow > 0) {
//         data += to_string(!bit);
//         bits_to_follow--;
//     }
// }

void BitsPlusFollow(string& data, bool bit, int& bits_to_follow) {
    data += (bit ? "1" : "0"); // Добавляем основной бит
    while (bits_to_follow > 0) { 
        data += (!bit ? "1" : "0"); // Добавляем противоположный бит
        bits_to_follow--;
    }
}


int IndexForSymbol(char c, const vector<char>& alphabet) {
    auto it = find(alphabet.begin(), alphabet.end(), c);
    if (it != alphabet.end()) {
        return distance(alphabet.begin(), it);
    }
    return -1; // символ не найден
}


string Compress(const string& DataFile, const vector<int>& freq_table, const vector<char>& alphabet) {
    string encode = "";
    int delitel = freq_table.back();
    int bits_to_follow = 0;
    char c;
    
    int l = 0, h = mx_value - 1;
    
    for (char c : DataFile) {
        
        int j = IndexForSymbol(c, alphabet);
        if (j == -1) {
            cout << "Error: Symbol not in alphabet!" << endl;
            exit(1);
        }
        
        int l_n = l + freq_table[j-1] * (h - l + 1) / delitel;
        int h_n = l + freq_table[j] * (h - l + 1) / delitel - 1;
        
        while (true) {
            if (h_n < Half) {
                BitsPlusFollow(encode, 0, bits_to_follow);
                bits_to_follow = 0;
            } else if(l_n >= Half) {
                BitsPlusFollow(encode, 1, bits_to_follow);
                bits_to_follow = 0;
                l_n -= Half; 
                h_n -= Half;
            } else if((l_n >= First_qrt) && (h_n < Third_qrt)) {
                bits_to_follow++;
                l_n -= First_qrt; 
                h_n -= First_qrt;
            } else break;
            
            l_n *= 2; 
            h_n = h_n * 2 + 1;
        }
        
        l = l_n;
        h = h_n;
    }
    
    return encode;
}

int Read16Bit(const string& encode, int lenght, int& b_count){
    int value;
    int mask = 1;
    
    for (int i = 15; i >= 0; i--){
        if (i < lenght) {
            if (encode[i] == '1'){
                value = value | (mask << b_count);
            }
            b_count++;
            mask = 1;
        }
    }
    
    return value;
    
}

int AddBit(const string& encode, int value, int cur, bool& flag) {
    bitset<16> a(value);

    if (flag == 1) {
        a.reset(0);
    }
    else if (cur >= encode.length()) {
        a.set(0);
        flag = 1;
    }
    else if (encode[cur] == '1') {
        a.set(0);
    }
    else if (encode[cur] == '0') {
        a.reset(0);
    }
    value = (int)(a.to_ulong());
    return value;

}

string Decompress(const string& encode, const vector<int>& freq_table, const vector<char>& alphabet) {
    string decode = "";
    int lenght = encode.length();
    
    int l = 0, h = mx_value - 1;
    int freq = 0, delitel = freq_table.back();
    
    int b_count = 0;
    int value = Read16Bit(encode, lenght, b_count);
    int tail = 16 - b_count;
    for (int i = 0; i < tail; i++) value *= 2;
    bool flag = false;
    
    for (int i = 1; i < delitel; i++){ //i=1
        freq = ((value - l + 1) * delitel - 1) / (h - l + 1);
        int j;
        for (j = 1; freq_table[j] <= freq; j++);
        
        decode += alphabet[j - 1];
        
        int l_n = l + freq_table[j-1] * (h - l + 1) / delitel;
        int h_n = l + freq_table[j] * (h - l + 1) / delitel - 1;
        
        if (alphabet[j] == BREAK_SYMBOL) return decode;
        
        for (;;) {
            if (h_n < Half) {
            } else if (l_n >= Half) {
                l_n -= Half;
                h_n -= Half;
                value -= Half;
            } else if (l_n >= First_qrt && h_n < Third_qrt) {
                l_n -= First_qrt;
                h_n -= First_qrt;
                value -= First_qrt;
            } else break;
            
            l *= 2;
            h = h * 2 + 1;
            
            value = AddBit(encode, 2 * value, b_count, flag);
            b_count++;
        }
        
        l = l_n;
        h = h_n;
    }
    
    return decode;
}

void write_binary(ofstream& out, int number) {
    
    bitset<32> binary(number);
    string b_str = binary.to_string();
    
    unsigned char byte = 0;
    unsigned char mask = 1;
    int cnt;
    
    for (int i = 0; i < b_str.length(); i++){
        byte <<= mask;
        
        if (b_str[i] == '1'){
            byte |= mask;
        }
        cnt++;
        if (cnt == 8) {
            out.put(byte);
            cnt = 0;
            byte = 0;
        }
    }
}

int read_binary(ifstream& in) {
    unsigned char mask = 1;
    int result = 0;
    string b_str = "";

    for (int i = 0; i < 4; i++) {
        unsigned char a = in.get();
        bitset<8> b(a);
        b_str += b.to_string();
    }

    bitset<32> tmp(b_str);

    return tmp.to_ullong();
}

