#include <Array.hpp>
#include <iostream>

int main() {
    // Demonstrate Array usage
    Array<int, 5> arr;
    
    try {
        // Add some elements
        arr.add(1);
        arr.add(2);
        arr.add(3);
        arr.add(4);
        arr.add(5);
        
        // Display elements using iterator
        std::cout << "Array elements: ";
        for (const auto& element : arr) {
            std::cout << element << " ";
        }
        std::cout << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}