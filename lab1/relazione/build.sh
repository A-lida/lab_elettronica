#! /bin/bash

# creo la directory dove copio i file con le variabili sostituite
rm -rf build
mkdir -p build

all_files=$(find src -type f)
all_dirs=$(find src -type d)

for source_path in $all_dirs; do
  dest_path="${source_path/src/"build"}"
  echo "making dir ${dest_path}"
  mkdir -p "${dest_path}"
done

for source_path in $all_files; do
  dest_path="${source_path/src/"build"}"
  echo "${source_path} -> ${dest_path}"
  PROJECT_ROOT="$(pwd)" \
  SRC="${PROJECT_ROOT}/src" \
  IMAGES="${PROJECT_ROOT}/images" \
  envsubst '$PROJECT_ROOT:$SRC:$IMAGES' < "${source_path}" > "${dest_path}"
done

latexmk -file-line-error -interaction=nonstopmode -synctex=1 -output-directory=out -lualatex build/main.tex