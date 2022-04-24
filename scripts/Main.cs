using Godot;
using MoonBuggy2D.scripts.Actors;
using MoonBuggy2D.scripts.UI;

namespace MoonBuggy2D.scripts
{
    public class Main : Node
    {
        public Background Background;
        public Buggy Buggy;

        public override void _Ready()
        {
            Background = GetNode<Background>(nameof(Background));
            Buggy = GetNode<Buggy>(nameof(Buggy));
        }
    }
}