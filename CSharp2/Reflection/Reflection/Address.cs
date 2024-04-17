namespace Reflection;

public class Address
{
    [PrimaryKey]
    [Type(Type.INTEGER)]
    public int Id { get; set; }
    
    [Type(Type.TEXT, true)]
    public string City { get; set; }
    [Type(Type.TEXT, true)]
    public string Street { get; set; }
    [Type(Type.INTEGER)]
    public int Number { get; set; }
    [Type(Type.TEXT, true)]
    public string ZipCode { get; set; }
}