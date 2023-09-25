using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C2
{
    internal class JaggedArray
    {
        public void Run()
        {
            int width = 10;
            int height = 20;

            bool[][] data = new bool[height][];

            for (int i = 0; i < data.Length; i++)
            {
                data[i] = new bool[width];
            }

            CreateT(data, 1, 1);
            CreateZ(data, 4, 3);

            while (true)
            {
                MainLoop(data);
                Thread.Sleep(1000);
            }
        }

        private void MainLoop(bool[][] data)
        {

            //print
            Console.BackgroundColor = ConsoleColor.Black;
            Console.Clear();
            Console.BackgroundColor = ConsoleColor.Gray;
            Console.ForegroundColor = ConsoleColor.Red;
            for (int y = 0; y < data.Length; y++)
            {
                for (int x = 0; x < data[0].Length; x++)
                {
                    if (data[y][x] == true)
                    {
                        Console.Write("\u2588\u2588");
                    }
                    else
                    {
                        Console.Write("  ");
                    }
                }
                Console.Write("\n");
            }

            //move
            for (int y = data.Length - 1; y > 0; y--)
            {
                data[y] = data[y - 1];
            }

            data[0] = new bool[data[0].Length];

        }

        private void CreateT(bool[][] data, int x, int y)
        {
            data[y][x] = true;
            data[y][x + 1] = true;
            data[y][x + 2] = true;
            data[y + 1][x + 1] = true;
        }

        private void CreateZ(bool[][] data, int x, int y)
        {
            data[y][x] = true;
            data[y][x + 1] = true;
            data[y + 1][x + 1] = true;
            data[y + 1][x + 2] = true;
        }
    }
}
