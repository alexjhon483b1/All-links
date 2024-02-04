#!/bin/bash

# Check if a file argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <file_with_links> <domain>"
    exit 1
fi

file_with_links=$1

# Check if the file exists
if [ ! -f "$file_with_links" ]; then
    echo "File not found: $file_with_links"
    exit 1
fi

# Read each line from the file and use curl to make a request
while IFS= read -r link; do
    echo "Curling: $link" 
    ./visitpath "$link" >> x.html
    echo "--------------------------"
done < "$file_with_links"
./alllink.sh x.html $2
