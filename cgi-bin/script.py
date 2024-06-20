#!/usr/bin/python3

import json


DATA_FILE = "./configuration_files/db-config/data.json"


ciao = 'ciao'
def load_data(filename):
  """Loads data from a JSON file."""
  try:
    with open(filename, "r") as f:
      return json.load(f)
  except (FileNotFoundError, json.JSONDecodeError) as e:
    print(f"Error loading data: {e}")
    return None

def generate_content(data):
  """Generates content based on data from JSON file."""


  if data:
    title = data.get("title", "Default Title")
    message = data.get("message", "Default Message")
    return f"""
<!doctype html>
<html>
<head>
<h2>{title}</h2>
</head>
<body>
<p>{message}</p>
</body>
</html>
"""
  else:
    return f"""
<!doctype html>
<html>
<head>
<h2>nammerda</h2>
</head>
<body>
<p>cgiErrorNaMMerda</p>
</body>
</html>
"""

def main():
  data = load_data(DATA_FILE)

  content = generate_content(data)

  print(content)


if __name__ == "__main__":
  main()
