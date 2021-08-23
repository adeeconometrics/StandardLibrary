# Design Requirements
The Structures Framework contains four ADT Categories with their respective design goals and use-cases:

1. *Array-based* modules are for Structures concerned with static implementation.

2. *List-based* modules are for Structures concerned with dynamic implementation.

3. *Tree-based* modules are for Structures concerned with ordered set of data. Thus, it requires data types to be comparable.

4. *Hash-based* modules are for Structures concerned with unordered set of data.

---
## Outline

### Array
- Vector
- Array
- ArrayStack
- ArrayQueue
- ArrayDeque
- ArrayCircularQueue
- Heap
- ArrayMatrix
### List
- VectorList
- SinglyList
- DoublyList
- CircularList
- ListStack
- ListQueue
- ListDeque
- ListCircularQueue
- ListMatrix
### Tree
- TreeMap
- TreeSet
- TreeHeap
### Hash
- HashMap
- HashSet
- HashList
- HashMattrix -- ragged hash

---
Generic structures contain the generic version of our data structures implemented in C++. 

Todo:
- implement iterators for containers 
- test and assure that it maintains RAII
- implement move semantics
- implement emplace methods 
- check if overloading the delete operator is needed
- ensure resource safety
- test for correctness