using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C2
{
    internal class ClassArray
    {
        public void Run()
        {
            CompositeBrickClass[] data = new CompositeBrickClass[2];

            CreateT(data[0], 2, 2);
            CreateT(data[1], 6, 4);

            while (true)
            {
                MainLoop(data);
                Thread.Sleep(1000);
            }
        }

        private void MainLoop(CompositeBrickClass[] data)
        {
            //print
            Console.BackgroundColor = ConsoleColor.Black;
            Console.Clear();
            Console.BackgroundColor = ConsoleColor.Gray;
            Console.ForegroundColor = ConsoleColor.Red;

            //print
            for (int i = 0; i < data.Length; i++)
            {
                for (int j = 0; j < data[i].bricks.Length; j++)
                {
                    BrickClass brick = data[i].bricks[j];
                    Console.SetCursorPosition(data[i].x + (brick.x * 2), data[i].y + brick.y);

                    Console.Write("\u2588\u2588");
                }
            }

            //move
            for (int i = 0; i < data.Length; i++)
            {
                data[i].y++;
            }
        }
        private void CreateT(CompositeBrickClass bricks, int x, int y)
        {
            bricks = new()
            {
                x = x,
                y = y,
                bricks = new BrickClass[4]
            };

            for (int i = 0; i < 3; i++)
            {
                bricks.bricks[i] = new()
                {
                    x = i,
                    y = 0
                };
            }

            bricks.bricks[3] = new()
            {
                x = 1,
                y = 1
            };
        }
    }
}
