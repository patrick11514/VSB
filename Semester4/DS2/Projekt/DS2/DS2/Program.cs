using Oracle.ManagedDataAccess.Client;
using System.Data;

namespace DS2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            OracleConnectionStringBuilder builder = new();
            builder.DataSource = "dbsys.cs.vsb.cz:1521/oracle.dbsys.cs.vsb.cz";
            builder.UserID = "MIN0150";
            builder.Password = "J1p5WNad4113HJmE";

            using var connection = new OracleConnection(builder.ToString());
            connection.Open();

            //function
            List<int> users = new()
            {
                3
            };


            Console.WriteLine(addGroupToUser(connection, 1, 1, 1, users));

            users[0] = 4;
            Console.WriteLine(addGroupToUser(connection, 1, 2, 1, users));

            Console.WriteLine(addGroupToUser(connection, 1, 5, 1, users));

            users.Add(4);
            users.Add(5);
            users.Add(6);
            //call function from SQL;
            using var command = connection.CreateCommand();
            command.CommandType = System.Data.CommandType.StoredProcedure;
            command.CommandText = "ADDGROUPTOUSER";
            command.Parameters.Add("f_groupId", 3);
            command.Parameters.Add("vf_currentUserId", 2);
            command.Parameters.Add("vf_serverId", 2);

            DataTable table = new();

            DataColumn column = new();
            column.DataType = typeof(int);
            column.ColumnName = "id";
            table.Columns.Add(column);

            foreach (var id in users)
            {
                table.Rows.Add(id);
            }

            command.Parameters.Add("vf_selectedUsers", table);

            var outParam = new OracleParameter("ret", OracleDbType.Boolean);
            outParam.Direction = ParameterDirection.Output;
            command.Parameters.Add(outParam);

            bool result = (bool)command.ExecuteScalar();
            Console.WriteLine(result);
        }

        private static bool addGroupToUser(OracleConnection conn, int f_groupId, int vf_currentUserId, int vf_serverId, List<int> vf_selectedUsers)
        {
            var transaction = conn.BeginTransaction();
            try
            {
                using var command = conn.CreateCommand();
                command.Transaction = transaction;
                command.CommandText = "SELECT COALESCE(MIN(group_id), -1) FROM user_have_group WHERE user_id = :UserId AND server_id = :ServerId";
                command.Parameters.Add("UserId", vf_currentUserId);
                command.Parameters.Add("ServerId", vf_serverId);

                decimal p_group_id = (decimal)command.ExecuteScalar();

                if (p_group_id == -1 || p_group_id < f_groupId)
                {
                    using var command2 = conn.CreateCommand();
                    command2.Transaction = transaction;
                    command2.CommandText = "SELECT COUNT(*) FROM permissions WHERE user_id = :UserId AND server_id = :ServerId AND (name = '*' OR name = 'permission.group.addhigher') AND remove = 0";
                    command2.Parameters.Add("UserId", vf_currentUserId);
                    command2.Parameters.Add("ServerId", vf_serverId);

                    decimal p_perm_count = (decimal)command2.ExecuteScalar();

                    if (p_perm_count == 0)
                    {
                        transaction.Rollback();
                        return false;
                    }
                }

                foreach (var user in vf_selectedUsers)
                {
                    using var command3 = conn.CreateCommand();
                    command3.Transaction = transaction;
                    command3.CommandText = "INSERT INTO user_have_group (group_id, user_id, server_id) VALUES (:GroupId, :UserId, :ServerId)";
                    command3.Parameters.Add("GroupId", f_groupId);
                    command3.Parameters.Add("UserId", user);
                    command3.Parameters.Add("ServerId", vf_serverId);
                }

                transaction.Commit();
                return true;
            }
            catch
            {
                transaction.Rollback();
                return false;
            }
        }
    }
}
