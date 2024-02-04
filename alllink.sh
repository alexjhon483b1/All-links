#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <html_file> <domain>"
    exit 1
fi

html_file=$1
domain=$2

cat "$html_file" | grep -o 'src="[^"]*"' | awk -F'"' '{print $2}' >> l.txt
cat l.txt | grep "$domain" >> m.txt
rm l.txt
sort -u m.txt >> unique_links.txt
rm m.txt
echo "Unique links from $html_file containing $domain have been saved to unique_links.txt."

