using C08.Models;

namespace C08
{
    public class ProductsService
    {
        public IEnumerable<Product> List()
        {
            return Product.GetProducts();
        }

        public Product Find(int id)
        {
            return Product.GetProducts().Find((product) => product.Id == id);
        }
    }
}
