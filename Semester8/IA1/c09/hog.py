#!/usr/bin/env python3
"""Histogram of Oriented Gradients demo for IA1 c09.

The script computes a simple HOG descriptor for a single image and saves a
visualization of the cell histograms over the image.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Compute and visualize HOG features.")
    parser.add_argument(
        "--image",
        type=Path,
        default=Path("hog_test.png"),
        help="Input image path (default: hog_test.png)",
    )
    parser.add_argument(
        "--cell-size",
        type=int,
        default=8,
        help="Cell size in pixels.",
    )
    parser.add_argument(
        "--block-size",
        type=int,
        default=2,
        help="Block size in cells.",
    )
    parser.add_argument(
        "--bins",
        type=int,
        default=9,
        help="Number of orientation bins in [0, 180).",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("hog_output.png"),
        help="Output visualization path.",
    )
    parser.add_argument(
        "--no-show",
        action="store_true",
        help="Do not open a window after saving the visualization.",
    )
    parser.add_argument(
        "--descriptor-only",
        action="store_true",
        help="Skip visualization display and only print descriptor statistics.",
    )
    return parser.parse_args()


def load_image(path: Path) -> np.ndarray:
    if not path.exists():
        raise FileNotFoundError(f"Image not found: {path}")

    image = Image.open(path).convert("L")
    return np.asarray(image, dtype=np.float32) / 255.0


def compute_gradients(image: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    padded = np.pad(image, 1, mode="edge")
    gx = padded[1:-1, 2:] - padded[1:-1, :-2]
    gy = padded[2:, 1:-1] - padded[:-2, 1:-1]
    magnitude = np.hypot(gx, gy)
    orientation = (np.degrees(np.arctan2(gy, gx)) + 180.0) % 180.0
    return magnitude, orientation


def compute_cell_histograms(
    magnitude: np.ndarray,
    orientation: np.ndarray,
    cell_size: int,
    bins: int,
) -> np.ndarray:
    height, width = magnitude.shape
    cells_y = height // cell_size
    cells_x = width // cell_size

    trimmed_mag = magnitude[: cells_y * cell_size, : cells_x * cell_size]
    trimmed_ori = orientation[: cells_y * cell_size, : cells_x * cell_size]

    cell_histograms = np.zeros((cells_y, cells_x, bins), dtype=np.float32)
    bin_width = 180.0 / bins

    for cell_y in range(cells_y):
        row_start = cell_y * cell_size
        row_end = row_start + cell_size
        for cell_x in range(cells_x):
            col_start = cell_x * cell_size
            col_end = col_start + cell_size

            cell_mag = trimmed_mag[row_start:row_end, col_start:col_end]
            cell_ori = trimmed_ori[row_start:row_end, col_start:col_end]

            bin_float = cell_ori / bin_width
            lower_bin = np.floor(bin_float).astype(int) % bins
            upper_bin = (lower_bin + 1) % bins
            upper_weight = bin_float - np.floor(bin_float)
            lower_weight = 1.0 - upper_weight

            hist = np.zeros(bins, dtype=np.float32)
            np.add.at(hist, lower_bin.ravel(), (cell_mag * lower_weight).ravel())
            np.add.at(hist, upper_bin.ravel(), (cell_mag * upper_weight).ravel())
            cell_histograms[cell_y, cell_x] = hist

    return cell_histograms


def normalize_blocks(cell_histograms: np.ndarray, block_size: int) -> np.ndarray:
    cells_y, cells_x, bins = cell_histograms.shape
    blocks_y = cells_y - block_size + 1
    blocks_x = cells_x - block_size + 1

    if blocks_y <= 0 or blocks_x <= 0:
        raise ValueError(
            "Image is too small for the requested cell/block configuration."
        )

    descriptor_blocks = []
    for block_y in range(blocks_y):
        for block_x in range(blocks_x):
            block = cell_histograms[
                block_y : block_y + block_size,
                block_x : block_x + block_size,
            ].ravel()
            norm = np.linalg.norm(block) + 1e-6
            block = block / norm
            block = np.clip(block, 0.0, 0.2)
            block = block / (np.linalg.norm(block) + 1e-6)
            descriptor_blocks.append(block)

    return np.concatenate(descriptor_blocks).astype(np.float32)


def build_hog_descriptor(
    image: np.ndarray,
    cell_size: int,
    block_size: int,
    bins: int,
) -> tuple[np.ndarray, np.ndarray]:
    magnitude, orientation = compute_gradients(image)
    cell_histograms = compute_cell_histograms(magnitude, orientation, cell_size, bins)
    descriptor = normalize_blocks(cell_histograms, block_size)
    return descriptor, cell_histograms


def draw_hog_overlay(
    image: np.ndarray,
    cell_histograms: np.ndarray,
    cell_size: int,
    output_path: Path,
) -> None:
    cells_y, cells_x, bins = cell_histograms.shape
    height = cells_y * cell_size
    width = cells_x * cell_size
    base = image[:height, :width]

    fig, ax = plt.subplots(figsize=(10, 10))
    ax.imshow(base, cmap="gray", interpolation="nearest")

    bin_width = 180.0 / bins
    angles = np.deg2rad(np.arange(bins) * bin_width + bin_width / 2.0)
    center_offset = cell_size / 2.0

    for cell_y in range(cells_y):
        for cell_x in range(cells_x):
            hist = cell_histograms[cell_y, cell_x]
            if not np.any(hist):
                continue

            x_center = cell_x * cell_size + center_offset
            y_center = cell_y * cell_size + center_offset

            scale = 0.45 * cell_size / (hist.max() + 1e-6)
            for value, angle in zip(hist, angles, strict=False):
                if value <= 0:
                    continue

                length = value * scale
                dx = np.cos(angle) * length
                dy = np.sin(angle) * length
                ax.plot(
                    [x_center - dx, x_center + dx],
                    [y_center - dy, y_center + dy],
                    color="tab:red",
                    linewidth=1.0,
                    alpha=0.9,
                )

    ax.set_xlim(0, width)
    ax.set_ylim(height, 0)
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_title("HOG visualization", fontsize=14)
    fig.tight_layout(pad=0.2)
    fig.savefig(output_path, dpi=200, bbox_inches="tight")
    plt.close(fig)


def main() -> int:
    args = parse_args()

    try:
        image = load_image(args.image)
    except FileNotFoundError as exc:
        print(f"[ERROR] {exc}")
        return 1

    try:
        descriptor, cell_histograms = build_hog_descriptor(
            image=image,
            cell_size=args.cell_size,
            block_size=args.block_size,
            bins=args.bins,
        )
    except ValueError as exc:
        print(f"[ERROR] {exc}")
        return 1

    print(f"[INFO] image: {args.image}")
    print(f"[INFO] descriptor shape: {descriptor.shape}")
    print(f"[INFO] descriptor length: {descriptor.size}")
    print(f"[INFO] descriptor norm: {np.linalg.norm(descriptor):.6f}")

    if not args.descriptor_only:
        draw_hog_overlay(image, cell_histograms, args.cell_size, args.output)
        print(f"[INFO] saved visualization: {args.output}")

        if not args.no_show:
            preview = Image.open(args.output)
            preview.show()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())