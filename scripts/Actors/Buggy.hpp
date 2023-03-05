#ifndef MB2D_SCRIPTS_ACTORS_BUGGY_HPP
#define MB2D_SCRIPTS_ACTORS_BUGGY_HPP

#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/classes/collision_polygon2d.hpp"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/core/error_macros.hpp"

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

    auto to_string(BuggyAnimation const &animation) -> godot::String
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

    struct Buggy : godot::CharacterBody2D
    {
        GDCLASS(Buggy, godot::CharacterBody2D);

        static auto _bind_methods() -> void
        {
            BIND_ENUM_CONSTANT(BuggyAnimation::Driving);
            BIND_ENUM_CONSTANT(BuggyAnimation::Flying);
            BIND_ENUM_CONSTANT(BuggyAnimation::Idling);
            BIND_ENUM_CONSTANT(BuggyAnimation::Jumping);
            BIND_ENUM_CONSTANT(BuggyAnimation::Landing);

            godot::ClassDB::bind_method(godot::D_METHOD("set_animation", "animation"), &Buggy::set_animation);
            godot::ClassDB::bind_method(godot::D_METHOD("get_animation"), &Buggy::get_animation);
            ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "animation", godot::PropertyHint::PROPERTY_HINT_ENUM, "Driving, Flying, Idling, Jumping, Landing"), "set_animation", "get_animation");
        }

        auto _ready() -> void override
        {
            collision_polygon = get_node<godot::CollisionPolygon2D>("CollisionPolygon");
            sprite = get_node<godot::AnimatedSprite2D>("Sprite");
            set_animation(BuggyAnimation::Idling);
        }

        auto get_animation() const -> BuggyAnimation
        {
            return current_animation;
        }

        auto set_animation(BuggyAnimation animation) -> void
        {
            current_animation = animation;
            if (sprite)
            {
                sprite->play(mb2d::to_string(animation));
            }
        }

    private:
        godot::CollisionPolygon2D *collision_polygon;
        godot::AnimatedSprite2D *sprite;
        BuggyAnimation current_animation{BuggyAnimation::Idling};
    };
}

VARIANT_ENUM_CAST(mb2d::BuggyAnimation);

#endif