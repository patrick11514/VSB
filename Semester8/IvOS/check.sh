#!/bin/bash

sudo losetup -P -f --show disk.img
sudo fsck.fat -v -n /dev/loop0p1
sudo losetup -d /dev/loop0