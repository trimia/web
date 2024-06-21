#!/bin/bash

DATA_FILE="./configuration_files/db-config/data.json"

# Check if data file exists
if [[ ! -f "$DATA_FILE" ]]; then
  echo "Error: data file '$DATA_FILE' not found"
  exit 1
fi

# Load data using jq (assuming it's installed)
data=$(jq '{}' "$DATA_FILE")  # Initialize empty object

# Extract title (default to "Default Title")
title=$(jq -r '.title // "Default Title"' "$DATA_FILE")
message=$(jq -r '.message // "Default Message"' "$DATA_FILE")

# Generate content
content="
<!doctype html>
<html>
<head>
<h2>$title</h2>
</head>
<body>
<p>$message</p>
</body>
</html>
"

# Print content
echo "$content"
