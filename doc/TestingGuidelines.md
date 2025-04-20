# Testing Guidelines for Data Structures

## Test Categories

### 1. Construction and Assignment Tests
- Default Constructor
  - Test empty state
  - Verify initial capacity
  - Verify size is 0
  - Verify proper initialization of members
- Initializer List Constructor (if applicable)
  - Test with valid input
  - Test with maximum capacity
  - Test exception handling for oversized lists
- Copy Constructor
  - Verify deep copy of elements
  - Verify independence of copies (modify copy)
  - Test with empty container
  - Test with full container
- Move Constructor
  - Verify ownership transfer
  - Verify source state after move
  - Test with various container states
- Assignment Operators
  - Copy assignment: Test deep copy
  - Move assignment: Test ownership transfer
  - Test self-assignment handling
  - Test chained assignments

### 2. Element Access Tests
- Test access methods (front/back/top/at etc.)
  - Access first element
  - Access last element
  - Access middle elements (if applicable)
  - Test const correctness
- Exception Safety
  - Test out-of-range access
  - Test empty container access
  - Test invalid index access

### 3. Modifier Tests
- Element Addition (push/insert/emplace)
  - Test single element addition
  - Test multiple element addition
  - Test at different positions (if applicable)
  - Test exception handling when full
- Element Removal (pop/erase)
  - Test single element removal
  - Test multiple element removal
  - Test from different positions (if applicable)
  - Test exception handling when empty
- Clear Operations
  - Test clearing empty container
  - Test clearing full container
  - Verify state after clear

### 4. Capacity Tests
- Empty State
  - Test initial state
  - Test after operations
  - Test edge cases
- Full State
  - Test capacity limits
  - Test size tracking
  - Test resize operations (if applicable)

### 5. Iterator Tests
- Forward Iteration
  - Test begin() to end()
  - Test element access
  - Test modification through iterator
- Const Iterator
  - Test const_iterator operations
  - Test cbegin() to cend()
  - Verify const correctness
- Iterator Invalidation (if applicable)
  - Test after modifications
  - Test after resizing
  - Test after clear

### 6. Template Constraint Tests
- Type Requirements
  - Test with non-copyable types
  - Test with non-moveable types
  - Test with incomplete types
- Size/Capacity Requirements
  - Test invalid sizes
  - Test zero size
  - Test maximum size limits

### 7. Memory Management Tests
- Resource Management
  - Test for memory leaks
  - Test exception safety guarantees
  - Test RAII compliance
- Move Semantics
  - Test efficient transfer of resources
  - Test no unnecessary copies
  - Test exception safety during moves

### 8. Special Operations Tests
- Custom Operations
  - Test data structure specific operations
  - Test edge cases for special operations
  - Test operation compositions

## Test Implementation Guidelines

1. Test Fixture Setup
```cpp
class DataStructureTest : public ::testing::Test {
protected:
    // Define common test sizes
    static constexpr size_t test_size = 5;
    
    // Setup commonly used test instances
    DataStructure<int, test_size> int_instance;
    DataStructure<std::string, test_size> string_instance;
    
    void SetUp() override {
        // Common setup code
    }
    
    void TearDown() override {
        // Common cleanup code
    }
};
```

2. Test Naming Convention
```cpp
TEST_F(DataStructureTest, CategoryName_OperationName_ExpectedBehavior) {
    // Test implementation
}
```

3. Exception Testing Pattern
```cpp
TEST_F(DataStructureTest, Operation_ThrowsExpectedException_WhenCondition) {
    EXPECT_THROW(operation(), expected_exception);
}
```

4. Complex Type Testing Pattern
```cpp
struct ComplexType {
    // Members and constructors
    // Must match container requirements
};
TEST_F(DataStructureTest, Operation_HandlesComplexTypes_AsExpected) {
    DataStructure<ComplexType, size> container;
    // Test implementation
}
```

## Best Practices

1. Test Independence
   - Each test should be independent and self-contained
   - Avoid test interdependencies
   - Reset state in SetUp() when needed

2. Edge Case Coverage
   - Empty container
   - Full container
   - Single element
   - Maximum capacity
   - Invalid operations

3. Type Testing
   - Test with built-in types (int, double)
   - Test with std::string
   - Test with custom types
   - Test with move-only types
   - Test with const-qualified types

4. Exception Safety
   - Basic guarantee tests
   - Strong guarantee tests where applicable
   - No-throw guarantee tests where applicable

5. Performance Considerations
   - Test complexity guarantees
   - Test iterator performance
   - Test large datasets when relevant

6. Documentation
   - Document test purpose
   - Document edge cases covered
   - Document performance expectations

## Example Test Structure
```cpp
// Construction tests
TEST_F(DataStructureTest, DefaultConstructor_CreatesEmptyContainer)
TEST_F(DataStructureTest, InitializerList_ConstructsWithElements)
TEST_F(DataStructureTest, CopyConstructor_CreatesDeepCopy)
TEST_F(DataStructureTest, MoveConstructor_TransfersOwnership)

// Element access tests
TEST_F(DataStructureTest, Access_ReturnsCorrectElement)
TEST_F(DataStructureTest, Access_ThrowsWhenEmpty)

// Modifier tests
TEST_F(DataStructureTest, Add_IncreasesSize)
TEST_F(DataStructureTest, Remove_DecreasesSize)

// Capacity tests
TEST_F(DataStructureTest, Capacity_TracksCorrectly)

// Iterator tests
TEST_F(DataStructureTest, Iterator_TraversesCorrectly)
TEST_F(DataStructureTest, ConstIterator_PreservesConstness)

// Template constraints
TEST_F(DataStructureTest, StaticAsserts_EnforceRequirements)

// Special operations
TEST_F(DataStructureTest, SpecialOp_WorksAsExpected)
```

## Validation Checklist

- [ ] All constructors tested
- [ ] All assignment operators tested
- [ ] Element access tested with bounds checking
- [ ] Modifier operations tested
- [ ] Capacity and size management tested
- [ ] Iterator operations tested
- [ ] Exception safety tested
- [ ] Template constraints tested
- [ ] Memory management tested
- [ ] Special operations tested
- [ ] Edge cases covered
- [ ] Type requirements verified
