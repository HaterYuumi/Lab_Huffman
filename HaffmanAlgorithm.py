import heapq
from collections import defaultdict

class Node:
    def __init__(self, char='', freq=0, left=None, right=None):
        self.char = char
        self.freq = freq
        self.left = left
        self.right = right

    def __lt__(self, other):
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

def HuffmanBuild(node, prefix='', codes={}):
    if node is not None:
        if node.char is not None:
            codes[node.char] = prefix
        HuffmanBuild(node.left, prefix + '0', codes)
        HuffmanBuild(node.right, prefix + '1', codes)
    return codes

def encode_text(text, codes):

    binary_str = ''.join(codes[char] for char in text)

    byte_array = bytearray()
    for i in range(0, len(binary_str), 8):
        byte = binary_str[i:i+8]
        byte_array.append(int(byte, 2))
    return byte_array

def write_to_text(frequency, encoded_text, output_file_path):
    with open(output_file_path, 'wb') as file: 
        for char, freq in frequency.items():
            escaped_char = repr(char)  # repr для корректной записи символов
            file.write(f"{freq}:{escaped_char}\n".encode('utf-8'))  

        file.write(b"\n") 
        
        file.write(encoded_text)  

def read_from_text(file_path):
    frequency = {}
    encoded_text = bytearray()
    with open(file_path, 'rb') as file:  
        for line in file:
            if line.strip() == b"":
                break
            try:
                line = line.decode('utf-8')
                freq, char = line.strip().split(":", 1)
                char = eval(char)
                frequency[char] = int(freq)
            except ValueError:
                print(f"Ошибка при обработке строки: '{line.strip()}'. Пропускаем её.")
                continue

        encoded_text = file.read()

    return frequency, encoded_text

def Decode_text(encoded_text, tree):
    decoded_output = ""
    node = tree
    bits = ''.join(f'{byte:08b}' for byte in encoded_text)
    
    for bit in bits:
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

    input_file_path = 'text.txt'
    if choice == '0':
        output_file_path = 'encoded_text.bin'  

        with open(input_file_path, 'r', encoding='utf-8') as file:
            text = file.read()

        frequency = defaultdict(int)
        for char in text:
            frequency[char] += 1

        tree = BuildTree(frequency)
        codes = HuffmanBuild(tree)

        encoded_text = encode_text(text, codes)

        write_to_text(frequency, encoded_text, output_file_path)

        print("\nТаблица кодов Хаффмана:")
        for char, code in codes.items():
            print(f"'{char}': {code}")

        original_size = len(text.encode('utf-8'))
        encoded_size = len(encoded_text)

        print("\nРазмер исходного текста:", original_size, "байт")
        print("Размер закодированного текста:", encoded_size, "байт")

    elif choice == '1':
        output_file_path = 'decoded_text.txt'

        frequency, encoded_text = read_from_text('encoded_text.bin')

        tree = BuildTree(frequency)

        decoded_text = Decode_text(encoded_text, tree)

        print("\nДекодированный текст:")
        print(decoded_text)

        with open(output_file_path, 'w', encoding='utf-8') as f:
            f.write(decoded_text)

if __name__ == "__main__":
    console()
