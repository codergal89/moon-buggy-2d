#ifndef MB2D_SCRIPTS_ACTORS_METEORITE_HPP
#define MB2D_SCRIPTS_ACTORS_METEORITE_HPP

#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/collision_shape2d.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/visible_on_screen_notifier2d.hpp"

#include "godot_cpp/core/class_db.hpp"

namespace mb2d
{

    struct Meteorite : godot::RigidBody2D
    {
        auto static constexpr cStartingVelocity = 1000.0;

        GDCLASS(Meteorite, godot::RigidBody2D);

        static auto _bind_methods() -> void
        {
        }

        auto _ready() -> void override
        {
            collision_shape = get_node<godot::CollisionPolygon2D>("CollisionShape");
            flying_sprite = get_node<godot::AnimatedSprite2D>("FlyingSprite");
            impacted_sprite = get_node<godot::AnimatedSprite2D>("ImpactedSprite");
            visibility_notifier = get_node<godot::VisibleOnScreenNotifier2D>("VisibilityNotifier");

            flying_sprite->play();
            impacted_sprite->set_visible(false);

            set_linear_velocity(godot::Vector2{1.0, 1.0} * cStartingVelocity);
        }

    private:
        godot::CollisionPolygon2D *collision_shape;
        godot::AnimatedSprite2D *flying_sprite;
        godot::AnimatedSprite2D *impacted_sprite;
        godot::VisibleOnScreenNotifier2D *visibility_notifier;
    };
}

#endif