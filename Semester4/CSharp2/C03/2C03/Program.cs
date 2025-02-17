namespace _2C03
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            SimpleStack<int> stack = new();

            Random rand = new();

            object lockObject = new();

            for (int i = 0; i < 5;i++)
            {
                Thread thread = new(() =>
                {
                    while (true)
                    {
                        if (rand.NextDouble() < 0.6)
                        {
                            lock (lockObject)
                            {
                                if (stack.IsEmpty)
                                {
                                    int x = stack.Pop();
                                }
                            }
                        } else
                        {
                            stack.Push(rand.Next());
                        }
                    }
                });

                //thread.Start();
            }

            int x = 0;

            Thread t = new (() =>
            {
                for (int i = 0; i < 10000; i++)
                {
                    Console.WriteLine("");
                }

                x++;
            });

            //t.Start();

            //t.Join();
            //Console.WriteLine(x);

            Thread main = new(() =>
            {
                while (true)
                {
                    lock (lockObject)
                    {
                        stack.Push(rand.Next());
                        Monitor.Pulse(lockObject);
                    }
                    Thread.Sleep(100);
                }
            });

            //main.Start();

            for (int i = 0; i < 5; i++)
            {
                Thread thread = new(() =>
                {
                    while (true)
                    {
                        int? x = null;
                        lock (lockObject){
                            if (!stack.IsEmpty) {
                                x = stack.Pop();
                            } else
                            {
                                Monitor.Wait(lockObject);
                            }
                        }

                        if (x.HasValue)
                        {
                            Console.WriteLine("Hodnota: " + x.Value + " ID: " + Thread.CurrentThread.ManagedThreadId);
                        
                        }

                        Thread.Sleep(rand.Next(40, 1000));
                    }
                });

                //thread.Start();
            }

            //Tasks

            Console.WriteLine("A");

            //await File.WriteAllTextAsync("test.txt", "5");//Task.Delay(1000);

            //int y = await ReadNum();

            Console.WriteLine("B");

            await Experiment();
        }

        private static async Task<int> ReadNum()
        {
            string txt = await File.ReadAllTextAsync("test.txt");
            return int.Parse(txt);
        }

        private static async Task Experiment()
        {
            Console.WriteLine("Start");
            await Task.Delay(1000);
            StreamWriter sw = new("testing.txt");
            await sw.WriteLineAsync("*Libovolné data*");
            await Task.Delay(1000);
            Console.WriteLine("End");
        }
    }
}
