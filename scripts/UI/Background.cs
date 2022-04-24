using Godot;
using MoonBuggy2D.scripts.Extensions;

namespace MoonBuggy2D.scripts.UI
{
    public class Background : ParallaxBackground
    {
        public ParallaxLayer Space;
        public AnimatedSprite SpaceSprite;
        public ParallaxLayer Stars;
        public AnimatedSprite StarsSprite;

        public ColorTheme Theme
        {
            get => EnumExtensions.ParseEnum<ColorTheme>(SpaceSprite.Animation.ToLower());
            set
            {
                SpaceSprite.Animation = value.Name();
                StarsSprite.Animation = value.Name();
            }
        }
        
        public override void _Ready()
        {
            Space = GetNode<ParallaxLayer>(nameof(Space));
            SpaceSprite = Space.GetNode<AnimatedSprite>("Sprite");
            Stars = GetNode<ParallaxLayer>(nameof(Stars));
            StarsSprite = Stars.GetNode<AnimatedSprite>("Sprite");
        }
    }
}
