using System.Text.Json.Serialization;

namespace C09;

public class Record
{
    [JsonPropertyName("timepoint")]
    public int timePoint { get; set; }
    [JsonPropertyName("temp2m")]
    public int temp { get; set; }
}