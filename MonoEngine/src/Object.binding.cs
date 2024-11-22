using System;
using System.Runtime.CompilerServices;

namespace MonoEngine
{
    public class Object
    {
        private IntPtr _cachedPtr;

        public string name
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }

        public override string ToString()
        {
            return $"Object<{name}>";
        }
    }
}
