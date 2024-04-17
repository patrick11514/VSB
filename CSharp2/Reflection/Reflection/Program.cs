namespace Reflection;

class Program
{
    static void Main(string[] args)
    {
        var dapper = new MyDapper();
        //dapper.SelectAll(new Address() { City = null, Number = 459, ZipCode = "42000", Street = "Zlatá"});
        dapper.SelectAll(new ExampleClass() { Bool = true, Byte = 1, Char = 'c'});
    }
}