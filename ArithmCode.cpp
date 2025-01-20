#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

const int mx_value = 65535;
const int First_qrt = (mx_value + 1) / 4;
const int Half = First_qrt * 2;
const int Third_qrt = First_qrt *3;

int l = 0, h = mx_value, bits_to_follow = 0;
vector<int> freq_table; // Таблица частот
vector<char> alphabet;

void BitsPlusFollow(string& data, bool bit, int bits_to_follow) {
    data += to_string(bit);
    
    while (bits_to_follow > 0) {
        data += to_string(!bit);
        bits_to_follow--;
    }
}

int IndexForSymbol(char c; const vector<char>& alphabet) {
    for (int i = 0; i < alphabet.size(); i++) {
        if (c == alphabet[i]) return i;
    }
}


strning Compress(const string& DataFile, const vector<int>& freq_table, const vector<char>& alphabet) {
    string encode = "";
    int delitel = b.back();
    int bits_to_follow = 0;
    char c;
    while (DataFile.get(c)) {
        int j = IndexForSymbol(c, alphabet);
        int l_n = l + freq_table[j-1] * (h - l + 1) / delitel;
        int h_n = l + freq_table[j] * (h - l + 1) / delitel - 1;
        
        while (true) {
            if (h_n < Half) {
                BitsPlusFollow(0);
                bits_to_follow = 0;
            } else if(l_n >= Half) {
                BitsPlusFollow(1);
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

void Decompress() {
    int value = 0;
    int bits_count = 0;
    int mask = 1;
    
    for (int i = 15; i >= 0; i--) {
        if //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   
        value |= (mask << bits_count);
    }
    
    char c;
    while (CompressedFile.get(c)) {
        int freq = ((value - l + 1) * b.back() - 1) / (h - l + 1);
        int j;
        for (j = i; b[j] <= freq; j++); //поиск символа
        
        int l_n = l + b[j-1] * (h - l + 1) / b.back();
        int h_n = l + b[j] * (h - l + 1) / b.back() - 1;
        
        while (true) {
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
        
        l_n *= 2;
        h_n = h_n * 2 + 1;
        
        ???
        }
        l = l_n;
        h = h_n;
        
        ???
}

int main()
{
    l = 0; h = 65535; i = 0; delitel = b[c_last]; //delitel = 10
    First_qrt = (h + 1) / 4; // = 16384
    Half = First_qrt * 2; // = 32768
    Third_qrt = First_qrt * 3; // = 49152
    bits_to_follow = 0;
    
    //алгоритм компресии
    while(not DataFile.EOF()) {
        c = DataFile.ReadSymbol();
        j = IndexForSymbol(c); i++;
        l_i = l_(i-1) + b[j-1] * (h_(i-1) - l_(i-1) + 1) / delitel;
        h_i = l_(i-1) + b[j] * (h_(i-1) - l_(i-1) + 1) / delitel - 1;
        for (;;){
            BitsPlusFollow(0);
            else if(l_i >= Half) {
                BitsPlusFollow(1);
                l_(i-) = Half; h_(i-) = Half;
            }
            else if((l_i >= First_qrt) && (h_i < Third_qrt)) {
                bits_to_follow++;
                l_(i-) = First_qrt; h_(i-) = First_qrt;
            } else break;
            l_(i+) = l_i; h_(i+) = h_i + 1;
        }
    }
    void BitsPlusFollow(int bit) { //процедура переноса найденных битов в файл
        CompressedFile.WriteBit(bit);
        for (; bits_to_follow > 0; bits_to_follow--)
            CompressedFile.WriteBit(!bit);
    }
    
    //алгоритм декомпресии
    value = CompressedFile.Read16Bit();
    for (i=1; i< CompressedFile.DataLength(); i++) {
        freq = ((value - l_(i-1) + 1) * delitel - 1) / (h_(i-1) - l_(i-1) + 1);
        for (j = i; b[j] <= freq; j++); //поиск символа
        
        l_i = l_(i-1) + b[j-1] * (h_(i-1) - l_(i-1) + 1) / delitel;
        h_i = l_(i-1) + b[j] * (h_(i-1) - l_(i-1) + 1) / delitel - 1;
        
            for(;;) { //Обрабатываем варианты переполнения
                if (h_i < Half)
                    ; //ничего
                else if (l_i >= Half) {
                    l_(i-) = Half; h_(i-) = Half; value -= Half;
                }
                else if ((l_i >= First_qrt) && (h_i < Third_qrt)) {
                    l_(i-) = First_qrt; h_(i-) = First_qrt;
                    value -= First_qrt;
                } else break;
                l_(i+) = l_i; h_(i+) = h_i + 1;
                value += value + CompressedFile.ReadBit();
            }
        DataFile.WriteSymbol(c);
    }
}