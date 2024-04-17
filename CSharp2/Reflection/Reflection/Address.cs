namespace Reflection;

public class Address
{
    [PrimaryKey]
    public int Id { get; set; }
    public string City { get; set; }
    public string Street { get; set; }
    public int Number { get; set; }
    public string ZipCode { get; set; }
}