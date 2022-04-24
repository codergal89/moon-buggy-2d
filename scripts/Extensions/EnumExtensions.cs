using System;

namespace MoonBuggy2D.scripts.Extensions
{
    public static class EnumExtensions
    {
        #region Private Methods

        private static void EnsureIsEnum<T>() where T : struct
        {
            if (!typeof(T).IsEnum)
                throw new InvalidOperationException("Generic type argument is not an enum");
        }

        #endregion

        #region Public Methods

        public static string Name<T>(this T animation) where T : struct
        {
            EnsureIsEnum<T>();
            return animation.ToString().ToLower();
        }

        public static T ParseEnum<T>(string value) where T : struct
        {
            EnsureIsEnum<T>();
            return (T)Enum.Parse(typeof(T), value);
        }

        #endregion
    }
}