using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Net;

namespace MapRendererLib
{
    public class MapRenderer : IDisposable
    {
        private Dictionary<string, MapDrawerTile> data = new Dictionary<string, MapDrawerTile>();

        private static readonly WebClient client = new WebClient();



        private const int TILE_SIZE = 256;
        private readonly int width;
        private readonly int height;
        private Bitmap bmp;

        public MapRenderer(int width, int height) {
            this.width = width;
            this.height = height;

        }

        public void Set(int x, int y, string url, float opacity = 1)
        {
            if(opacity < 0) { 
                opacity = 0; 
            }
            else if (opacity > 1) 
            { 
                opacity = 1; 
            }
            if (x < 0 || y < 0 || x >= this.width || y >= this.height)
            {
                throw new Exception("MapDrawer index out of bounds!");
            }

            if (string.IsNullOrWhiteSpace(url))
            {
                throw new ArgumentNullException(nameof(url));
            }

            data[$"{x}:{y}"] = new MapDrawerTile(x * TILE_SIZE, y * TILE_SIZE, url, opacity);
        }

        public void Clear()
        {
            this.data.Clear();
        }



        public void Flush()
        {
            if (bmp == null)
            {
                bmp = new Bitmap(this.width * TILE_SIZE, this.height * TILE_SIZE);
                using (Graphics g = Graphics.FromImage(bmp))
                {
                    g.FillRectangle(new SolidBrush(Color.Gray), 0, 0, this.width * TILE_SIZE, this.height * TILE_SIZE);
                }
            }

            if (this.data.Count == 0)
            {
                return;
            }

            using (Graphics g = Graphics.FromImage(bmp))
            {
                foreach (var tile in this.data.Values)
                {
                    using (Stream ms = DownloadTileOrResolveFromCache(tile.Url))
                    {
                        using (Image tileImg = Image.FromStream(ms))
                        {
                            ColorMatrix matrix = new ColorMatrix();
                            matrix.Matrix33 = tile.Opacity;
                            ImageAttributes attributes = new ImageAttributes();
                            attributes.SetColorMatrix(matrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);


                            g.DrawImage(tileImg, new Rectangle(tile.X, tile.Y, tileImg.Width, tileImg.Height), 0, 0, tileImg.Width, tileImg.Height, GraphicsUnit.Pixel, attributes);
                        }
                    }

                }
            }

            this.Clear();
        }



        public void Render(string path = "map.png")
        {
            this.Flush();

            bmp?.Save(path, ImageFormat.Png);
            bmp?.Dispose();
            bmp = null;
        }


        private Stream DownloadTileOrResolveFromCache(string url)
        {
            DirectoryInfo di = new DirectoryInfo("./cache");
            if (!di.Exists)
            {
                di.Create();
            }
            FileInfo fi = new FileInfo(Path.Combine(di.FullName, url.Replace('/', '_').Replace(':', '_')));
            if (fi.Exists)
            {
                return fi.OpenRead();
            }

            Stream result = DownloadTile(url);

            using (FileStream cache = fi.OpenWrite())
            {
                long pos = result.Position;
                result.CopyTo(cache);
                result.Seek(pos, SeekOrigin.Begin);
            }

            return result;
        }


        private Stream DownloadTile(string url)
        {
            client.Headers.Add("User-Agent", "VSB - CSharp");
            byte[] data = client.DownloadData(url);
            return new MemoryStream(data);
        }

        public void Dispose()
        {
            this.bmp?.Dispose();
        }

        class MapDrawerTile
        {
            public int X { get; set; }
            public int Y { get; set; }
            public string Url { get; set; }
            public float Opacity { get; set; }

            public MapDrawerTile(int x, int y, string url, float opacity)
            {
                this.X = x;
                this.Y = y;
                this.Url = url;
                this.Opacity = opacity;
            }
        }
    }
}
