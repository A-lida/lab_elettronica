#!/bin/bash

# Clean and create the build directory
rm -rf build
mkdir -p build

all_files=$(find src -type f)
all_dirs=$(find src -type d)

# Create the directory structure in the build directory
for source_path in $all_dirs; do
  dest_path="${source_path/src/build}"
  echo "Creating directory ${dest_path}"
  mkdir -p "${dest_path}"
done

# Substitute @VARNAME@ with their corresponding environment variable values
for source_path in $all_files; do
  dest_path="${source_path/src/build}"
  echo "${source_path} -> ${dest_path}"

  # Define the environment variables for substitution
  PROJECT_ROOT="$(pwd)"
  SRC="${PROJECT_ROOT}/src"
  IMAGES="${PROJECT_ROOT}/images/"

  # Perform substitution for @VARNAME@ syntax
  sed -e "s|@PROJECT_ROOT@|${PROJECT_ROOT}|g" \
      -e "s|@SRC@|${SRC}|g" \
      -e "s|@IMAGES@|${IMAGES}|g" \
      < "${source_path}" > "${dest_path}"
done

# Determine if a specific file was passed as an argument
if [ "$1" ]; then
  # Specific file mode
  source_file="$1"
  if [[ ! -f "$source_file" ]]; then
    echo "Error: File '$source_file' does not exist."
    exit 1
  fi

  # Compute destination paths
  dest_file="${source_file/src/build}"
  dest_dir=$(dirname "$dest_file")

  # Compile the LaTeX file
  output_pdf="out/${source_file/src\//}"
  output_pdf="${output_pdf%.tex}.pdf"
  output_dir=$(dirname "$output_pdf")
  mkdir -p "$output_dir"

  echo "Building PDF: ${dest_file} -> ${output_pdf}"
  latexmk -file-line-error -interaction=nonstopmode -shell-escape -synctex=1 -output-directory=$(dirname "$output_pdf") -lualatex "$dest_file"
else
  # Compile the main LaTeX document
  echo "Building PDF: build/main.tex -> out/main.pdf"
  latexmk -file-line-error -interaction=nonstopmode -shell-escape -synctex=1 -output-directory=out -lualatex build/main.tex
fi
