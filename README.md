# Huffman Compression
## Copyright 2022 Toaca Alexandra Simona

This project aims at using Huffman Coding to compress and decompress files.  


## Usage

For compression:
> ./comp -c _file_

For decompression:
> ./comp -d _file.cmp_

Keep in mind that the resulting file will be saved in the same directory as  
the original file.


## What is Huffman Compression?

- is a type of **loseless** compression  
- it works by attributing a **code** (made only of 0's and 1's) to each **symbol**  
(a byte, in this example)  
- a symbol with a high frequency gets a shorter code than a symbol which  
is rarely encountered (using [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding))  

## My implementation

For **compression**:  
- _parse file_ (get the occurence **probability** of every symbol)
- construct a priority queue containing the leaf nodes of the "Huffman" Tree  
(each node contains the byte value and its **probability**, having the children set to NULL)
- construct the tree from the priority queue ([Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding))
- **serialize the tree**, meaning the tree is traversed in **postorder** and every node is  
saved in a vector of HuffmanTreeSerializedNode (the purpose is to save the serialized nodes  
in the compressed file and be able to reconstruct the tree for decompression)  
- getting the code for each symbol is done by traversing the tree in postorder and adding 0 or 1  
to the code based on the direction (going right is 1 and left is 0). When a leaf is reached, the  
symbol in the leaf gets the code build up to then (being then stored in a map)
- the file is read again and the code for each byte encountered is concatenated, resulting in  
a vector of 0's and 1's
- every 8 bits in the vector are transformed into a byte as to save space when writing the  
encoding (if the number of bits is not a multiple of 8, a padding of 0's is added)
- the compressed file contains:
	- the number of nodes in the tree
	- the serialized nodes
	- the original file size
	- the encoded bytes


For **decompression**:

- the compressed file is read and the serialized nodes and the encoding are saved
- the byte encoding is turned into the original vector of 0's and 1's
- the bit vector is traversed with an 'iterator' and, starting at the root  = serializedNodes\[0\],   
the iterator becomes either the left child (0) of the parent node or the right child (1).  
When a terminal node is encountered, the decoded symbol is written to the decompressed  
file and the process resumes  


**Comments**
- because we have to save the nodes, this method of compression is not suitable for small files
- the functions starting with _ are helper functions, mainly used here for recursion   
in the traversals of the tree






