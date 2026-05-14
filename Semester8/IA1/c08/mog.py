#!/usr/bin/env python3
"""Simple MoG (MOG2) foreground segmentation for IA1 video task."""

from __future__ import annotations

import argparse
from pathlib import Path

import cv2


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Run MOG2 background subtraction and display a live foreground mask."
    )
    parser.add_argument(
        "--video",
        type=Path,
        default=Path("dt_passat(1).mpg"),
        help="Input video path (default: dt_passat(1).mpg)",
    )
    parser.add_argument(
        "--history",
        type=int,
        default=500,
        help="Number of frames used for background model history.",
    )
    parser.add_argument(
        "--var-threshold",
        type=float,
        default=25.0,
        help="MOG2 variance threshold; lower values make detector more sensitive.",
    )
    parser.add_argument(
        "--detect-shadows",
        action="store_true",
        help="Enable shadow detection in MOG2.",
    )
    parser.add_argument(
        "--no-morph",
        action="store_true",
        help="Disable morphology post-processing.",
    )
    parser.add_argument(
        "--learning-rate",
        type=float,
        default=-1.0,
        help="Background model learning rate (-1 means automatic OpenCV behavior).",
    )
    parser.add_argument(
        "--max-frames",
        type=int,
        default=0,
        help="Stop after N frames (0 means process full video).",
    )
    parser.add_argument(
        "--headless",
        action="store_true",
        help="Run without GUI windows (useful for environment checks).",
    )
    return parser.parse_args()


def create_mask_cleaner() -> tuple[int, tuple[int, int]]:
    return cv2.MORPH_ELLIPSE, (3, 3)


def main() -> int:
    args = parse_args()

    if not args.video.exists():
        print(f"[ERROR] Video not found: {args.video}")
        return 1

    cap = cv2.VideoCapture(str(args.video))
    if not cap.isOpened():
        print(f"[ERROR] Cannot open video: {args.video}")
        return 1

    back_sub = cv2.createBackgroundSubtractorMOG2(
        history=args.history,
        varThreshold=args.var_threshold,
        detectShadows=args.detect_shadows,
    )

    kernel_shape, kernel_size = create_mask_cleaner()
    kernel = cv2.getStructuringElement(kernel_shape, kernel_size)

    if not args.headless:
        print("[INFO] Press 'q' or ESC to quit.")

    processed = 0

    while True:
        ok, frame = cap.read()
        if not ok:
            break

        processed += 1

        fg_mask = back_sub.apply(frame, learningRate=args.learning_rate)

        if not args.no_morph:
            # Light denoising to reduce salt-and-pepper noise in moving regions.
            fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_OPEN, kernel, iterations=1)
            fg_mask = cv2.morphologyEx(fg_mask, cv2.MORPH_CLOSE, kernel, iterations=1)

        if not args.headless:
            cv2.imshow("Original", frame)
            cv2.imshow("Foreground Mask (MOG2)", fg_mask)

            key = cv2.waitKey(1) & 0xFF
            if key in (27, ord("q")):
                break

        if args.max_frames > 0 and processed >= args.max_frames:
            break

    cap.release()
    if not args.headless:
        cv2.destroyAllWindows()

    print(f"[INFO] Processed frames: {processed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
