#include "game/Buggy.hpp"

#include <AnimatedSprite.hpp>
#include <Input.hpp>
#include <Physics2DDirectBodyState.hpp>
#include <Variant.hpp>
#include <Vector2.hpp>

#include <cmath>

namespace moon_buggy
{

  auto Buggy::_register_methods() -> void
  {
    godot::register_method("_ready", &Buggy::_ready);
    godot::register_method("_integrate_forces", &Buggy::_integrate_forces);
    godot::register_method("_process", &Buggy::_process);
    godot::register_method("animation_played", &Buggy::animation_played);
    godot::register_method("kill_zone_entered", &Buggy::kill_zone_entered);

    godot::register_property("acceleration", &Buggy::acceleration, default_acceleration);
    godot::register_property("base_speed", &Buggy::base_speed, default_base_speed);
    godot::register_property("drag", &Buggy::drag, default_drag);
    godot::register_property("jump_velocity", &Buggy::jump_velocity, default_jump_velocity);
    godot::register_property("speed_limit", &Buggy::speed_limit, default_speed_limit);

    godot::register_signal<Buggy>("crashed", "buggy", GODOT_VARIANT_TYPE_OBJECT);
  }

  auto Buggy::_init() -> void
  {
  }

  auto Buggy::_ready() -> void
  {
    can_drive = true;

    sprite = get_typed_node<godot::AnimatedSprite>("Sprite");
    sprite->set_animation("idle");
    sprite->connect("animation_finished", this, "animation_played");
    sprite->play();
  }

  auto Buggy::_process(real_t frame_time) -> void
  {
    static_cast<void>(frame_time);
    update_animation();
  }

  auto Buggy::_integrate_forces(godot::Physics2DDirectBodyState * state) -> void
  {
    update_collision_state(state);

    update_drive_speed();
    set_axis_velocity(godot::Vector2{1.f, 0.f} * (current_drive_velocity - base_speed));

    if (should_jump())
    {
      set_axis_velocity(godot::Vector2{0.0f, -1.f} * jump_velocity);
    }

    auto linear_velocity = state->get_linear_velocity();
    auto step = state->get_step();
    linear_velocity += state->get_total_gravity() * step;
    state->set_linear_velocity(linear_velocity);
  }

  auto Buggy::update_collision_state(godot::Physics2DDirectBodyState const * state) -> void
  {
    is_on_floor = false;
    bumped_into_something = false;

    for (auto i{0}; i < state->get_contact_count(); ++i)
    {
      auto contact_normal = state->get_contact_local_normal(i);
      if (contact_normal.dot({0, -1}) >= 0.9)
      {
        is_on_floor = true;
      }
      else if (contact_normal.dot({1, 0}) >= 0.9)
      {
        bumped_into_something = true;
      }
    }
  }

  auto Buggy::update_drive_speed() -> void
  {
    if (is_on_floor)
    {
      auto input = godot::Input::get_singleton();
      auto accelerate = input->is_action_pressed("player_speed_up");
      auto decelerate = input->is_action_pressed("player_slow_down");
      auto drive_direction = accelerate * -1 + decelerate * 1;

      if (drive_direction)
      {
        current_drive_velocity = std::lerp(current_drive_velocity, static_cast<real_t>(drive_direction) * speed_limit, acceleration);
      }
      else
      {
        current_drive_velocity = std::lerp(current_drive_velocity, 0.f, drag);
      }
    }

    if (bumped_into_something)
    {
      current_drive_velocity = 0;
    }
  }

  auto Buggy::should_jump() const -> bool
  {
    auto input = godot::Input::get_singleton();
    return is_on_floor && input->is_action_pressed("player_jump");
  }

  auto Buggy::update_animation() -> void
  {
    if (is_on_floor && !was_on_floor)
    {
      was_on_floor = true;
      sprite->set_animation("land");
    }
    else if (is_on_floor && landed)
    {
      landed = false;
      sprite->set_animation("move");
    }
    else if (!is_on_floor && was_on_floor)
    {
      was_on_floor = false;
      sprite->set_animation("jump_off");
    }
    else if (!is_on_floor && jumped_off)
    {
      jumped_off = false;
      sprite->set_animation("fly");
    }
  }

  auto Buggy::animation_played() -> void
  {
    if (sprite->get_animation() == "land")
    {
      landed = true;
    }
    else if (sprite->get_animation() == "jump_off")
    {
      jumped_off = true;
    }
  }

  auto Buggy::stop() -> void
  {
    can_drive = false;
    set_linear_velocity({0, 0});
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