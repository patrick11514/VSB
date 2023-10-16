using MapRendererLib;

namespace OSMMapLib
{
    public class Map
    {
        public Layer? Layer { get; set; }

        private double lat { get; set; }
        public double Lat {
            get {
                return lat;
            }
            set
            {
                if (value < -90)
                {
                    lat = -180 - value;
                } else if (value > 90)
                {
                    lat = 180 - value;
                } else
                {
                    lat = value;
                }
            }
        }

        private double lon {get; set;}
        public double Lon { 
            get {
                return lon;
            } 
            set {
                if (value < -180)
                {
                    lon = -360 - value;
                }
                else if (value > 180)
                {
                    lon = 360 - value;
                }
                else
                {
                    lon = value;
                }
            } 
        }

        private int zoom { get; set;}
        public int Zoom { 
            get {
                if (zoom > Layer.MaxZoom)
                {
                    return Layer.MaxZoom;
                }
                return zoom;
            }
            set {
                zoom = value < 1 ? 1 : value;
            } 
        }

        private int CenterTileX { 
            get
            {
                return (int)(
                    (lon + 180.0) / 360.0 * (1 << Zoom)
                    );
            } 
        }

        private int CenterTileY
        {
            get
            {
                return (int)(
                    (1.0 - Math.Log(
                        Math.Tan(lat * Math.PI / 180.0) + 1.0 
                        / Math.Cos(lat * Math.PI / 180.0)
                        ) / Math.PI) 
                        / 2.0 * 
                        (1 << Zoom));
            }
        }

        public void Render(string fileName)
        {
            MapRenderer mapRenderer = new MapRenderer(4, 4);
            for (int x = -2; x < 2; x++)
            {
                for (int y = -2; y < 2; y++)
                {
                    Tile tile = this.Layer[this.CenterTileX + x, this.CenterTileY + y, this.Zoom];

                    Console.WriteLine(tile);

                    mapRenderer.Set(x + 2, y + 2, tile.Url);
                }
            }
            mapRenderer.Flush();
            mapRenderer.Render(fileName);
        }

    }
}
