#ifndef MB2D_SCRIPTS_ACTORS_BUGGY_HPP
#define MB2D_SCRIPTS_ACTORS_BUGGY_HPP

#include "Helpers/DontWarn.hpp"
#include "Helpers/PropertiesGetSet.hpp"

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string.hpp>

#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>

namespace mb2d
{

  enum BuggyAnimation
  {
    Driving,
    Flying,
    Idling,
    Jumping,
    Landing,
  };

  auto inline to_string(BuggyAnimation const & animation) -> godot::String
  {
    switch (animation)
    {
    case BuggyAnimation::Driving:
      return "driving";
    case BuggyAnimation::Flying:
      return "flying";
    case BuggyAnimation::Idling:
      return "idling";
    case BuggyAnimation::Jumping:
      return "jumping";
    case BuggyAnimation::Landing:
      return "landing";
    default:
      CRASH_NOW_MSG("Invalid enum value for buggy animation!");
    }
  }

  struct Buggy
      : godot::CharacterBody2D
      , helpers::EasyProperties<Buggy>
  {
    ENABLE_EASY_PROPERTIES();
    DONT_WARN(GDCLASS(Buggy, godot::CharacterBody2D))

    static auto _bind_methods() -> void
    {
      BIND_ENUM_CONSTANT(BuggyAnimation::Driving);
      BIND_ENUM_CONSTANT(BuggyAnimation::Flying);
      BIND_ENUM_CONSTANT(BuggyAnimation::Idling);
      BIND_ENUM_CONSTANT(BuggyAnimation::Jumping);
      BIND_ENUM_CONSTANT(BuggyAnimation::Landing);

      add_property("animation",
                   &Buggy::current_animation,
                   &Buggy::set_animation,
                   godot::PROPERTY_HINT_ENUM,
                   "Driving,Flying,Idling,Jumping,Landing");
    }

    auto _notification(int notification) -> void
    {
      switch (notification)
      {
      case NOTIFICATION_READY:
        ready();
        set_process(true);
        break;
      }
    }

  private:
    auto ready() -> void
    {
      collision_polygon = get_node<godot::CollisionPolygon2D>("CollisionPolygon");
      sprite = get_node<godot::AnimatedSprite2D>("Sprite");

      ERR_FAIL_NULL(collision_polygon);
      ERR_FAIL_NULL(sprite);

      if (!godot::Engine::get_singleton()->is_editor_hint())
      {
        ready_in_game();
      }
    }

    auto ready_in_game() -> void
    {
      set_animation(current_animation);
    }

    auto set_animation(BuggyAnimation animation) -> void
    {
      current_animation = animation;
      sprite->play(mb2d::to_string(animation));
    }

    godot::CollisionPolygon2D * collision_polygon;
    godot::AnimatedSprite2D * sprite;
    BuggyAnimation current_animation{BuggyAnimation::Idling};
  };
}  // namespace mb2d

VARIANT_ENUM_CAST(mb2d::BuggyAnimation);

#endif