using System;
using System.Runtime.CompilerServices;

namespace MonoScript
{
    public class ScriptBase
    {
    }

    public class ScriptTest : ScriptBase
    {
        public int PublicIntVal = 10;

        public string Name 
        {
            get => _name; 
            set 
            {
                PublicIntVal += 5;
                _name = value;
            }
        }
        private string _name = "C# - Wangch";

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void CFunction();

        public ScriptTest()
        {
            Console.WriteLine("ScriptTest default constructor!");
        }
        void PrintMessage()
        {
            Console.WriteLine($"C# argument-less method, PublicIntVal: {PublicIntVal}, Name: {Name}");
        }

        void PrintMessage(int value)
        {
            Console.WriteLine($"C# message, argument: {value}");
            CFunction(typeof(ScriptTest));
        }

        static void PrintMessage(string value, string message)
        {
            Console.WriteLine($"C# static message: {value} - {message}");
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void CFunction(System.Type type);
    }
}
