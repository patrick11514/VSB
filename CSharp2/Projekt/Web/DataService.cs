using DataLayer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Web
{
    public class DataService
    {
        public static MyDapper dapper;
        public MyDapper GetDapper()
        {
            return dapper;
        }
    }
}
