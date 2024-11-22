using System.Runtime.CompilerServices;

namespace MonoEngine
{
    public class GameObject : Object
    {
        public GameObject(string name)
        {
            Internal_CreateGameObject(this, name);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Internal_CreateGameObject(GameObject self, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern GameObject Find(string name);
    }
}