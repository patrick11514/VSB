using System;

namespace C1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string name = Console.ReadLine();
            name = name.ToUpper();

            Console.WriteLine("Ahoj: " + name);
        }
    }
}