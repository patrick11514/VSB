using System.Numerics;

namespace ProductsApp
{
    delegate int MyDelegate(int x, int y);

    delegate int Operation(int x, int y);

    static class MyExt
    {
        public static string JoinNumbers(this IEnumerable<int> nums)
        {
            return string.Join(", ", nums);
        }

        /*public static int[] Odds(this int[] nums)
        {
            return nums.Where((x) => x % 2 == 0).ToArray();
        }*/
        public static IEnumerable<int> Odds(this int[] nums)
        {
            return nums.Where((x) => x % 2 == 0);
        }

        public static IEnumerable<T> MapNumbers<T>(this IEnumerable<T> nums, Map<T> map)
        {
            foreach (T x in nums)
            {
                yield return map(x);
            }
        }
    }

    delegate T Map<T>(T x);

    internal class Program
    {
        static void Main(string[] args)
        {

            //MyDelegate mojePromena = Sum;

            //Console.WriteLine(mojePromena(2, 5));

            /*Calculator calc = new();

            calc.OnSetXY += (object obj, EventArgs args) =>
            {
                Console.WriteLine("Zavolano");
            };

            calc.OnCompute += (object obj, CalcArgs args) =>
            {
                Console.WriteLine($"Vysledek: {args.output}");
            };

            calc.SetXY(2, 10);
            calc.Execute((a, b) => a + b * 2);*/

            //int[] nums = [1, 2, 5, 93, 452, 2, -6];
            //Console.WriteLine(nums.JoinNumbers());
            //Console.WriteLine(nums.MapNumbers(x => x * x).JoinNumbers());
            //Console.WriteLine(nums.Odds().JoinNumbers());

            IEnumerable<Product> products = GetProducts();

            double priceSum = products.Sum(x => x.Price);
            int productCount = products.Count();

            Console.WriteLine(priceSum / productCount);
            Console.WriteLine(products.Average(x => x.Price));

            Console.WriteLine(products.Where(x => x.Quantity > 0).Average(x => x.Price));

            Console.WriteLine(string.Join(",", products.Select(x => x.Name).ToArray()));

            Console.WriteLine(products.Where(x => x.Quantity > 0).First());
            Console.WriteLine(products.First(x => x.Quantity > 0));
            Console.WriteLine(products.FirstOrDefault(x => x.Quantity > 0));

            Console.WriteLine(products.Last());

            Console.WriteLine(products.GroupBy(x => x.Quantity));

        }

        private static void Calc_OnSetXY(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        static int Sum(int x, int y)
        {
            return x + y;
        }

        private static IEnumerable<Product> GetProducts()
        {
            return new List<Product>()
            {
                new Product(){ Id = 1, Name = "Auto", Price = 700_000, Quantity = 10 },
                new Product(){ Id = 1, Name = "Slon", Price = 1_500_000, Quantity = 0 },
                new Product(){ Id = 1, Name = "Kolo", Price = 26_000, Quantity = 5 },
                new Product(){ Id = 1, Name = "Brusle", Price = 2_800, Quantity = 30 },
                new Product(){ Id = 1, Name = "Hodinky", Price = 18_500, Quantity = 2 },
                new Product(){ Id = 1, Name = "Mobil", Price = 24_000, Quantity = 0 }
            };
        }
    }
}
