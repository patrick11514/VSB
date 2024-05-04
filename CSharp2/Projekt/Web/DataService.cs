using DataLayer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Web
{
	internal class DataService
	{
		public MyDapper GetDapper()
		{
			return Models.DataLayer.dapper;
		}
	}
}
