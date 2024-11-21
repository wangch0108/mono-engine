using System.Runtime.CompilerServices;

namespace MonoEngine
{
    public class GameObject
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern GameObject Find(string name);
    }
}