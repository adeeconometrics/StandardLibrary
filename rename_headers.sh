#!/bin/bash

# Function to convert PascalCase to snake_case
to_snake_case() {
    echo "$1" | sed -E 's/([A-Z])/_\L\1/g' | sed 's/^_//'
}

# Find all .h files recursively in src directory
find src -name "*.h" | while read file; do
    # Get the directory and filename
    dir=$(dirname "$file")
    filename=$(basename "$file" .h)
    
    # Convert filename to snake_case
    snake_case=$(to_snake_case "$filename")
    
    # Construct new filename with .hpp extension
    new_file="$dir/${snake_case}.hpp"
    
    # Print the rename operation
    echo "Renaming: $file -> $new_file"
    
    # Perform git mv
    git mv "$file" "$new_file"
done
