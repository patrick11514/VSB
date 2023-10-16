using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSMMapLib
{
    public class Tile
    {
        public int X { get; private set; }
        public int Y { get; private set; }

        private int zoom { get; set; }
        public int Zoom { get {
                return zoom;
            }
            private set
            {
                if (value < 1)
                {
                    zoom = 1;
                }
                else
                {
                    zoom = value;
                }
            } 
        }

        public string Url { get; private set; }

        public Tile(int x, int y, int zoom, string url)
        {
            X = x;
            Y = y;
            Zoom = zoom;
            Url = url;
        }

        public override string ToString()
        {
            StringBuilder sb = new();
            //sb.AppendFormat
            sb.Append('[');
            sb.Append(X);
            sb.Append(", ");
            sb.Append(Y);
            sb.Append(", ");
            sb.Append(Zoom);
            sb.Append("]: ");
            sb.Append(Url);

            return sb.ToString();
        }

    }
}
