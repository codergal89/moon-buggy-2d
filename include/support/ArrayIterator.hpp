#ifndef MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP
#define MOON_BUGGY_2D_SUPPORT_ARRAY_ITERATOR_HPP

#include <Array.hpp>
#include <Color.hpp>
#include <Defs.hpp>
#include <GodotGlobal.hpp>
#include <PoolArrays.hpp>
#include <String.hpp>
#include <Variant.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace godot
{

  template<typename>
  struct is_array : std::false_type
  {
  };

  template<>
  struct is_array<Array> : std::true_type
  {
  };

  template<>
  struct is_array<PoolByteArray> : std::true_type
  {
  };

  template<>
  struct is_array<PoolColorArray> : std::true_type
  {
  };

  template<>
  struct is_array<PoolIntArray> : std::true_type
  {
  };

  template<>
  struct is_array<PoolRealArray> : std::true_type
  {
  };

  template<>
  struct is_array<PoolStringArray> : std::true_type
  {
  };

  template<>
  struct is_array<PoolVector2Array> : std::true_type
  {
  };

  template<>
  struct is_array<PoolVector3Array> : std::true_type
  {
  };

  template<typename, bool = false>
  struct is_pool_array_impl : std::false_type
  {
  };

  template<typename T>
  struct is_pool_array_impl<T, true> : std::true_type
  {
  };

  template<typename T>
  struct is_pool_array : is_pool_array_impl<T, is_array<T>::value && !std::is_same_v<Array, T>>
  {
  };

  template<typename>
  struct array_value_type
  {
    using type = void;
  };

  template<>
  struct array_value_type<Array>
  {
    using type = Variant;
  };

  template<>
  struct array_value_type<PoolByteArray>
  {
    using type = std::uint8_t;
  };

  template<>
  struct array_value_type<PoolColorArray>
  {
    using type = godot::Color;
  };

  template<>
  struct array_value_type<PoolIntArray>
  {
    using type = int;
  };

  template<>
  struct array_value_type<PoolRealArray>
  {
    using type = real_t;
  };

  template<>
  struct array_value_type<PoolStringArray>
  {
    using type = String;
  };

  template<>
  struct array_value_type<PoolVector2Array>
  {
    using type = Vector2;
  };

  template<>
  struct array_value_type<PoolVector3Array>
  {
    using type = Vector3;
  };

  template<typename ArrayType,
           bool IsConstIterator,
           typename CleanArrayType = std::decay_t<ArrayType>,
           bool IsPoolArray = is_pool_array<CleanArrayType>::value,
           typename = std::enable_if_t<is_array<CleanArrayType>::value>>
  struct ArrayIterator
  {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
    using value_type = typename array_value_type<CleanArrayType>::type;
    using pointer = std::conditional_t<IsConstIterator, value_type const *, value_type *>;
    using reference = std::conditional_t<IsConstIterator, value_type const &, value_type &>;

    using array_type = std::conditional_t<IsConstIterator, ArrayType const, ArrayType>;

    auto static constexpr runs_over_pool_array = is_pool_array<CleanArrayType>::value;

    [[nodiscard]] auto pool_access() const noexcept
    {
      if constexpr (IsConstIterator)
      {
        return (*array).read();
      }
      else
      {
        return (*array).write();
      }
    }

    constexpr ArrayIterator() = default;

    explicit constexpr ArrayIterator(array_type & array, difference_type position = 0)
        : array{const_cast<array_type *>(&array)}
        , size{array.size()}
        , position{position}
    {
    }

    auto operator*() const noexcept -> reference
    {
      if constexpr (runs_over_pool_array)
      {
        return pool_access()[position];
      }
      else
      {
        return (*array)[position];
      }
    }

    auto operator->() const noexcept -> pointer
    {
      if constexpr (runs_over_pool_array)
      {
        return &pool_access()[position];
      }
      else
      {
        return &(*array)[position];
      }
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
    auto constexpr operator-(ArrayIterator<ArrayType, OtherIsConstIterator> const & other) const noexcept -> difference_type
    {
      return position - other.position;
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator==(ArrayIterator<ArrayType, OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return (!other.array && position == size) || (!array && other.position == other.size) || position == other.position;
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator!=(ArrayIterator<ArrayType, OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return !(*this == other);
    }

    template<bool OtherIsConstIterator>
    auto constexpr operator<(ArrayIterator<ArrayType, OtherIsConstIterator> const & other) const noexcept -> bool
    {
      return !(*this == other);
    }

  private:
    array_type * array;
    difference_type size;
    difference_type position;
  };

  template<typename ArrayType>
  ArrayIterator(ArrayType &) -> ArrayIterator<ArrayType, std::is_const_v<std::remove_reference_t<ArrayType>>>;

  template<typename ArrayType>
  ArrayIterator(ArrayType &, int) -> ArrayIterator<ArrayType, std::is_const_v<std::remove_reference_t<ArrayType>>>;

  template<typename ArrayType, bool IsConstIterator>
  auto constexpr operator++(ArrayIterator<ArrayType, IsConstIterator> & it, int) noexcept -> ArrayIterator<ArrayType, IsConstIterator>
  {
    auto copy = it;
    ++it;
    return copy;
  }

  template<typename ArrayType, bool IsConstIterator>
  auto constexpr operator--(ArrayIterator<ArrayType, IsConstIterator> & it, int) noexcept -> ArrayIterator<ArrayType, IsConstIterator>
  {
    auto copy = it;
    --it;
    return copy;
  }

  template<typename ArrayType, bool IsConstIterator>
  auto constexpr operator+(ArrayIterator<ArrayType, IsConstIterator> const & it,
                           typename ArrayIterator<ArrayType, IsConstIterator>::difference_type difference) noexcept
      -> ArrayIterator<ArrayType, IsConstIterator>
  {
    auto copy = it;
    return copy += difference;
  }

  template<typename ArrayType, bool IsConstIterator>
  auto constexpr operator+(typename ArrayIterator<ArrayType, IsConstIterator>::difference_type difference,
                           ArrayIterator<ArrayType, IsConstIterator> const & it) noexcept -> ArrayIterator<ArrayType, IsConstIterator>
  {
    auto copy = it;
    return copy += difference;
  }

  template<typename ArrayType, bool IsConstIterator>
  auto constexpr operator-(ArrayIterator<ArrayType, IsConstIterator> const & it,
                           typename ArrayIterator<ArrayType, IsConstIterator>::difference_type difference) noexcept
      -> ArrayIterator<ArrayType, IsConstIterator>
  {
    auto copy = it;
    return copy -= difference;
  }

  template<typename ArrayType, bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator<(ArrayIterator<ArrayType, LhsIsConstIterator> const & lhs,
                           ArrayIterator<ArrayType, RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return (rhs - lhs) > 0;
  }

  template<typename ArrayType, bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator>(ArrayIterator<ArrayType, LhsIsConstIterator> const & lhs,
                           ArrayIterator<ArrayType, RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return rhs < lhs;
  }

  template<typename ArrayType, bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator<=(ArrayIterator<ArrayType, LhsIsConstIterator> const & lhs,
                            ArrayIterator<ArrayType, RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return !(lhs > rhs);
  }

  template<typename ArrayType, bool LhsIsConstIterator, bool RhsIsConstIterator>
  auto constexpr operator>=(ArrayIterator<ArrayType, LhsIsConstIterator> const & lhs,
                            ArrayIterator<ArrayType, RhsIsConstIterator> const & rhs) noexcept -> bool
  {
    return !(lhs < rhs);
  }

  /// Iterator factories for godot::Array

  auto inline begin(Array & array)
  {
    return ArrayIterator{array};
  }

  auto inline end(Array & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline begin(Array const & array)
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

  auto inline rend(Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline rbegin(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline rend(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline cbegin(Array const & array)
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

  auto inline crend(Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolByteArray

  auto inline begin(PoolByteArray & array)
  {
    return ArrayIterator{array};
  }

  auto inline end(PoolByteArray & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline begin(PoolByteArray const & array)
  {
    return ArrayIterator{array};
  }

  auto inline end(PoolByteArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline rbegin(PoolByteArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline rend(PoolByteArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline rbegin(PoolByteArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline rend(PoolByteArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  auto inline cbegin(PoolByteArray const & array)
  {
    return ArrayIterator{array};
  }

  auto inline cend(PoolByteArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }

  auto inline crbegin(PoolByteArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }

  auto inline crend(PoolByteArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolColorArray

  auto inline begin(PoolColorArray & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolColorArray & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolColorArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolColorArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolColorArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolColorArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolColorArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolColorArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolColorArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolColorArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolColorArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolColorArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolIntArray

  auto inline begin(PoolIntArray & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolIntArray & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolIntArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolIntArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolIntArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolIntArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolIntArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolIntArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolIntArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolIntArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolIntArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolIntArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolRealArray

  auto inline begin(PoolRealArray & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolRealArray & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolRealArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolRealArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolRealArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolRealArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolRealArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolRealArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolRealArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolRealArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolRealArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolRealArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolStringArray

  auto inline begin(PoolStringArray & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolStringArray & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolStringArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolStringArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolStringArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolStringArray & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolStringArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolStringArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolStringArray const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolStringArray const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolStringArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolStringArray const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolVector2Array

  auto inline begin(PoolVector2Array & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolVector2Array & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolVector2Array const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolVector2Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolVector2Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolVector2Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolVector2Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolVector2Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolVector2Array const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolVector2Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolVector2Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolVector2Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  /// Iterator factories for godot::PoolVector3Array

  auto inline begin(PoolVector3Array & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolVector3Array & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline begin(PoolVector3Array const & array)
  {
    return ArrayIterator{array};
  }
  auto inline end(PoolVector3Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline rbegin(PoolVector3Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolVector3Array & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline rbegin(PoolVector3Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline rend(PoolVector3Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }
  auto inline cbegin(PoolVector3Array const & array)
  {
    return ArrayIterator{array};
  }
  auto inline cend(PoolVector3Array const & array)
  {
    return ArrayIterator{array, array.size()};
  }
  auto inline crbegin(PoolVector3Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array, array.size()}};
  }
  auto inline crend(PoolVector3Array const & array)
  {
    return std::reverse_iterator{ArrayIterator{array}};
  }

  template<typename ArrayType>
  struct ArrayBackInsertIterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = void;
    using container_type = ArrayType;

    explicit constexpr ArrayBackInsertIterator(container_type & array)
        : array{&array}
    {
    }

    auto operator=(Variant const & value) -> ArrayBackInsertIterator &
    {
      array->push_back(value);
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
    container_type * array;
  };

  template<typename ArrayType>
  auto inline constexpr back_inserter(ArrayType & array) -> ArrayBackInsertIterator<ArrayType>
  {
    return ArrayBackInsertIterator{array};
  }

}  // namespace godot

#endif