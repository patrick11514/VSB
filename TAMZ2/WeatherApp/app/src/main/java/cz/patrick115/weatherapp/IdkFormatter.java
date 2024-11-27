package cz.patrick115.weatherapp;

import com.github.mikephil.charting.formatter.ValueFormatter;

import java.util.List;

public class IdkFormatter extends ValueFormatter {
    private final List<String> times;

    public IdkFormatter(List<String> times) {
        this.times = times;
    }

    @Override
    public String getFormattedValue(float value) {
        int index = (int) value;
        if (index >= 0 && index < times.size()) {
            return times.get(index); // Return the corresponding time
        }
        return ""; // Default for out-of-bounds values
    }
}