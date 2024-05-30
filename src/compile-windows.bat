clang -target wasm32 -O3 -c asr/asr_helpers.c
clang -target wasm32 -O3 -c asr/malloc.c
clang -target wasm32 -O3 -c asr/mini_libc.c
clang -target wasm32 -O3 -c ringracers.c
wasm-ld --no-entry --allow-undefined-file=asr/asr.syms ringracers.o malloc.o mini_libc.o asr_helpers.o -o ringracers.wasm