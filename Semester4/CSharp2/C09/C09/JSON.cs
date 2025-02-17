using System.Text.Json.Serialization;

namespace C09;

public class JSON
{
    [JsonPropertyName("init")]
    public string init { get; set; }
    [JsonPropertyName("dataseries")]
    public List<Record> dataSeries { get; set; }
}