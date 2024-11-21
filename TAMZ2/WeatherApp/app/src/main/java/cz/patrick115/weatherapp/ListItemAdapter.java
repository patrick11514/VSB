package cz.patrick115.weatherapp;

import android.app.Activity;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import org.w3c.dom.Text;

import java.util.List;

public class ListItemAdapter extends RecyclerView.Adapter<ListItemAdapter.ViewHolder> {
    private List<ListItem> itemList;

    public ListItemAdapter(List<ListItem> itemList) {
        this.itemList = itemList;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.single_forecast, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        ListItem item = itemList.get(position);
        holder.date.setText(item.date);
        holder.desc.setText(item.desc);
        holder.temp.setText(item.temp + "°C");
        holder.wind.setText(item.wind + "m/s");
        holder.clouds.setText(item.clouds + "%");
        setWeatherIcon(item.icon, holder.icon);
    }

    private void setWeatherIcon(String iconCode, ImageView imageView) {
        // Build the resource name dynamically
        String resourceName = "icon_" + iconCode;

        // Get the resource ID for the drawable
        int resourceId = imageView.getContext().getResources().getIdentifier(
                resourceName, "drawable", imageView.getContext().getPackageName()
        );

        if (resourceId != 0) {
            // Set the image resource dynamically
            imageView.setImageResource(resourceId);
        }
    }

    @Override
    public int getItemCount() {
        return itemList.size();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        TextView date;
        TextView desc;
        TextView temp;
        TextView wind;
        TextView clouds;
        ImageView icon;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            date = itemView.findViewById(R.id.date);
            desc = itemView.findViewById(R.id.desc);
            temp = itemView.findViewById(R.id.temp);
            wind = itemView.findViewById(R.id.wind);
            clouds = itemView.findViewById(R.id.clouds);
            icon = itemView.findViewById(R.id.icon);
        }
    }
}
