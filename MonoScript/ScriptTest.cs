using System;
using System.Runtime.InteropServices;

namespace MonoScript
{
    public class ScriptTest
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
        }

        static void PrintMessage(string value, string message)
        {
            Console.WriteLine($"C# static message: {value} - {message}");
        }
    }
}
