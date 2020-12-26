# Fantastic Filter CLI [![Build Status](https://travis-ci.com/ray1422/Fantastic-Filter-CLI.svg?branch=master)](https://travis-ci.com/ray1422/Fantastic-Filter-CLI) [![CodeFactor](https://www.codefactor.io/repository/github/ray1422/fantastic-filter-cli/badge)](https://www.codefactor.io/repository/github/ray1422/fantastic-filter-cli)
A CLI tool for [Fantastic Filter](https://ray-fish.me/Fantastic-Filter-Professional-Plus) written in C.

## Usage
Enter `fantastic-Filter-cli --help` for helps.
### TL;DR
#### Run Benchmark
```bash
fantastic-Filter-cli -b[=LEVEL] # where LEVEL is 0-4
```
Note: Level 0 is for testing, which might be TOO stressless for GPU, but enough for CPU.

#### Process An Image
```bash
fantastic-Filter-cli -o <OUT_DIR> [-w=MAX_HEIGHT] [-h=MAX_HEIGHT] <input_files...>
```
MAX_HEIGHT & MAX_WIDTH depends on how much (V)RAM you have. For example, an RTX 2080ti with 11GB VRAM can process images larger than 3840x2160, but 1920x1080 is too much for a GTX 1050 with 2GB VRAM. Try to reduce the size if you get <span title="Out of Memory">OOM</span> error.
## Pre-requirements
- `libtensorflow` (tested: `2.3` `2.4`)
- `gcc` or `clang` (set `$CC`)
- `make`
### Optional
- `cuda` & `cudnn` (tested: `cuda-10.1` & `cudnn-7`)


## Build & Install
```bash
make
```
After `make`, `./bin/fantastic-filter-cli` will be generated. Move it to somewhere if you want.

Download `FF_Model.tar.gz` from releases, extract it, and set the environment variable `FF_MODEL_PATH` to the directory.

## Docker

The basic idea is to mount the directory where the image you want to process to docker.

Here's an example:
```bash
docker run --gpus all -v <dir_on_host>:/myDir -it <image_name> fantastic-filter-cli tmp -o /myDir <filename>
```

# TODO
Build an app image.
