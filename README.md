# C++ Standard Library Reimplementation Project
 
## About the Repo

This project serves as a comprehensive reimplementation of the C++ Standard Template Library (STL) components using modern C++17 features. The goal is to deeply understand the design space and experiment with different implementation strategies.

## Project Goals

- Implement STL components with modern C++17 features
- Explore template metaprogramming techniques
- Provide clear, well-documented implementations
- Experiment with different design trade-offs
- Serve as an educational resource for C++ developers

## Disclaimer

This project is for educational and experimental purposes only. It is not intended for production use. The implementations here may prioritize clarity and experimentation over performance optimizations found in standard library implementations.

## Project Structure
- `src/`: Core implementation
  - `DataStructures/`: Container implementations
  - `Iterators/`: Iterator implementations
- `docs_sphinx/`: API documentation
- `tests/`: Unit test suites
- `doc/`: Design documents

## Technical Requirements

1. **Language Standards**:
   - C++17 compliant
   - No STL containers (except std::string, std::except)
   - Custom implementation of smart pointers and iterators

2. **Implementation Guidelines**:
   - Use raw pointers only for iterator/smart pointer implementation
   - Follow RAII principles
   - Implement move semantics
   - Use constexpr where possible
   - Proper const correctness

3. **Templates**:
   - Type-safe generic containers
   - SFINAE for template constraints
   - Modern template metaprogramming techniques

## Documentation

API documentation is built using:
- Doxygen for C++ code documentation
- Sphinx for generating HTML documentation
- Breathe for Doxygen/Sphinx integration

To build documentation:
```bash
cd docs_sphinx
make html
```

## Building the Project

1. Clone and setup:
```bash
git clone <repository-url>
cd StandardLibrary
```

2. Build:
```bash
mkdir build && cd build
cmake ..
make
```

3. Run tests:
```bash
./run_tests
```

## Contributing

See [CONTRIBUTING.md] for detailed guidelines on:
- Coding standards
- Documentation requirements
- Testing requirements
- Pull request process