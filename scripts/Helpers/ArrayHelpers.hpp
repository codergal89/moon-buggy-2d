#ifndef MB2D_SCRIPTS_HELPERS_ARRAY_HELPERS_HPP
#define MB2D_SCRIPTS_HELPERS_ARRAY_HELPERS_HPP

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/object.hpp>

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace mb2d
{

  namespace detail
  {

    template<typename TargetType>
    auto VariantCast(godot::Variant const & variant)
        -> std::conditional_t<std::is_base_of_v<godot::Object, TargetType>, TargetType *, TargetType>
    {
      if constexpr (std::is_base_of_v<godot::Object, TargetType>)
      {
        return godot::Object::cast_to<TargetType>(variant);
      }
      else
      {
        return static_cast<TargetType>(variant);
      }
    }

  }  // namespace detail

  template<typename ValueType, typename PredicateType>
  auto Filter(godot::TypedArray<ValueType> const & array, PredicateType && predicate) -> godot::TypedArray<ValueType>
    requires(std::invocable<PredicateType, decltype(detail::VariantCast<ValueType>(std::declval<godot::Variant &>()))>)
  {
    auto result = godot::TypedArray<ValueType>{};

    for (auto index = 0ll; index < array.size(); ++index)
    {
      auto element = detail::VariantCast<ValueType>(array[index]);
      if (std::invoke(std::forward<PredicateType>(predicate), element))
      {
        result.push_back(array[index]);
      }
    }

    return result;
  }

  template<typename ValueType, typename PredicateType>
  auto CountIf(godot::TypedArray<ValueType> const & array, PredicateType && predicate) -> decltype(std::declval<godot::Array>().size())
    requires(std::invocable<PredicateType, decltype(detail::VariantCast<ValueType>(std::declval<godot::Variant &>()))>)
  {
    auto result = decltype(std::declval<godot::Array>().size()){};

    for (auto index = 0ll; index < array.size(); ++index)
    {
      auto element = detail::VariantCast<ValueType>(array[index]);
      if (std::invoke(std::forward<PredicateType>(predicate), element))
      {
        ++result;
      }
    }

    return result;
  }

}  // namespace mb2d

#endif