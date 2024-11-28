using System.Runtime.CompilerServices;

namespace MonoEngine
{
    public class GameObject : Object
    {
        public GameObject(string name)
        {
            Internal_CreateGameObject(this, name);
        }

        public T AddComponent<T>() where T : MonoBehaviour => this.AddComponent(typeof(T)) as T;
        public MonoBehaviour AddComponent(System.Type componentType) => this.Internal_AddComponentWithType(componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Internal_CreateGameObject(GameObject self, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern GameObject Find(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern MonoBehaviour Internal_AddComponentWithType(System.Type componentType);
    }
}