using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSMMapLib
{
    public class Layer
    {
        public string UrlTemplate { get; private set; }
        public int MaxZoom { get; private set; }

        public Tile this[int x, int y, int zoom]
        {
            get
            {
                return new Tile(x, y ,zoom, FormatUrl(x, y, zoom));
            }
        }

        public Layer(string urlTemplate = "https://{c}.tile.openstreetmap.org/{z}/{x}/{y}.png", int maxZoom = 10)
        {
            UrlTemplate = urlTemplate;
            MaxZoom = maxZoom;
            
        }

        public string FormatUrl(int x, int y, int zoom)
        {
            Random r = new();
            char c = (char)r.Next('a', 'd');

            return UrlTemplate
                .Replace("{x}", x.ToString())
                .Replace("{y}", y.ToString())
                .Replace("{z}", zoom.ToString())
                .Replace("{c}", c.ToString());
        }

    }
}
