using StockLib;
using StockLib.Filters;
using StockLib.Products;

namespace C08
{
    internal class Program
    {
        static void Main(string[] args)
        {
            MobilePhone mobilePhone = new()
            {
                Height = 50,
                Length = 20,
                Width = 30,
                Name = "Nokia 3310",
                Price = 1000
            };

            Voucher voucher = new()
            {
                Name = "Vstup zdarma",
                Price = 150
            };

            Bike bike = new()
            {
                Height = 50,
                Length = 40,
                Width = 20,
                Name = "Kolo",
                Price = 8460,
                WheelCount = 2
            };

            Stock<IProduct> sklad = new();

            PriceFilter priceFilter = new() { 
                MinPrice = 800
            };

            WheelCountFilter wheelCountFilter = new()
            {
                WheelCount = 2
            };

            WidthFilter widthFilter = new() 
            {
                MinWidth = 10
            };

            FilterEvaluator filter = new();
            filter.Add(widthFilter);
            filter.Add(priceFilter);
            filter.Add(wheelCountFilter);
            

            try
            {
                sklad.Add(mobilePhone);
                sklad.Add(voucher);
                sklad.Add(bike);

                List<IProduct> produkty = sklad.Filter(filter);

                foreach (IProduct product in produkty)
                {
                    Console.WriteLine(product.Name);
                }
 
                sklad.Add(bike);
            }
            catch(DuplicitProductException ex)
            {
                Console.WriteLine("Pridavani duplicitniho produktu: " + ex.Product.Name);
            }
            catch (InvalidFilterArgumentException ex)
            {
                Console.WriteLine("Spatny parametry filteru: " + ex.Message);
            }
        }
    }
}