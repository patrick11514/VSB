using Dapper;
using Microsoft.Data.Sqlite;

namespace _2C04
{
    [Table("Customer")]
    class Customer
    {
        [Key]
        public int Id { get; set; }

        [Column("Name")]
        public string Name { get; set; }
        public string Address { get; set; }
    }
    internal class Program
    {
        static void Main(string[] args)
        {
            SimpleCRUD.SetDialect(SimpleCRUD.Dialect.SQLite);
            string connectionString = "Data Source=mydb.db;";
            using SqliteConnection connection = new(connectionString);

            //connection.Open();

            /*using SqliteCommand cmd = connection.CreateCommand();
            cmd.CommandText = File.ReadAllText("database-create.sql");
            cmd.ExecuteNonQuery();*/

            /*using SqliteCommand command = connection.CreateCommand();
            command.CommandText = "INSERT INTO Customer (Name, Address) VALUES (@Name, @Address)";
            command.Parameters.AddWithValue("Name", "Karlos Amigos");
            command.Parameters.AddWithValue("Address", "Levá Pravá 15/4");

            command.ExecuteNonQuery();*/

            /*SqliteCommand command = connection.CreateCommand();
            command.CommandText = "SELECT COUNT(*) FROM Customer";
            long count = (long)command.ExecuteScalar();

            Console.WriteLine(count);

            using SqliteCommand select = connection.CreateCommand();
            select.CommandText = "SELECT * FROM Customer";
            using SqliteDataReader reader = select.ExecuteReader();
            
            while (reader.Read())
            {
                int id = reader.GetInt32(reader.GetOrdinal("Id"));
                String name = reader.GetString(reader.GetOrdinal("Name"));
                String? address = null;
                if (!reader.IsDBNull(reader.GetOrdinal("Address")))
                {
                    address = reader.GetString(reader.GetOrdinal("Address"));
                }
                Console.WriteLine(id + " | " + name + " | " + address);
            }

            /*using SqliteCommand aa = connection.CreateCommand();
            aa.CommandText = "INSERT INTO [Order] (CustomerId, Product, Price) VALUES (1, 'Platina v lolku', 666), (2, 'Iron v lolku', 7777)";
            aa.ExecuteNonQuery();*/

            /*using SqliteTransaction transaction = connection.BeginTransaction();
            for (int i = 0; i < 10; i++)
            {
                using SqliteCommand cmd = connection.CreateCommand();
                cmd.Transaction = transaction;
                cmd.CommandText = "INSERT INTO [Order] (CustomerId, Product, Price) VALUES (@CustomerId, @Product, @Price)";
                cmd.Parameters.AddWithValue("CustomerId", 1);
                cmd.Parameters.AddWithValue("Product", "Auticko");
                cmd.Parameters.AddWithValue("Price", 1451 * i);
            }

            transaction.Commit();*/

            //anonymní objekt: new {Name = "Petr", Address = "Praha"}

            /*connection.Execute("INSERT INTO Customer (Name, Address) Values (@Name, @Address)", new Customer() { 
                Name = "Petr",
                Address = "Praha"
            });

            long count = connection.ExecuteScalar<long>("SELECT COUNT(*) FROM Customer");
            Console.WriteLine(count);

            IEnumerable<Customer> customers = connection.Query<Customer>("SELECT * FROM Customer");

            foreach (Customer customer in customers)
            {
                Console.WriteLine(customer.Id + " | " + customer.Name + " | " + customer.Address);
            }*/

            int? id = connection.Insert(new Customer()
            {
                Name = "Petr",
                Address = "Praha"
            });

            connection.Open();

            SqliteTransaction transaction = connection.BeginTransaction();
            Customer c = connection.Get<Customer>(id, transaction);
            c.Address = "Brno";

            connection.Update(c, transaction);

            transaction.Commit();

        }
    }
}
