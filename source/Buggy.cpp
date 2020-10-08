#include "Buggy.hpp"

#include <AnimatedSprite.hpp>
#include <Camera2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <KinematicCollision2D.hpp>
#include <ProjectSettings.hpp>
#include <Sprite.hpp>
#include <Vector2.hpp>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>

namespace moon_buggy
{

  auto Buggy::_register_methods() -> void
  {
    godot::register_method("_ready", &Buggy::_ready);
    godot::register_method("kill_zone_entered", &Buggy::kill_zone_entered);

    godot::register_property("acceleration", &Buggy::acceleration, default_acceleration);
    godot::register_property("drag", &Buggy::drag, default_drag);
    godot::register_property("jump_velocity", &Buggy::jump_velocity, default_jump_velocity);
    godot::register_property("speed_limit", &Buggy::speed_limit, default_speed_limit);

    godot::register_signal<Buggy>("crashed", "buggy", GODOT_VARIANT_TYPE_OBJECT);
  }

  auto Buggy::_init() -> void
  {
    gravity = godot::ProjectSettings::get_singleton()->get_setting("physics/2d/default_gravity_vector");
    gravity *= static_cast<real_t>(godot::ProjectSettings::get_singleton()->get_setting("physics/2d/default_gravity"));
  }

  auto Buggy::_ready() -> void
  {
    can_drive = true;
  }

  auto Buggy::stop() -> void
  {
    can_drive = false;
    velocity.x = 0.f;
  }

  auto Buggy::kill_zone_entered(godot::Node * node) -> void
  {
    if (cast_to<Buggy>(node) == this)
    {
      stop();
      emit_signal("crashed", this);
    }
  }

}  // namespace moon_buggy