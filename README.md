
# C++ Standard Library Reimplementation Project
 
## About the Repo

I intend to make this project serve as a repository to deconstruct and re-build the C++ standard template library to deeply understand the design space and find opportunities to experiment with different design morphications.

This project aims to deconstruct and re-implement the C++ Standard Template Library (STL) from first principles. Our goal is not to replace the STL, but rather to:
    - Deeply understand the design decisions behind the C++ STL
    - Explore alternative implementation strategies
    - Experiment with different design trade-offs
    - Serve as an educational resource for C++ developers

## Disclaimer

This project is for educational and experimental purposes only. It is not intended for production use. The implementations here may prioritize clarity and experimentation over performance optimizations found in standard library implementations.

## Project Structure
    - `src/`: Implementation of core components
    - `DataStructures/`: Fundamental data structure implementations
    - `Iterators/`: Iterator abstractions and implementations
    - `ReferenceManager/`: Smart pointer and memory management utilities
    - `TypeSupport/`: Type system utilities and enhancements
    - `doc/`: Design documents and specifications
    - `tests/`: Comprehensive test suites


## Contribution Guidelines

1. **Code Style**:
    - Follow the C++ Core Guidelines.
    - Use consistent indentation and naming conventions: `snake_case` for functions and `PascalCase` for class names.
    - Use the C++17 tailed return types for readability. 

2. **Testing**:
    - Write unit tests for all new features.
    - Use a testing framework like Google Test or Catch2.

3. **Documentation**:
    - Document all public APIs using Doxygen-style comments.
    - Provide examples where applicable.

4. **Pull Requests**:
    - Ensure your branch is up-to-date with the main branch.
    - Provide a clear description of the changes made.

## Getting Started

1. Clone the repository:
    ```bash
    git clone <repository-url>
    cd StandardLibrary
    ```

2. Build the project:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run tests:
    ```bash
    ./run_tests
    ```

## Directory Structure

- `src/`: Contains the source code.
- `include/`: Contains header files.
- `tests/`: Contains unit tests.
- `docs/`: Contains project documentation.