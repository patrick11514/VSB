﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataLayer.Structures
{
    public class Student
    {
        [PrimaryKey]
        public int Id { get; set; }
        public string Name { get; set; }
        public string Surname { get; set; }
        public string State { get; set; }
        public string City { get; set; }
        public string Street { get; set; }
        public string Region { get; set; }
        public int HouseNumber { get; set; }
        public int PostalCode { get; set; }
        public string Phone { get; set; }
        public string Email { get; set; }

    }
}