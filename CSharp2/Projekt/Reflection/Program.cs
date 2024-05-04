namespace DataLayer;

class Program
{
	public
	static void Main(string[] args)
	{


		/*dapper.GetType<string>();
        dapper.GetType<int>();
        dapper.GetType<double>();
        dapper.GetType<float>();
        dapper.GetType<decimal>();
        dapper.GetType<long>();
        dapper.GetType<short>();
        dapper.GetType<byte>();
        dapper.GetType<char>();
        dapper.GetType<uint?>();
        dapper.GetType<ulong>();
        dapper.GetType<ushort>();
        dapper.GetType<sbyte>();
        dapper.GetType<bool>();
        dapper.GetType<DateTime>();
        dapper.GetType<TimeSpan>();*/

		//dapper.CreateTable<Address>();
		//dapper.Insert(new Address(){Id = 1, City = "Petrov", Date = new DateTime(2003,5,1,6,30,12), Number = 69,Street = "Česká", ZipCode = "123 45"});
		//dapper.Delete(new Address(){Id = 1, City = "Petrov", Date = new DateTime(2003,5,1,6,30,12), Number = 69,Street = "Česká", ZipCode = "123 45"});

		/*var i = dapper.Select<Address>(1);
        Console.WriteLine(i.Id);
        Console.WriteLine(i.City);
        Console.WriteLine(i.Street);
        Console.WriteLine(i.Number);
        Console.WriteLine(i.ZipCode);
        Console.WriteLine(i.Date);

        i.City = "PEPA";

        if (!dapper.Update(i))
        {
            Console.WriteLine("FAILED");
        }
        
        var b = dapper.Select<Address>(1);
        
        Console.WriteLine(b.Id);
        Console.WriteLine(b.City);
        Console.WriteLine(b.Street);
        Console.WriteLine(b.Number);
        Console.WriteLine(b.ZipCode);
        Console.WriteLine(b.Date);*/
		//dapper.Insert(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
		//dapper.SelectAll(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
		//dapper.SelectAll<ExampleClass>();

		//dapper.Update(new ExampleClass() { Bool = true, Byte = 1, Char = 'c'});
	}
}