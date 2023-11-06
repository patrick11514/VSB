using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib
{
    public class Stock<T> where T: IProduct
    {
        private List<T> products = new();

        public void Add(T product)
        {
            foreach (T item in products)
            {
                if (ReferenceEquals(item, product))
                {
                    throw new DuplicitProductException(product);
                }
            }

            products.Add(product);
        }

        public List<T> Filter(FilterEvaluator filterEvaluator)
        {
            List<T> newProducts = new();
            foreach (T item in products) {
                if (filterEvaluator.IsMatch(item))
                {
                    newProducts.Add(item);
                }
            }

            return newProducts;
        }
    }
}
