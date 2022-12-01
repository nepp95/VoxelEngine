using System;

namespace VoxelEngine
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello world!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"Value is: {value}");
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"Value 1 is: {value1}, value 2 is: {value2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"String is: {message}");
        }
    }
}