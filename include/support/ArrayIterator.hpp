#ifndef MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP
#define MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP

#include <Array.hpp>
#include <Variant.hpp>

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace godot
{

  template<typename ValueType>
  struct ArrayIterator
  {
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = value_type const &;
    using pointer = value_type *;
    using const_pointer = value_type const *;
    using iterator_category = std::random_access_iterator_tag;
    using container_type = std::conditional_t<std::is_const_v<ValueType>, Array const, Array>;

    constexpr ArrayIterator(container_type & array, difference_type position)
        : m_array{&array}
        , m_position{position}
    {
    }

    explicit constexpr ArrayIterator(container_type & array)
        : ArrayIterator{array, 0}
    {
    }

    ArrayIterator(ArrayIterator const & other) = default;

    auto operator+=(difference_type n) -> ArrayIterator &
    {
      m_position += n;
      return *this;
    }

    auto operator+(difference_type n) const -> ArrayIterator
    {
      auto result = *this;
      return result += n;
    }

    auto operator-=(difference_type n) -> ArrayIterator &
    {
      return *this += -n;
    }

    auto operator-(difference_type n) const -> ArrayIterator
    {
      return *this + -n;
    }

    auto operator-(ArrayIterator const & other) const -> difference_type
    {
      return m_position - other.m_position;
    }

    auto operator<(ArrayIterator const & other) const -> bool
    {
      return m_position < other.m_position;
    }

    auto operator>(ArrayIterator const & other) const -> bool
    {
      return other < *this;
    }

    auto operator<=(ArrayIterator const & other) const -> bool
    {
      return !(*this > other);
    }

    auto operator>=(ArrayIterator const & other) const -> bool
    {
      return !(*this < other);
    }

    auto operator==(ArrayIterator const & other) const -> bool
    {
      return (*this >= other) && (other >= *this);
    }

    auto operator!=(ArrayIterator const & other) const -> bool
    {
      return !(*this == other);
    }

    auto operator--() -> ArrayIterator &
    {
      --m_position;
      return *this;
    }

    auto operator--(int) -> ArrayIterator
    {
      auto result = *this;
      --*this;
      return result;
    }

    auto operator++() -> ArrayIterator &
    {
      ++m_position;
      return *this;
    }

    auto operator++(int) -> ArrayIterator
    {
      auto result = *this;
      ++*this;
      return result;
    }

    auto operator*() const -> std::conditional_t<std::is_const_v<ValueType>, value_type, reference>
    {
      return (*m_array)[static_cast<int>(m_position)];
    }

    auto operator->() const -> pointer
    {
      return &(*m_array)[static_cast<int>(m_position)];
    }

  private:
    container_type * m_array;
    difference_type m_position;
  };

  struct ArrayBackInsertIterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Array;

    explicit constexpr ArrayBackInsertIterator(Array & array)
        : m_array{&array}
    {
    }

    auto operator=(Variant const & value) -> ArrayBackInsertIterator &
    {
      m_array->push_back(value);
      return *this;
    }

    auto constexpr operator*() -> ArrayBackInsertIterator &
    {
      return *this;
    }

    auto constexpr operator++() -> ArrayBackInsertIterator &
    {
      return *this;
    }

    auto constexpr operator++(int) -> ArrayBackInsertIterator &
    {
      return *this;
    }

  private:
    Array * m_array;
  };

  auto inline constexpr begin(Array & array) -> ArrayIterator<Variant>
  {
    return ArrayIterator<Variant>{array};
  }

  auto inline end(Array & array) -> ArrayIterator<Variant>
  {
    return {array, array.size()};
  }

  auto inline constexpr cbegin(Array const & array) -> ArrayIterator<Variant const>
  {
    return ArrayIterator<Variant const>{array};
  }

  auto inline cend(Array const & array) -> ArrayIterator<Variant const>
  {
    return {array, array.size()};
  }

  auto inline constexpr back_inserter(Array & array) -> ArrayBackInsertIterator
  {
    return ArrayBackInsertIterator{array};
  }

}  // namespace godot

#endif