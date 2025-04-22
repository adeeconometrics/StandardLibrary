#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <type_traits>

namespace sl { // standard library namespace

// Iterator tags to distinguish iterator categories
struct ForwardIteratorTag {};
struct BidirectionalIteratorTag : ForwardIteratorTag {};
struct RandomAccessIteratorTag : BidirectionalIteratorTag {};

// Base iterator interface using CRTP
template <typename Derived, typename ValueType,
          typename Category = ForwardIteratorTag>
class IteratorBase {
public:
  using value_type = ValueType;
  using pointer = ValueType *;
  using reference = ValueType &;
  using iterator_category = Category;

  // Cast to derived type
  auto derived() -> Derived & { return static_cast<Derived &>(*this); }

  auto derived() const -> const Derived & {
    return static_cast<const Derived &>(*this);
  }

  // Common iterator operations
  auto operator*() const -> reference { return *derived().operator->(); }

  auto operator->() const -> pointer { return derived().operator->(); }

  auto operator++() -> Derived & {
    derived().increment();
    return derived();
  }

  auto operator++(int) -> Derived {
    Derived temp(derived());
    derived().increment();
    return temp;
  }

  // Bidirectional operations - enabled only for bidirectional iterators
  template <typename D = Derived>
  auto operator--()
      -> std::enable_if_t<std::is_base_of_v<BidirectionalIteratorTag, Category>,
                          Derived &> {
    derived().decrement();
    return derived();
  }

  template <typename D = Derived>
  auto operator--(int)
      -> std::enable_if_t<std::is_base_of_v<BidirectionalIteratorTag, Category>,
                          Derived> {
    Derived temp(derived());
    derived().decrement();
    return temp;
  }

  // Comparison operators
  auto operator==(const IteratorBase &other) const -> bool {
    return derived().equals(other.derived());
  }

  auto operator!=(const IteratorBase &other) const -> bool {
    return !derived().equals(other.derived());
  }

protected:
  // Protected constructor to prevent direct instantiation
  IteratorBase() = default;
};

// Example of how a concrete iterator would be implemented
template <typename T>
class ConcreteIterator
    : public IteratorBase<ConcreteIterator<T>, T, BidirectionalIteratorTag> {
private:
  T *ptr_;

public:
  explicit ConcreteIterator(T *ptr) : ptr_(ptr) {}

  // Required implementation for the CRTP interface
  auto operator->() const -> T * { return ptr_; }

  void increment() { ++ptr_; }
  void decrement() { --ptr_; }

  auto equals(const ConcreteIterator &other) const -> bool {
    return ptr_ == other.ptr_;
  }
};

} // namespace sl

#endif // __ITERATOR_HPP__