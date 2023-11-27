using System;

namespace Tutorial10.Commons
{
    public abstract class CustomerHelper
    {
        public static void Print<T>(ICustomer<T> customer) where T : IOrder
        {
            Console.WriteLine("------");

            Console.WriteLine("Customer:");
            Console.WriteLine($"\tName: {customer.Name}");
            Console.WriteLine($"\tAge: {customer.Age}");
            Console.WriteLine($"\tEmail: {customer.Email}");
            Console.WriteLine($"\tWeight: {customer.Weight}");
            Console.WriteLine($"\tIsAlive: {customer.IsAlive}");


            Console.WriteLine("Orders:");
            foreach (T order in customer.Orders)
            {
                Console.WriteLine($"\tName: {order.Name}");
                Console.WriteLine($"\tAge: {order.Price}");
                Console.WriteLine();
            }
            Console.WriteLine();
            Console.WriteLine();
        }
    }
}
