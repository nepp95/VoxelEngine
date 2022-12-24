using System;

namespace VoxelEngine
{
    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Entity constructor!");
            InternalCalls.NativeLog("Test!", 5);
        }

        private void Log(string text, int param)
        {
            InternalCalls.NativeLog(text, param);
        }

        private Vector3 Log(Vector3 param)
        {
            InternalCalls.NativeLog_Vector(ref param, out Vector3 result);
            return result;
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