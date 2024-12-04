import heapq
from collections import defaultdict

class Node:
    def __init__ (self, ch = '', freq = 0, left = None, right = None):
        self.ch= ch
        self.freq= freq
        self.left= left
        self.right= right
        
    def __lt__ (self, other):
        return self.freq < other.freq
    
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

def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()
        
def write_encode_file(file_path, encoded_text):
    with open(file_path, 'w') as file:
        file.write(encoded_text)
        
        
        
node1 = Node('a', 3)
node2 = Node('b', 2)
print(node1 < node2)  # Ожидается True, т.к. частота 'a' больше, чем частота 'b'

text = "abacabad"
tree = BuildTree(text)

# Для проверки структуры дерева
print(tree.ch)  # Ожидаем None, потому что корень не содержит символа
print(tree.freq)  # Ожидаем общую частоту всех символов в строке























