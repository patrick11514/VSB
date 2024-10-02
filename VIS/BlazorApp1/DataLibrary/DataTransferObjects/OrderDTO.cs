namespace DataLibrary.DataTransferObjects;

public class OrderDTO
{
    public int Id { get; set; }
    public AddressDTO BillingAddress { get; set; }
    public AddressDTO ShippingAddress { get; set; }
    public CustomerDTO Customer { get; set; }
    public List<ItemDTO> Items { get; set; }
}