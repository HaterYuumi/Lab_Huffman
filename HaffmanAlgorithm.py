import heapq
from collections import defaultdict
import os

class Node:
    def __init__ (self, char = '', freq = 0, left = None, right = None):
        self.char = char
        self.freq = freq
        self.left = left
        self.right = right
        
    def __lt__ (self, other):
        return self.freq < other.freq
    
def BuildTree(frequency):
    
    heap = []
    
    for char, freq in frequency.items():
        heapq.heappush(heap, Node(char, freq))
        
    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        
        in_node = Node(None, left.freq + right.freq)
        in_node.left = left
        in_node.right = right
        
        heapq.heappush(heap, in_node)
        
    return heap[0]

def HuffmanBuild(node, prefix = '', codes = {}):
    
    if node is not None:
        
        if node.char is not None:
            codes[node.char] = prefix
        
        HuffmanBuild(node.left, prefix + '0', codes)
        HuffmanBuild(node.right, prefix + '1', codes)
            
    return codes

def encode_text(text, codes):
    return ''.join(codes[char] for char in text)

def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()
        
def write_encode_file(file_path, encoded_text):
    with open(file_path, 'w') as file:
        file.write(encoded_text)

def write_binary(encoded_text, frequency, file_path):
    with open(file_path, 'w', encoding='utf-8') as f:
        
        freq_string = ' '.join([f"{ord(char)}:{freq}" for char, freq in frequency.items()])
        f.write(freq_string + '\n')  
        
        byte_arr = bytearray()
        
        for i in range(0, len(encoded_text), 8):
            byte = encoded_text[i:i + 8]
            if len(byte) < 8:
                byte += '0' * (8 - len(byte))
            byte_arr.append(int(byte, 2))

        f.write(byte_arr.decode('latin-1'))
    
def read_binary(file_path):
    with open(file_path, 'r', encoding = 'utf-8') as f:
        
        frequency = {}
        
        line = f.readline().strip()
        
        freq_data = line.split(' ')

        for item in freq_data:
            if ':' in item:  
                char, freq = item.split(':', 1)
                if char == '':
                    char = ' '  
                frequency[chr(int(char))] = int(freq) 

        encoded_text = ''.join(format(ord(byte), '08b') for byte in f.read())

    return frequency, encoded_text

def Decode_text(encoded_text, tree):
    decoded_output = ""
    node = tree
    
    for bit in encoded_text:
        if bit == '0':
            node = node.left
        else:
            node = node.right
            
        if node.char is not None:
            decoded_output += node.char
            node = tree
        
    return decoded_output

def console():
    choice = input("Что вы хотите сделать? Введите 0 для кодирования или 1 для декодирования: ")
    if choice not in ['0', '1']:
        print("Неверный ввод! Пожалуйста, введите 0 или 1.")
        return console()
    
    file_path = 'text.txt'
    
    if choice == '0':
        with open(file_path, 'r', encoding = 'utf-8') as file:
            text = file.read()
        
        frequency = defaultdict(int)
        for char in text:
            frequency[char] += 1
        
        tree = BuildTree(frequency)
        
        codes = HuffmanBuild(tree)
        
        encoded_text = encode_text(text, codes)
        
        write_binary(encoded_text, frequency, file_path)
        
        print("\nТаблица кодов Хаффмана:")
        for char, code in codes.items():
            print(f"'{char}': {code}")
        
        original_size = len(text.encode('utf-8'))
        encoded_size = len(encoded_text) // 8 + (1 if len(encoded_text) % 8 else 0)
        
        print("\nРазмер исходного текста:", original_size, "байт")
        print("Размер закодированного текста:", encoded_size, "байт")

        print("\nЗакодированный текст в двоичном виде:")
        print(encoded_text)
    
    elif choice == '1':
        frequency, encoded_text = read_binary(file_path)
        
        tree = BuildTree(frequency)
        
        decoded_text = Decode_text(encoded_text, tree)
        
        print("\nДекодированный текст:")
        print(decoded_text)
        
        
        with open(file_path, 'w', encoding = 'utf-8') as f:
            f.write(decoded_text)



if __name__ == "__main__":
    console()
