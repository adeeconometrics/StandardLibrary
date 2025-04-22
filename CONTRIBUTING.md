# Contributing to the C++ Standard Library Reimplementation Project

Welcome to our project! This guide will help you understand how to contribute effectively while maintaining our high standards for code quality and consistency.

## Table of Contents
1. [Getting Started](#getting-started)
2. [Development Workflow](#development-workflow)
3. [Code Style and Standards](#code-style-and-standards)
4. [Testing Requirements](#testing-requirements)
5. [Documentation Requirements](#documentation-requirements)
6. [Pull Request Process](#pull-request-process)

## Getting Started

### Prerequisites
- C++17 compatible compiler
- CMake (3.15 or higher)
- Git
- GoogleTest framework (included as submodule; refer to [CI configuration](.github/workflows/c-cpp.yml))

### Setting Up Your Development Environment
1. Fork the repository
2. Clone your fork:
   ```bash
   git clone <your-fork-url>
   cd StandardLibrary
   ```
3. Set up build environment:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

## Development Workflow

1. **Create a New Branch**
   - Branch naming convention: `feature/description` or `fix/description`
   - Keep branches focused on single features/fixes

2. **Development Cycle**
   - Write tests first (TDD approach)
   - Implement the feature/fix
   - Document your changes
   - Run all tests locally
   - Update documentation if needed

3. **Commit Guidelines**
   - Use descriptive commit messages
   - Format: `[Component] Brief description\n\nDetailed description`
   - Reference issues/PRs where applicable

## Code Style and Standards

### Naming Conventions
- Use `snake_case` for variables and functions
- Use `PascalCase` for classes and structs
- Use `CAPITAL_CASE` for enums
- Use descriptive names that clearly convey purpose

### Template Style
```cpp
template<typename ValueType>
auto MyClass<ValueType>::function() -> ReturnType {
    // Implementation
}
```

### Header File Structure
```cpp
#ifndef __HEADER_NAME__
#define __HEADER_NAME__

// Contents

#endif // __HEADER_NAME__
```

### Smart Pointer Usage
- Use project-implemented smart pointers
- Use raw pointers only for:
  - Iterator implementations
  - Smart pointer implementations
  - Low-level container operations

### Best Practices
- Use `constexpr` whenever possible
- Ensure proper const-correctness
- Implement move semantics
- Follow RAII principles
- Use C++17 features appropriately

## Testing Requirements

### Test Categories Required
1. Construction and Assignment
   - Default construction
   - Copy/Move semantics
   - Assignment operators

2. Core Operations
   - Element access
   - Modification operations
   - Capacity handling

3. Iterator Requirements
   - Forward/Reverse iteration
   - Const correctness
   - Iterator invalidation

4. Edge Cases
   - Empty/Full states
   - Resource exhaustion
   - Invalid operations

### Test Structure
```cpp
class TestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

TEST_F(TestFixture, Operation_Scenario_ExpectedBehavior) {
    // Test implementation
}
```

## Documentation Requirements

### Code Documentation
- Every public API must have comprehensive Doxygen comments
- Document template parameters and constraints
- Include usage examples for complex features
- Document complexity guarantees

### Design Documentation
- Update relevant design docs in `/doc`
- Document major design decisions
- Include rationale for non-obvious implementations

### Example Documentation Format
```cpp
/**
 * @brief Brief description
 * 
 * @tparam T Type parameter description
 * @param param Parameter description
 * @return Return value description
 * 
 * @throws ExceptionType Description of when/why
 * 
 * @note Important notes
 * 
 * Example:
 * @code
 *     // Usage example
 * @endcode
 */
```

## Pull Request Process

1. **Before Submitting**
   - Ensure all tests pass
   - Update documentation
   - Follow code style guidelines
   - Add/update test cases

2. **PR Description Template**
   ```markdown
   ## Description
   Brief description of changes

   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Breaking change
   - [ ] Documentation update

   ## Test Coverage
   - [ ] New tests added
   - [ ] Existing tests updated
   - [ ] All tests passing

   ## Documentation
   - [ ] Code comments updated
   - [ ] Documentation files updated
   ```

3. **Review Process**
   - Address reviewer comments promptly
   - Keep PR scope focused
   - Update PR based on feedback

4. **Merging**
   - Squash commits if requested
   - Ensure CI passes
   - Maintain clean commit history

## Need Help?

- Check existing documentation in `/doc`
- Review closed PRs for examples
- Ask questions in issues/discussions
- Reference the testing guidelines in `/doc/TestingGuidelines.md`

---

Remember: This project aims to be educational and experimental. Clarity and correctness take precedence over optimization.
