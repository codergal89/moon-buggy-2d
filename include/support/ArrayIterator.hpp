#ifndef MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP
#define MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP

#include <Array.hpp>
#include <Variant.hpp>

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace godot
{

  template<bool IsConstIterator>
  struct ArrayIterator
  {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
    using value_type = godot::Variant;
    using pointer = std::conditional_t<IsConstIterator, value_type const *, value_type *>;
    using reference = std::conditional_t<IsConstIterator, value_type const &, value_type &>;

    using array_type = std::conditional_t<IsConstIterator, Array const, Array>;

    constexpr ArrayIterator() = default;
    constexpr ArrayIterator(ArrayIterator const &) = default;
    constexpr ArrayIterator(ArrayIterator &&) = default;
    auto constexpr operator=(ArrayIterator const &) -> ArrayIterator & = default;
    auto constexpr operator=(ArrayIterator &&) -> ArrayIterator & = default;

    explicit constexpr ArrayIterator(array_type & array, difference_type position = 0)
        : array{const_cast<Array *>(&array)}
        , size{array.size()}
        , position{position}
    {
    }

    auto operator*() const noexcept -> reference
    {
      return (*array)[position];
    }

    auto operator->() const noexcept -> pointer
    {
      return &array[position];
    }

    auto operator[](difference_type index) const noexcept -> reference
    {
      return *(*this + index);
    }

    auto constexpr operator++() noexcept -> ArrayIterator &
    {
      ++position;
      return *this;
    }

    auto constexpr operator--() noexcept -> ArrayIterator &
    {
      --position;
      return *this;
    }

    auto constexpr operator+=(difference_type difference) noexcept -> ArrayIterator &
    {
      position += difference;
      return *this;
    }

    auto constexpr operator-=(difference_type difference) noexcept -> ArrayIterator &
    {
      position -= difference;
      return *this;
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator-(ArrayIterator<OtherIsConstIterator> const & other) const noexcept -> difference_type
    {
      return position - other.position;
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator==(ArrayIterator<OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return (!other.array && position == size) || (!array && other.position == other.size) || position == other.position;
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator!=(ArrayIterator<OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return !(*this == other);
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator<(ArrayIterator<OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return !(*this == other);
    }

  private:
    Array * array;
    difference_type size;
    difference_type position;
  };

  template<typename ArrayType>
  ArrayIterator(ArrayType &) -> ArrayIterator<std::is_const_v<std::remove_reference_t<ArrayType>>>;

  template<typename ArrayType>
  ArrayIterator(ArrayType &, int) -> ArrayIterator<std::is_const_v<std::remove_reference_t<ArrayType>>>;

  template<bool IsConstIterator>
  auto constexpr operator++(ArrayIterator<IsConstIterator> & it, int) noexcept -> ArrayIterator<IsConstIterator>
  {
    auto copy = it;
    ++it;
    return copy;
  }

  template<bool IsConstIterator>
  auto constexpr operator--(ArrayIterator<IsConstIterator> & it, int) noexcept -> ArrayIterator<IsConstIterator>
  {
    auto copy = it;
    --it;
    return copy;
  }

  template<bool IsConstIterator>
  auto constexpr operator+(ArrayIterator<IsConstIterator> const & it,
                           typename ArrayIterator<IsConstIterator>::difference_type difference) noexcept -> ArrayIterator<IsConstIterator>
  {
    auto copy = it;
    return copy += difference;
  }

  template<bool IsConstIterator>
  auto constexpr operator+(typename ArrayIterator<IsConstIterator>::difference_type difference,
                           ArrayIterator<IsConstIterator> const & it) noexcept -> ArrayIterator<IsConstIterator>
  {
    auto copy = it;
    return copy += difference;
  }

  template<bool IsConstIterator>
  auto constexpr operator-(ArrayIterator<IsConstIterator> const & it,
                           typename ArrayIterator<IsConstIterator>::difference_type difference) noexcept -> ArrayIterator<IsConstIterator>
  {
    auto copy = it;
    return copy -= difference;
  }

  template<bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator<(ArrayIterator<LhsIsConstIterator> const & lhs, ArrayIterator<RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return (rhs - lhs) > 0;
  }

  template<bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator>(ArrayIterator<LhsIsConstIterator> const & lhs, ArrayIterator<RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return rhs < lhs;
  }

  template<bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator<=(ArrayIterator<LhsIsConstIterator> const & lhs, ArrayIterator<RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return !(lhs > rhs);
  }

  template<bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator>=(ArrayIterator<LhsIsConstIterator> const & lhs, ArrayIterator<RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return !(lhs < rhs);
  }

  auto inline constexpr begin(Array & array)
  {
    return ArrayIterator{array};
  }

  auto inline end(Array & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline constexpr begin(Array const & array)
  {
    return ArrayIterator{array};
  }

  auto inline end(Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline rbegin(Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline constexpr rend(Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline rbegin(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline constexpr rend(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline constexpr cbegin(Array const & array)
  {
    return ArrayIterator{array};
  }

  auto inline cend(Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline crbegin(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline constexpr crend(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  struct ArrayBackInsertIterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = Array;

    explicit constexpr ArrayBackInsertIterator(Array & array)
        : array{array}
    {
    }

    auto operator=(Variant const & value) -> ArrayBackInsertIterator &
    {
      array.push_back(value);
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
    Array & array;
  };

  auto inline constexpr back_inserter(Array & array) -> ArrayBackInsertIterator
  {
    return ArrayBackInsertIterator{array};
  }

}  // namespace godot

#endif