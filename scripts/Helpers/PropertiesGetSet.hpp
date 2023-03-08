#ifndef MB2D_SCRIPTS_HELPERS_PROPERTIES_GET_SET_HPP
#define MB2D_SCRIPTS_HELPERS_PROPERTIES_GET_SET_HPP

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/templates/list.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iterator>
#include <map>
#include <tuple>
#include <type_traits>

namespace mb2d::helpers
{
  namespace detail
  {
    template<typename T>
    concept Object = std::is_base_of_v<godot::Object, T>;

    template<typename T>
    struct RemoveRef
    {
      using type = T;
    };

    template<typename T>
    struct RemoveRef<godot::Ref<T>> : RemoveRef<T>
    {
    };

    template<typename T>
    using RemoveRefT = typename RemoveRef<T>::type;

    template<typename ValueType>
    auto constexpr GetPropertyType()
      requires std::integral<ValueType>
    {
      return godot::Variant::Type::INT;
    }

    template<typename ValueType>
    auto constexpr GetPropertyType()
      requires std::floating_point<ValueType>
    {
      return godot::Variant::Type::FLOAT;
    }

    template<typename ValueType>
    auto constexpr GetPropertyType()
      requires Object<ValueType>
    {
      return godot::Variant::Type::OBJECT;
    }

    template<typename ValueType>
    auto constexpr GetPropertyType()
      requires std::is_enum_v<ValueType>
    {
      return godot::Variant::Type::INT;
    }

    template<typename ValueType>
    auto constexpr PropertyType = GetPropertyType<RemoveRefT<ValueType>>();

    template<typename ValueType>
    auto constexpr GetPropertyHint()
    {
      return godot::PROPERTY_HINT_NONE;
    }

    template<typename ValueType>
    auto constexpr GetPropertyHint()
      requires std::is_enum_v<ValueType>
    {
      return godot::PROPERTY_HINT_ENUM;
    }

    template<typename ValueType>
    auto constexpr GetPropertyHint()
      requires std::is_base_of_v<godot::Resource, ValueType>
    {
      return godot::PROPERTY_HINT_RESOURCE_TYPE;
    }

    template<typename ValueType>
    auto constexpr PropertyHint = GetPropertyHint<RemoveRefT<ValueType>>();

    template<typename ValueType>
    auto constexpr GetPropertyHintString()
    {
      return "";
    }

    template<typename ValueType>
    auto GetPropertyHintString()
      requires std::is_base_of_v<godot::Resource, RemoveRefT<ValueType>>
    {
      return RemoveRefT<ValueType>::get_class_static();
    }

    template<typename EnumType, typename SourceType>
    auto constexpr EnumCast(SourceType value)
      requires std::is_enum_v<EnumType>
    {
      return static_cast<EnumType>(static_cast<std::underlying_type_t<EnumType>>(value));
    }

    template<typename NonEnumType>
    auto constexpr EnumCast(NonEnumType value)
    {
      return value;
    }
  }  // namespace detail

  template<typename DerivedType>
  struct EasyProperties
  {
    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType (DerivedType::*getter)() const,
                             void (DerivedType::*setter)(ValueType),
                             godot::PropertyHint hint = detail::PropertyHint<ValueType>,
                             const godot::String & hint_string = detail::GetPropertyHintString<ValueType>(),
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto info = godot::PropertyInfo{detail::PropertyType<ValueType>, name, hint, hint_string, usage};
      properties[name] = std::make_tuple(info, std::mem_fn(getter), std::mem_fn(setter));
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType (DerivedType::*getter)() const,
                             void (DerivedType::*setter)(ValueType),
                             std::type_identity_t<ValueType> minimum,
                             std::type_identity_t<ValueType> maximum,
                             std::type_identity_t<ValueType> step,
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto hint = godot::String{"{},{},{}"}.format(godot::Array::make(minimum, maximum, step), "{}");
      add_property(name, getter, setter, godot::PROPERTY_HINT_RANGE, hint, usage);
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType DerivedType::*property,
                             godot::PropertyHint hint = detail::PropertyHint<ValueType>,
                             const godot::String & hint_string = detail::GetPropertyHintString<ValueType>(),
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto info = godot::PropertyInfo{detail::PropertyType<ValueType>, name, hint, hint_string, usage};
      properties[name] = std::make_tuple(
          info,
          [=](DerivedType const * object) { return object->*property; },
          [=](DerivedType * object, godot::Variant const & value) { object->*property = detail::EnumCast<ValueType>(value); });
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType DerivedType::*property,
                             void (DerivedType::*setter)(ValueType),
                             godot::PropertyHint hint = detail::PropertyHint<ValueType>,
                             const godot::String & hint_string = detail::GetPropertyHintString<ValueType>(),
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto info = godot::PropertyInfo{detail::PropertyType<ValueType>, name, hint, hint_string, usage};
      properties[name] = std::make_tuple(
          info,
          [=](DerivedType const * object) { return object->*property; },
          [=](DerivedType * object, godot::Variant value) { (object->*setter)(detail::EnumCast<ValueType>(value)); });
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType DerivedType::*property,
                             std::type_identity_t<ValueType> minimum,
                             std::type_identity_t<ValueType> maximum,
                             std::type_identity_t<ValueType> step,
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto hint = godot::String{"{},{},{}"}.format(godot::Array::make(minimum, maximum, step), "{}");
      add_property(name, property, godot::PROPERTY_HINT_RANGE, hint, usage);
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType DerivedType::*property,
                             void (DerivedType::*setter)(ValueType),
                             std::type_identity_t<ValueType> minimum,
                             std::type_identity_t<ValueType> maximum,
                             std::type_identity_t<ValueType> step,
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto hint = godot::String{"{},{},{}"}.format(godot::Array::make(minimum, maximum, step), "{}");
      auto info = godot::PropertyInfo{detail::PropertyType<ValueType>, name, godot::PROPERTY_HINT_RANGE, hint, usage};
      properties[name] = std::make_tuple(
          info,
          [=](DerivedType const * object) { return object->*property; },
          [=](DerivedType * object, godot::Variant value) { (object->*setter)(detail::EnumCast<ValueType>(value)); });
    }

    template<typename ValueType>
    auto static add_property(godot::StringName name,
                             ValueType (DerivedType::*getter)() const,
                             godot::PropertyHint hint = detail::PropertyHint<ValueType>,
                             const godot::String & hint_string = detail::GetPropertyHintString<ValueType>(),
                             uint32_t usage = godot::PROPERTY_USAGE_DEFAULT)
    {
      auto info = godot::PropertyInfo{detail::PropertyType<ValueType>, name, hint, hint_string, usage};
      properties[name] = std::make_tuple(info, std::mem_fn(getter), nullptr);
    }

    auto _get_property_list(godot::List<godot::PropertyInfo> * list) const -> void
    {
      std::for_each(cbegin(properties), cend(properties), [&](auto const & entry) { list->push_back(std::get<0>(entry.second)); });
    }

    auto _get(godot::StringName const & name, godot::Variant & output) const -> bool
    {
      if (!(properties.contains(name) && std::get<1>(properties[name])))
      {
        return false;
      }

      output = std::get<1>(properties.at(name))(static_cast<DerivedType const *>(this));
      return true;
    }

    auto _set(godot::StringName const & name, godot::Variant const & value) -> bool
    {
      if (!(properties.contains(name) && std::get<2>(properties[name])))
      {
        return false;
      }

      std::get<2>(properties.at(name))(static_cast<DerivedType *>(this), value);
      return true;
    }

  private:
    auto static inline properties = std::map<godot::StringName,
                                             std::tuple<godot::PropertyInfo,
                                                        std::function<godot::Variant(DerivedType const *)>,
                                                        std::function<void(DerivedType *, godot::Variant)>>>{};
  };
}  // namespace mb2d::helpers

#define ENABLE_EASY_PROPERTIES() using EasyProperties::_get, EasyProperties::_set, EasyProperties::_get_property_list

#endif