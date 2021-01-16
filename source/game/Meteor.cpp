#include "game/Meteor.hpp"

#include <AnimatedSprite.hpp>
#include <Godot.hpp>

namespace moon_buggy
{

  auto Meteor::_register_methods() -> void
  {
    godot::register_method("_ready", &Meteor::_ready);
    godot::register_method("on_screen_exited", &Meteor::on_screen_exited);
  }

  auto Meteor::_init() -> void
  {
  }

  auto Meteor::_ready() -> void
  {
    sprite = get_typed_node<godot::AnimatedSprite>("Sprite");
    CRASH_COND(!sprite);

    sprite->set_animation("fly");
    sprite->play();
  }

}  // namespace moon_buggy