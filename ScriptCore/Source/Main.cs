using System;

namespace VoxelEngine
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world!");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine(message);
        }
    }
}