using Godot;
using MoonBuggy2D.extensions;

namespace MoonBuggy2D.scripts
{
    public class Meteorite : RigidBody2D
    {
        public CollisionShape2D CollisionShape;
        public AnimatedSprite FlyingSprite;
        public AnimatedSprite ImpactedSprite;
        public VisibilityNotifier2D VisibilityNotifier;
 
        public ColorTheme Theme
        {
            get => EnumExtensions.ParseEnum<ColorTheme>(ImpactedSprite.Animation.ToLower());
            set => ImpactedSprite.Animation = value.Name().ToLower();
        }

        public int ShapeCount
        {
            get => ImpactedSprite.Frames.GetFrameCount(ImpactedSprite.Animation);
        }

        public override void _Ready()
        {
            CollisionShape = GetNode<CollisionShape2D>(nameof(CollisionShape));
            FlyingSprite = GetNode<AnimatedSprite>(nameof(FlyingSprite));
            ImpactedSprite = GetNode<AnimatedSprite>(nameof(ImpactedSprite));
            VisibilityNotifier = GetNode<VisibilityNotifier2D>(nameof(VisibilityNotifier));

            FlyingSprite.Playing = true;
            ImpactedSprite.Visible = false;
        }
    }
}