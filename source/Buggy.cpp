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
    godot::register_method("_physics_process", &Buggy::_physics_process);
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

  auto Buggy::_physics_process(float delta) -> void
  {
    apply_gravity(delta);
    handle_input();
  }

  auto Buggy::apply_gravity(real_t delta) -> void
  {
    velocity.y += gravity.y * delta;
    velocity = move_and_slide(velocity, godot::Vector2{0.f, -1.f});
  }

  auto Buggy::handle_input() -> void
  {
    auto input{godot::Input::get_singleton()};
    auto direction{0};

    direction += static_cast<int>(input->is_action_pressed("player_slow_down"));
    direction -= static_cast<int>(input->is_action_pressed("player_speed_up"));

    if (is_on_floor())
    {
      if (direction)
      {
        accelerate(direction);
      }
      else
      {
        apply_drag();
      }

      if (input->is_action_pressed("player_jump"))
      {
        jump();
      }
    }
  }

  auto Buggy::accelerate(int direction) -> void
  {
    velocity.x = std::lerp(velocity.x, static_cast<real_t>(direction) * speed_limit, acceleration);
  }

  auto Buggy::apply_drag() -> void
  {
    velocity.x = std::lerp(velocity.x, 0.f, drag);
  }

  auto Buggy::jump() -> void
  {
    velocity.y = -jump_velocity;
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