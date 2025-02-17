namespace C11
{
    internal class Program
    {

        class MyComparer2 : MyComparer
        {
            public override int Compare(int x, int y)
            {
                return -1 * base.Compare(x, y);
            }
        }

        class MyComparer : IComparer<int>
        {
            public virtual int Compare(int x, int y)
            {
                //return y - x;
                if (x == y) return 0;

                if (x % 2 == 0 && y % 2 == 0)
                {
                    return x - y;
                }

                if (Math.Abs(x) % 2 == 1 && Math.Abs(y) % 2 == 1)
                {
                    return x - y;
                }

                if (x % 2 == 0)
                {
                    return -1;
                }

                return 1;
            }
        }

        static void Main(string[] args)
        {
            MyList a = new();
            a.Add(1);
            a.Add(2);
            a.Add(3);
            a.Add(4);
            a.Add(5);
            a.Add(6);

            foreach (int x in a.Multiply(10))
            {
                Console.WriteLine(x);
            }

            int[] arr = new int[] { 3, 9, 0, -5, 73, 8, -2, -3 };
            List<int> list = new(arr);

            Array.Sort(arr, new MyComparer());
            list.Sort(new MyComparer2());

            Console.WriteLine(string.Join(", ", arr));
            Console.WriteLine(string.Join(", ", list));



            Translator t = new();
            t.Add("nový", "new");
            t.Add("rok", "year");
            t.Add("Šťastný", "Happy");

            Console.WriteLine(t.Translate("šťastný nový rok"));
        }
    }
}