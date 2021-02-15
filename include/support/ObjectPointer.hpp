#ifndef MOON_BUGGY_2D_SUPPORT_OBJECT_POINTER_HPP
#define MOON_BUGGY_2D_SUPPORT_OBJECT_POINTER_HPP

#include <memory>

namespace moon_buggy
{

  template<typename ObjectType>
  struct object_deleter
  {
    auto constexpr operator()(ObjectType * object) const -> void
    {
      object->free();
    }
  };

  template<typename ObjectType>
  using ObjectPointer = std::unique_ptr<ObjectType, object_deleter<ObjectType>>;

  template<typename ObjectType>
  auto make_object_ptr(ObjectType * object) -> ObjectPointer<ObjectType>
  {
    return ObjectPointer<ObjectType>{object};
  }

}  // namespace moon_buggy

#endif