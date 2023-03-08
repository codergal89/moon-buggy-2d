#ifndef MB2D_SCRIPTS_ACTORS_METEOR_HPP
#define MB2D_SCRIPTS_ACTORS_METEOR_HPP

#include "Helpers/DontWarn.hpp"
#include "Helpers/PropertiesGetSet.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_direct_body_state2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/visible_on_screen_notifier2d.hpp>

namespace mb2d
{

  struct Meteor
      : godot::RigidBody2D
      , helpers::EasyProperties<Meteor>
  {
    auto static constexpr cStartingVelocity = 800.0;

    ENABLE_EASY_PROPERTIES();
    DONT_WARN(GDCLASS(Meteor, godot::RigidBody2D))

    auto static _bind_methods() -> void
    {
      add_property("starting_angle", &Meteor::starting_angle, 0, 180, 0.1);
    }

    auto _ready() -> void override
    {
      collision_shape = get_node<godot::CollisionShape2D>("%CollisionShape");
      flying_sprite = get_node<godot::AnimatedSprite2D>("%FlyingSprite");
      impacted_sprite = get_node<godot::AnimatedSprite2D>("%ImpactedSprite");
      visibility_notifier = get_node<godot::VisibleOnScreenNotifier2D>("%VisibilityNotifier");

      ERR_FAIL_NULL(collision_shape);
      ERR_FAIL_NULL(flying_sprite);
      ERR_FAIL_NULL(impacted_sprite);
      ERR_FAIL_NULL(visibility_notifier);

      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        ready_in_game();
      }
    }

    auto ready_in_game() -> void
    {
      flying_sprite->play();
      impacted_sprite->set_visible(false);

      auto radians = godot::Math::deg_to_rad(starting_angle);
      apply_impulse(godot::Vector2::from_angle(radians) * cStartingVelocity);
      set_rotation(radians);

      visibility_notifier->connect("screen_exited", {this, "queue_free"});
    }

    auto _integrate_forces(godot::PhysicsDirectBodyState2D * state) -> void override
    {
      auto right = godot::Vector2(1.0, 0.0);
      auto direction_of_travel = state->get_linear_velocity();
      auto angle = right.angle_to(direction_of_travel);
      set_rotation(angle);
    }

  private:
    double starting_angle;

    godot::CollisionShape2D * collision_shape;
    godot::AnimatedSprite2D * flying_sprite;
    godot::AnimatedSprite2D * impacted_sprite;
    godot::VisibleOnScreenNotifier2D * visibility_notifier;
  };
}  // namespace mb2d

#endif