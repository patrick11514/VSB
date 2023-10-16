using OSMMapLib;

namespace OSMMap
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Layer layer = new(maxZoom: 16);

            Map map = new();
            map.Layer = layer;
            map.Lat = 49.83185824851034;
            map.Lon = 18.16025164421896;
            map.Zoom = 13;

            map.Render("mapa.png");

            CycleMapLayer layer2 = new();

            map.Layer = layer2;
            map.Zoom = 14;
            map.Render("mapa2.png");
        }
    }
}