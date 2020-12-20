# Fantastic Filter CLI
A CLI tool for [Fantastic Filter](https://ray-fish.me/Fantastic-Filter-Professional-Plus) written in C.

## Usage
Enter `fantastic-Filter-cli --help` for helps.
### TL;DR
#### Run Benchmark
```bash
fantastic-Filter-cli -b[=LEVEL] # where LEVEL is 0-4
```
note: Level 0 is for testing, which might be TOO stressless for GPU, but enough for CPU.

#### Process An Image
```bash
fantastic-Filter-cli -o <OUT_DIR> [-w=MAX_HEIGHT] [-h=MAX_HEIGHT] <input_files...>
```
MAX_HEIGHT & MAX_WIDTH is depending on how much (V)RAM do you have. e.g. an RTX 2080ti with 11GB VRAM can process the image larger than 3840x2160, but 1920x1080 is too much for an GTX 1050 with 2GB VRAM. Try to reduce the size if you get <span title="Out of Memory">OOM</span> error.
## Pre-requirements
- `libtensorflow` (tested: `2.3` `2.4`)
- `gcc` or `clang` (you can change it in `Makefile`)
- `make`
### Optional
- `cuda` & `cudnn` (tested: `cuda-10.1` & `cudnn-7`)


## Build & Install
```bash
make
```
After `make`, `./bin/fantastic-filter-cli` will be created, move it to somewhere if you want.

Download FF_Model.zip from releases, extract, and set enviorment variable `FF_MODEL_PATH` to the direcrory.

## Docker
Docker with GPUs is not working now somehow, PR will be very welcome if someone can figure out how to build docker of [Tensorflow for C](https://www.tensorflow.org/install/lang_c), but you can still run it without GPU support.

The basic idea is mount the directory that where the image you want to process to docker.

Here's an example:
```bash
docker run -v <dir_on_host>:/myDir -it <image_name> fantastic-filter-cli tmp -o /myDir <filename>
```
