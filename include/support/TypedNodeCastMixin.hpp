#ifndef MOON_BUGGY_2D_SUPPORT_TYPED_NODE_CAST_MIXIN_HPP
#define MOON_BUGGY_2D_SUPPORT_TYPED_NODE_CAST_MIXIN_HPP

#include "support/ArrayIterator.hpp"

#include <NodePath.hpp>
#include <Object.hpp>

#include <algorithm>
#include <type_traits>
#include <utility>

template<typename CRTP>
struct TypedNodeCastMixin
{
  template<typename TargetType>
  [[nodiscard]] auto get_typed_node(godot::NodePath path = "") const -> TargetType *
  {
    static_assert(std::is_base_of_v<godot::Object, CRTP>);

    auto & that = *static_cast<CRTP const *>(this);
    if (path.is_empty())
    {
      auto children = that.get_children();
      for (auto child : children)
      {
        if (auto found = that.template cast_to<TargetType>(child))
        {
          return found;
        }
      }
      return nullptr;
    }
    else
    {
      return that.template cast_to<TargetType>(that.get_node(std::move(path)));
    }
  }
};

#endif