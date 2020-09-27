#include "Buggy.hpp"

#include <Camera2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <KinematicCollision2D.hpp>
#include <ProjectSettings.hpp>
#include <Vector2.hpp>

#include <algorithm>
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

    floor_actions["player_speed_up"] = std::bind(&Buggy::accelerate, this);
    floor_actions["player_slow_down"] = std::bind(&Buggy::decelerate, this);
    floor_actions["player_jump"] = std::bind(&Buggy::jump, this);
  };

  auto Buggy::_ready() -> void
  {
  }

  auto Buggy::_physics_process(float delta) -> void
  {
    handle_gravity(delta);
    handle_drag(delta);
    handle_input();

    move_and_slide(velocity, godot::Vector2{0.f, -1.f});
  }

  auto Buggy::handle_gravity(real_t delta) -> void
  {
    velocity += gravity * delta;
    if (is_on_floor())
    {
      velocity.y = 0.f;
    }
  }

  auto Buggy::handle_drag(real_t delta) -> void
  {
    if (is_on_floor())
    {
      if (velocity.x < 0.f)
      {
        velocity.x = std::clamp(velocity.x + drag * delta, -speed_limit, 0.f);
      }
      else if (velocity.x > 0.f)
      {
        velocity.x = std::clamp(velocity.x - drag * delta, 0.f, speed_limit);
      }
    }
  }

  auto Buggy::handle_input() -> void
  {
    if (is_on_floor())
    {
      for_each(cbegin(floor_actions), cend(floor_actions), [input = godot::Input::get_singleton()](auto action) {
        if (input->is_action_pressed(action.first))
        {
          invoke(action.second);
        }
      });
    }
  }

  auto Buggy::accelerate() -> void
  {
    velocity.x = std::clamp(velocity.x - acceleration, -speed_limit, speed_limit);
  }

  auto Buggy::decelerate() -> void
  {
    velocity.x = std::clamp(velocity.x + acceleration, -speed_limit, speed_limit);
  }

  auto Buggy::jump() -> void
  {
    velocity.y = -jump_velocity;
  }

  auto Buggy::kill_zone_entered(godot::Node * node) -> void
  {
    if (cast_to<Buggy>(node) == this)
    {
      emit_signal("crashed", this);
    }
  }

}  // namespace moon_buggy