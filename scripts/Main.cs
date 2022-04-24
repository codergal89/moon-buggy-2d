using Godot;

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