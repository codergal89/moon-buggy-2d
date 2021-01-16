#include "game/Meteor.hpp"

#include <AnimatedSprite.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  auto Meteor::_register_methods() -> void
  {
    godot::register_method("_init", &Meteor::_init);
  }

  auto Meteor::_init() -> void
  {
  }

  auto Meteor::_ready() -> void
  {
    sprite = get_typed_node<godot::AnimatedSprite>("Sprite");
    CRASH_COND(!sprite);
  }

}  // namespace moon_buggy