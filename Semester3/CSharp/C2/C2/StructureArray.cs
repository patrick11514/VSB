using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace C2
{
    internal class StructureArray
    {
        public void Run()
        {
            CompositeBrick[] data = new CompositeBrick[2];

            CreateT(ref data[0], 2, 2);
            CreateT(ref data[1], 6, 4);

            while(true)
            {
                MainLoop(data);
                Thread.Sleep(1000);
            } 
        }

        private void MainLoop(CompositeBrick[] data)
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
                    Brick brick = data[i].bricks[j];
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
        private void CreateT(ref CompositeBrick bricks, int x, int y)
        {
            bricks.x = x;
            bricks.y = y;
            bricks.bricks = new Brick[4];

            for (int i = 0; i < 3; i++)
            {
                bricks.bricks[i].x = i;
                bricks.bricks[i].y = 0;
            }

            bricks.bricks[3].x = 1;
            bricks.bricks[3].y = 1;
        }
    }
}
