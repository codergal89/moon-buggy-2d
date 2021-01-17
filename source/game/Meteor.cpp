#include "game/Meteor.hpp"

#include "core/Constants.hpp"

#include <AnimatedSprite.hpp>
#include <Godot.hpp>
#include <Physics2DDirectBodyState.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{
  namespace
  {
    auto default_entry_angle = real_t{0.0f};
    auto default_entry_speed = real_t{250.f};
  }  // namespace

  auto Meteor::_register_methods() -> void
  {
    godot::register_method("_ready", &Meteor::_ready);
    godot::register_method("_integrate_forces", &Meteor::_integrate_forces);
    godot::register_method("on_screen_exited", &Meteor::on_screen_exited);

    godot::register_property("entry_angle", &Meteor::entry_angle, default_entry_angle);
    godot::register_property("entry_speed", &Meteor::entry_speed, default_entry_speed);
  }

  auto Meteor::_init() -> void
  {
    entry_angle = default_entry_angle;
    entry_speed = default_entry_speed;
  }

  auto Meteor::_ready() -> void
  {
    sprite = get_typed_node<godot::AnimatedSprite>("Sprite");
    CRASH_COND(!sprite);

    sprite->set_animation("fly");
    sprite->play();

    set_linear_velocity(direction_down.rotated(entry_angle) * entry_speed);
  }

  auto Meteor::on_screen_exited() -> void
  {
    queue_free();
  }

  auto Meteor::_integrate_forces(godot::Physics2DDirectBodyState * body_state) -> void
  {
    auto velocity = body_state->get_linear_velocity().normalized();
    auto angle = velocity.angle_to(direction_down);
    set_rotation(-angle);
  }

}  // namespace moon_buggy