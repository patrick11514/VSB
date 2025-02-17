using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSMMapLib
{
    public class CycleMapLayer : Layer
    {
        public CycleMapLayer() : base("https://b.tile-cyclosm.openstreetmap.fr/cyclosm/{z}/{x}/{y}.png", 17)
        {

        }
    }
}
