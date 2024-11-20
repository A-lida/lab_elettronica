#!/bin/bash

# Create the build directory where files with substituted variables will be copied
rm -rf build
mkdir -p build

all_files=$(find src -type f)
all_dirs=$(find src -type d)

# Create the directory structure in the build directory
for source_path in $all_dirs; do
  dest_path="${source_path/src/"build"}"
  echo "Creating directory ${dest_path}"
  mkdir -p "${dest_path}"
done

# Substitute @VARNAME@ with their corresponding environment variable values
for source_path in $all_files; do
  dest_path="${source_path/src/"build"}"
  echo "${source_path} -> ${dest_path}"
  
  # Define the environment variables for substitution
  PROJECT_ROOT="$(pwd)"
  SRC="${PROJECT_ROOT}/src"
  IMAGES="${PROJECT_ROOT}/images"
  
  # Perform substitution for @VARNAME@ syntax
  sed -e "s|@PROJECT_ROOT@|${PROJECT_ROOT}|g" \
      -e "s|@SRC@|${SRC}|g" \
      -e "s|@IMAGES@|${IMAGES}|g" \
      < "${source_path}" > "${dest_path}"
done

# Compile the LaTeX document
latexmk -file-line-error -interaction=nonstopmode -synctex=1 -output-directory=out -lualatex build/main.tex
