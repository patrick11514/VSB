using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C2
{
    internal class MultidimensionalArray
    {
        public void Run()
        {
            bool[,] data = new bool[10, 20];

            CreateT(data, 1, 1);
            CreateZ(data, 4, 3);

            while (true)
            {
                MainLoop(data);
                Thread.Sleep(1000);
            }
        }

        private void MainLoop(bool[,] data)
        {

            //print
            Console.BackgroundColor = ConsoleColor.Black;
            Console.Clear();
            Console.BackgroundColor = ConsoleColor.Gray;
            Console.ForegroundColor = ConsoleColor.Red;
            for (int y = 0; y < data.GetLength(1); y++)
            {
                for (int x = 0; x < data.GetLength(0); x++)
                {
                    if (data[x, y] == true)
                    {
                        Console.Write("\u2588\u2588");
                    } else
                    {
                        Console.Write("  ");
                    }
                }
                Console.Write("\n");
            }

            //move
            for (int y = data.GetLength(1) - 1; y >  0; y--)
            {
                for (int x = 0; x < data.GetLength(0); x++)
                {
                    if (data[x, y] == true)
                    {
                        int newY = y + 1;
                        if (newY < data.GetLength(1))
                        {
                            data[x, y] = false; 
                            data[x, newY] = true;
                        } else
                        {
                            data[x, y] = false;
                        }
                    }
                }
            }
        }

        private void CreateT(bool[,] data, int x, int y)
        {
            data[x, y] = true;
            data[x + 1, y] = true;
            data[x + 2, y] = true;
            data[x + 1, y + 1] = true;
        }

        private void CreateZ(bool[,] data, int x, int y)
        {
            data[x, y] = true;
            data[x + 1, y] = true;
            data[x + 1, y + 1] = true;
            data[x + 2, y + 1] = true;
        }
    }
}
