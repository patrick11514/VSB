namespace Reflection;

class Program
{
    static void Main(string[] args)
    {
        using var dapper = new MyDapper("db.sqlite");
        dapper.CreateTable<Address>();
        //dapper.Insert(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        //dapper.SelectAll<ExampleClass>();
        
        //dapper.Update(new ExampleClass() { Bool = true, Byte = 1, Char = 'c'});
    }
}