using Godot;

namespace MoonBuggy2D.scripts
{
    public class Main : Node
    {
        public Buggy Buggy;
        public override void _Ready()
        {
            Buggy = GetNode<Buggy>(nameof(Buggy));
        }
    }
}