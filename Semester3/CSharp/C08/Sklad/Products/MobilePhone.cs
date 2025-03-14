﻿using StockLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockLib.Products
{
    public class MobilePhone : IPhysicalProduct
    {
        public int Width  { get; set; }

        public int Height  { get; set; }

        public int Length  { get; set; }

        public string Name  { get; set; }

        public double Price  { get; set; }
    }
}
