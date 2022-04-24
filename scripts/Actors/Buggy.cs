using Godot;
using MoonBuggy2D.extensions;

namespace MoonBuggy2D.scripts.Actors
{
    public class Buggy : KinematicBody2D
    {
        public enum Animation
        {
            Driving,
            Flying,
            Idling,
            Jumping,
            Landing
        }

        public CollisionPolygon2D CollisionPolygon;
        public AnimatedSprite Sprite;

        public Animation CurrentAnimation
        {
            get => EnumExtensions.ParseEnum<Animation>(Sprite.Animation.Capitalize());
            set => Sprite.Animation = value.Name();
        }

        public override void _Ready()
        {
            CollisionPolygon = GetNode<CollisionPolygon2D>(nameof(CollisionPolygon));
            Sprite = GetNode<AnimatedSprite>(nameof(Sprite));
            CurrentAnimation = Animation.Idling;
            Sprite.Playing = true;
        }
    }
}