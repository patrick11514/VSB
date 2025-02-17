package cz.patrick115.soko2;

import android.app.Activity;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;

public class ListItemAdapter extends RecyclerView.Adapter<ListItemAdapter.ViewHolder> {
    private List<ListItem> itemList;

    public ListItemAdapter(List<ListItem> itemList) {
        this.itemList = itemList;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        ListItem item = itemList.get(position);
        holder.sokoView.bind(item.getLevel());
        holder.level.setText("Level: " + item.getLevel().getId());
        holder.score.setText("Skóre: " + item.getLevel().getScore());

        holder.view.setOnClickListener(v -> {
            Intent intend = new Intent(holder.view.getContext(), CurrentLevel.class);
            intend.putExtra("id", item.getLevel().getId());
            ((Activity) holder.view.getContext()).startActivityForResult(intend, 1);
        });
    }

    @Override
    public int getItemCount() {
        return itemList.size();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        SokoView sokoView;
        TextView level;
        TextView score;
        View view;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            sokoView = itemView.findViewById(R.id.sokoView);
            level = itemView.findViewById(R.id.level);
            score = itemView.findViewById(R.id.score);
            view = itemView;
        }
    }
}