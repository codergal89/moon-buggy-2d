#ifndef MOON_BUGGY_2D_BUGGY_HPP
#define MOON_BUGGY_2D_BUGGY_HPP

#include "TypedNodeCastMixin.hpp"

#include <Defs.hpp>
#include <Godot.hpp>
#include <Physics2DDirectBodyState.hpp>
#include <RigidBody2D.hpp>
#include <Vector2.hpp>

namespace moon_buggy
{

  struct Buggy
      : godot::RigidBody2D
      , TypedNodeCastMixin<Buggy>
  {
    auto static constexpr default_acceleration{.25f};
    auto static constexpr default_base_speed{0.f};
    auto static constexpr default_drag{.1f};
    auto static constexpr default_jump_velocity{98.f};
    auto static constexpr default_speed_limit{100.f};

    auto static _register_methods() -> void;

    auto _init() -> void;
    auto _ready() -> void;

  private:
    GODOT_CLASS(Buggy, godot::RigidBody2D)  // NOLINT

    auto _integrate_forces(godot::Physics2DDirectBodyState * state) -> void;

    auto update_collision_state(godot::Physics2DDirectBodyState const * state) -> void;
    auto update_drive_speed() -> void;
    [[nodiscard]] auto should_jump() const -> bool;

    auto stop() -> void;

    auto kill_zone_entered(godot::Node * node) -> void;

    real_t acceleration{default_acceleration};
    real_t base_speed{default_base_speed};
    real_t drag{default_drag};
    real_t jump_velocity{default_jump_velocity};
    real_t speed_limit{default_speed_limit};

    real_t current_drive_velocity{};

    bool can_drive{};
    bool is_on_floor{};
    bool bumped_into_something{};
  };

}  // namespace moon_buggy

#endif