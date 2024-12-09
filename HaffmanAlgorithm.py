import heapq
from collections import defaultdict

class Node:
    def __init__ (self, ch = '', freq = 0, left = None, right = None):
        self.ch= ch
        self.freq= freq
        self.left= left
        self.right= right
        
    def __lt__ (self, other):
        return self.freq > other.freq
    
def BuildTree(text):
    
    freq = defaultdict(int)
    for char in text:
        freq[char] += 1
    
    heap = [Node(char, freq[char]) for char in freq]
    heapq.heapify(heap)
    
    #Метод heapify восстанавливает основное свойство кучи для дерева
    #с корнем в i-ой вершине при условии, что оба поддерева ему
    #удовлетворяют. Для этого необходимо «опускать» i-ую вершину
    #(менять местами с наибольшим из потомко ), пока основное
    #свойство не будет восстановлено (процесс завершится, когда не
    #найдется потомка, большего своего родителя).
        
    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        
        in_node = Node(None, left.freq + right.freq)
        in_node.left = left
        in_node.right = right
        
        heapq.heappush(heap, in_node)
        
    return heap[0]

def HuffmanBuild(node, prefix = '', codes = None):
    if codes is None:
        codes = {}
        
    if node.ch is not None:
        codes[node.ch] = prefix
    else:
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

def Decode_text(encode_text, tree):
    decoded_output = ""
    node = tree
    
    for bit in encoded_text:
        if bit == '0':
            node = node.left
        else:
            node = node.right
            
        if node.ch is not None:
            decoded_output += node.ch
            node = tree
        
    return decoded_output

file = 'text.txt'
text = read_file(file)

tree = BuildTree(text)
codes = HuffmanBuild(tree)

encoded_text = encode_text(text, codes)

write_encode_file(file, encoded_text)

print("Коды символов:", codes)
print("Зашифрованный текст:", encoded_text)

decoded_text = Decode_text(encoded_text, tree)

print("Декодированный текст:", decoded_text)
