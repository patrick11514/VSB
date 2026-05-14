#!/usr/bin/env python3
"""SLIC superpixel segmentation for the c10 bear image."""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path

import cv2
import matplotlib.pyplot as plt
import numpy as np


@dataclass
class SLICParams:
    segments: int
    compactness: float
    max_iter: int
    convergence: float


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Run SLIC superpixel segmentation on slic_bears.jpg."
    )
    parser.add_argument(
        "--image",
        type=Path,
        default=Path("slic_bears.jpg"),
        help="Input image path (default: slic_bears.jpg)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("slic_bears_slic.png"),
        help="Output visualization path.",
    )
    parser.add_argument(
        "--segmented-output",
        type=Path,
        default=None,
        help="Output path for the mean-color segmented image (default: derived from --output).",
    )
    parser.add_argument(
        "--segments",
        type=int,
        default=180,
        help="Desired number of superpixels.",
    )
    parser.add_argument(
        "--compactness",
        type=float,
        default=15.0,
        help="Balance between color and spatial distance.",
    )
    parser.add_argument(
        "--max-iter",
        type=int,
        default=10,
        help="Maximum number of SLIC refinement iterations.",
    )
    parser.add_argument(
        "--convergence",
        type=float,
        default=1.0,
        help="Stop when the average center shift falls below this value.",
    )
    parser.add_argument(
        "--show",
        action="store_true",
        help="Display the result after saving it.",
    )
    return parser.parse_args()


def load_image(path: Path) -> np.ndarray:
    if not path.exists():
        raise FileNotFoundError(f"Image not found: {path}")

    image_bgr = cv2.imread(str(path), cv2.IMREAD_COLOR)
    if image_bgr is None:
        raise ValueError(f"Unable to read image: {path}")

    return image_bgr


def to_lab_float(image_bgr: np.ndarray) -> np.ndarray:
    image_lab = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2LAB)
    return image_lab.astype(np.float32)


def compute_gradient_magnitude(image_bgr: np.ndarray) -> np.ndarray:
    gray = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2GRAY)
    gray = gray.astype(np.float32)
    grad_x = cv2.Sobel(gray, cv2.CV_32F, 1, 0, ksize=3)
    grad_y = cv2.Sobel(gray, cv2.CV_32F, 0, 1, ksize=3)
    return cv2.magnitude(grad_x, grad_y)


def initialize_centers(image_lab: np.ndarray, segments: int) -> tuple[np.ndarray, float]:
    height, width, _ = image_lab.shape
    pixel_count = height * width
    step = math.sqrt(pixel_count / float(max(segments, 1)))
    step = max(1.0, step)

    gradient = compute_gradient_magnitude(cv2.cvtColor(image_lab.astype(np.uint8), cv2.COLOR_LAB2BGR))

    centers: list[np.ndarray] = []
    y_positions = np.arange(step / 2.0, height, step)
    x_positions = np.arange(step / 2.0, width, step)

    for y in y_positions:
        for x in x_positions:
            yi = int(round(y))
            xi = int(round(x))
            y0 = max(1, yi - 1)
            y1 = min(height - 1, yi + 1)
            x0 = max(1, xi - 1)
            x1 = min(width - 1, xi + 1)

            window = gradient[y0 : y1 + 1, x0 : x1 + 1]
            min_idx = int(np.argmin(window))
            wy, wx = np.unravel_index(min_idx, window.shape)
            cy = y0 + wy
            cx = x0 + wx

            centers.append(np.array([*image_lab[cy, cx], float(cx), float(cy)], dtype=np.float32))

    if not centers:
        raise ValueError("Unable to initialize SLIC centers for the given image.")

    return np.stack(centers, axis=0), step


def build_distance_grid(
    centers: np.ndarray,
    image_lab: np.ndarray,
    step: float,
    compactness: float,
) -> tuple[np.ndarray, np.ndarray]:
    height, width, _ = image_lab.shape
    labels = np.full((height, width), -1, dtype=np.int32)
    distances = np.full((height, width), np.inf, dtype=np.float32)

    for center_idx, center in enumerate(centers):
        l, a, b, cx, cy = center
        x0 = max(0, int(cx - 2.0 * step))
        x1 = min(width, int(cx + 2.0 * step) + 1)
        y0 = max(0, int(cy - 2.0 * step))
        y1 = min(height, int(cy + 2.0 * step) + 1)

        region = image_lab[y0:y1, x0:x1]
        yy, xx = np.mgrid[y0:y1, x0:x1]

        color_distance = np.sqrt(
            (region[:, :, 0] - l) ** 2
            + (region[:, :, 1] - a) ** 2
            + (region[:, :, 2] - b) ** 2
        )
        spatial_distance = np.sqrt((xx - cx) ** 2 + (yy - cy) ** 2)
        distance = color_distance + (compactness / step) * spatial_distance

        current = distances[y0:y1, x0:x1]
        update_mask = distance < current
        current[update_mask] = distance[update_mask]
        distances[y0:y1, x0:x1] = current
        labels[y0:y1, x0:x1][update_mask] = center_idx

    return labels, distances


def recompute_centers(image_lab: np.ndarray, labels: np.ndarray, num_centers: int) -> tuple[np.ndarray, np.ndarray]:
    height, width, _ = image_lab.shape
    centers = np.zeros((num_centers, 5), dtype=np.float32)
    counts = np.zeros(num_centers, dtype=np.int32)

    ys, xs = np.indices((height, width))
    flat_labels = labels.ravel()
    flat_pixels = image_lab.reshape(-1, 3)
    flat_xs = xs.ravel().astype(np.float32)
    flat_ys = ys.ravel().astype(np.float32)

    for label_idx in range(num_centers):
        mask = flat_labels == label_idx
        if not np.any(mask):
            continue

        selected_pixels = flat_pixels[mask]
        selected_x = flat_xs[mask]
        selected_y = flat_ys[mask]

        centers[label_idx, :3] = selected_pixels.mean(axis=0)
        centers[label_idx, 3] = selected_x.mean()
        centers[label_idx, 4] = selected_y.mean()
        counts[label_idx] = int(mask.sum())

    return centers, counts


def slic_segment(image_bgr: np.ndarray, params: SLICParams) -> tuple[np.ndarray, np.ndarray, int]:
    image_lab = to_lab_float(image_bgr)
    centers, step = initialize_centers(image_lab, params.segments)

    previous_centers = centers.copy()
    labels = np.full(image_lab.shape[:2], -1, dtype=np.int32)

    for _ in range(params.max_iter):
        labels, _ = build_distance_grid(centers, image_lab, step, params.compactness)
        centers, counts = recompute_centers(image_lab, labels, len(centers))

        valid = counts > 0
        if not np.any(valid):
            break

        movement = np.linalg.norm(centers[valid] - previous_centers[valid], axis=1)
        average_shift = float(np.mean(movement)) if movement.size else 0.0
        previous_centers = centers.copy()
        if average_shift < params.convergence:
            break

    return labels, centers, int(round(step))


def create_boundary_overlay(image_bgr: np.ndarray, labels: np.ndarray) -> np.ndarray:
    overlay = image_bgr.copy()
    boundary = np.zeros(labels.shape, dtype=bool)
    boundary[:-1, :] |= labels[:-1, :] != labels[1:, :]
    boundary[:, :-1] |= labels[:, :-1] != labels[:, 1:]

    overlay[boundary] = (0, 0, 255)
    return overlay


def create_segmented_image(image_bgr: np.ndarray, labels: np.ndarray) -> np.ndarray:
    segmented = np.zeros_like(image_bgr)
    for label_idx in np.unique(labels):
        mask = labels == label_idx
        if not np.any(mask):
            continue

        mean_color = image_bgr[mask].mean(axis=0)
        segmented[mask] = np.clip(mean_color, 0, 255).astype(np.uint8)

    return segmented


def save_overlay(image_bgr: np.ndarray, labels: np.ndarray, output: Path) -> None:
    overlay = create_boundary_overlay(image_bgr, labels)
    output.parent.mkdir(parents=True, exist_ok=True)
    if not cv2.imwrite(str(output), overlay):
        raise OSError(f"Unable to write output image: {output}")


def save_segmented_image(image_bgr: np.ndarray, labels: np.ndarray, output: Path) -> None:
    segmented = create_segmented_image(image_bgr, labels)
    output.parent.mkdir(parents=True, exist_ok=True)
    if not cv2.imwrite(str(output), segmented):
        raise OSError(f"Unable to write output image: {output}")


def maybe_show(image_bgr: np.ndarray, labels: np.ndarray) -> None:
    segmented = create_segmented_image(image_bgr, labels)
    overlay = create_boundary_overlay(image_bgr, labels)

    fig, axes = plt.subplots(1, 2, figsize=(14, 8))
    axes[0].imshow(cv2.cvtColor(segmented, cv2.COLOR_BGR2RGB))
    axes[0].set_title("Mean-color segmentation")
    axes[1].imshow(cv2.cvtColor(overlay, cv2.COLOR_BGR2RGB))
    axes[1].set_title("SLIC boundary overlay")

    for axis in axes:
        axis.set_xticks([])
        axis.set_yticks([])

    fig.tight_layout()
    plt.show()


def main() -> int:
    args = parse_args()

    if args.segments <= 0:
        print("[ERROR] --segments must be positive")
        return 1
    if args.compactness <= 0:
        print("[ERROR] --compactness must be positive")
        return 1
    if args.max_iter <= 0:
        print("[ERROR] --max-iter must be positive")
        return 1
    if args.convergence < 0:
        print("[ERROR] --convergence must be non-negative")
        return 1

    try:
        image_bgr = load_image(args.image)
        params = SLICParams(
            segments=args.segments,
            compactness=args.compactness,
            max_iter=args.max_iter,
            convergence=args.convergence,
        )
        labels, _, step = slic_segment(image_bgr, params)
        save_overlay(image_bgr, labels, args.output)
        segmented_output = (
            args.segmented_output
            if args.segmented_output is not None
            else args.output.with_name(f"{args.output.stem}_segmented{args.output.suffix}")
        )
        save_segmented_image(image_bgr, labels, segmented_output)
    except (FileNotFoundError, ValueError, OSError) as exc:
        print(f"[ERROR] {exc}")
        return 1

    print(f"[INFO] image: {args.image}")
    print(f"[INFO] output: {args.output}")
    print(f"[INFO] segmented output: {segmented_output}")
    print(f"[INFO] segments requested: {args.segments}")
    print(f"[INFO] grid step: {step}")

    if args.show:
        maybe_show(image_bgr, labels)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())