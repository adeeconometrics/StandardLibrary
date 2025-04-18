/**
 * @file Nullable.h
 * @author ddamiana
 * @brief 
 * @version 0.1
 * @date 2021-08-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <cstddef>
#include <stdexcept>
#include <type_traits>

template <typename T> class Nullable final {
public:
  Nullable() {}

  Nullable(const T &value) : Value(value) {}

  Nullable(std::nullptr_t nullpointer) { (void)nullpointer; }

  const Nullable<T> &operator=(const Nullable<T> &value) {
    Value.m_has_value = value.Value.m_has_value;
    Value.m_value = value.Value.m_value;
    return *this;
  }

  const Nullable<T> &operator=(const T &value) {
    Value.m_has_value = true;
    Value.m_value = value;
    return *this;
  }

  const Nullable<T> &operator=(std::nullptr_t nullpointer) {
    (void)nullpointer;
    Value.m_has_value = false;
    Value.m_value = T();
    return *this;
  }

  bool HasValue() const { return Value.m_has_value; }

  const T &get_content() const { return Value.m_value; }

  const T &get_content(const T &def) const {
    if (Value.m_has_value)
      return Value.m_value;
    else
      return def;
  }

  bool TryGetValue(T &value) const {
    value = Value.m_value;
    return Value.m_has_value;
  }

  T *operator->() { return &Value.m_value; }

  const T *operator->() const { return &Value.m_value; }

  T &operator*() { return Value.m_value; }

  const T &operator*() const { return Value.m_value; }

public:
  class NullableValue final {
  public:
    friend class Nullable;

  private:
    NullableValue() : m_has_value(false), m_value(T()) {}

    NullableValue(const T &value) : m_has_value(true), m_value(value) {}

  public:
    NullableValue &operator=(const NullableValue &) = delete;

    operator const T &() const {
      has_value();
      return m_value;
    }

    const T &operator*() const {
      has_value();
      return m_value;
    }

    const T *operator&() const {
      has_value();
      return &m_value;
    }

    const T *operator->() const {
      has_value();
      return &m_value;
    }

  public:
    template <typename T2>
    friend bool operator==(const Nullable<T2> &op1, const Nullable<T2> &op2) {
      if (op1.Value.m_has_value != op2.Value.m_has_value)
        return false;

      if (op1.Value.m_has_value)
        return op1.Value.m_value == op2.Value.m_value;
      else
        return true;
    }

    template <typename T2>
    friend bool operator==(const Nullable<T2> &op, const T2 &value) {
      if (!op.Value.m_has_value)
        return false;

      return op.Value.m_value == value;
    }

    template <typename T2>
    friend bool operator==(const T2 &value, const Nullable<T2> &op) {
      if (!op.Value.m_has_value)
        return false;

      return op.Value.m_value == value;
    }

    template <typename T2>
    friend bool operator==(const Nullable<T2> &op, std::nullptr_t nullpointer) {
      (void)nullpointer;
      return !op.Value.m_has_value;
    }

    template <typename T2>
    friend bool operator==(std::nullptr_t nullpointer, const Nullable<T2> &op) {
      (void)nullpointer;
      return !op.Value.m_has_value;
    }

    template <typename T2>
    friend bool operator!=(const Nullable<T2> &op1, const Nullable<T2> &op2) {
      if (op1.Value.m_has_value != op2.Value.m_has_value)
        return true;

      if (op1.Value.m_has_value)
        return op1.Value.m_value != op2.Value.m_value;
      else
        return false;
    }

    template <typename T2>
    friend bool operator!=(const Nullable<T2> &op, const T2 &value) {
      if (!op.Value.m_has_value)
        return true;

      return op.Value.m_value != value;
    }

    template <typename T2>
    friend bool operator!=(const T2 &value, const Nullable<T2> &op) {
      if (!op.Value.m_has_value)
        return false;

      return op.Value.m_value != value;
    }

    template <typename T2>
    friend bool operator!=(const Nullable<T2> &op, std::nullptr_t nullpointer) {
      (void)nullpointer;
      return op.Value.m_has_value;
    }

    template <typename T2>
    friend bool operator!=(std::nullptr_t nullpointer, const Nullable<T2> &op) {
      (void)nullpointer;
      return op.Value.m_has_value;
    }

  private:
    void has_value() const {
      if (!m_has_value)
        throw std::runtime_error("Nullable object must have a value");
    }

  private:
    bool m_has_value;
    T m_value;
  };

public:
  NullableValue Value;
};