using System;

namespace C2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            /*
            //normální pole
            int[] arr = new int[3];

            //pole polí, tedy pole, které má v sobě pointery na jiné pole
            int [][] arrayOfArrays = new int[2][];
            arrayOfArrays[1] = arr;
            arrayOfArrays[1][1] = 5;

            //více rozmněrné pole (tedy něco co jsme dělali v Cčku)
            int [,] multiArray = new int[2, 2];
            multiArray[1, 1] = 5;
            */

            //MultidimensionalArray v1 = new MultidimensionalArray();
            ///JaggedArray v1 = new JaggedArray();
            StructureArray v1 = new StructureArray();


            //Nefunguje XD
            //ClassArray v1 = new ClassArray();

            v1.Run();

            Console.WriteLine("Hello World!");
        }
    }
}