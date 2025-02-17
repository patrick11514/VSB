using C08.Models;

namespace C08
{
	public class CartService
	{
		private List<Product> products = new();
		public void Add(Product product)
		{
			products.Add(product);
		}

		public int Count { get { return products.Count; } }

		public List<Product> GetProducts() { return products; }
	}
}
