#!/usr/bin/python3

import json
import os


DATA_FILE = "./configuration_files/db-config/data.json"


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
  body = os.environ.get('PATH_INFO')
  query = os.environ.get('QUERY_STRING')

  if data:
    title = data.get("title", "Default Title")
    message = data.get("message", "Default Message")
    return f"<h2>{title}</h2><p>{message}</h2><p>{body}</h2><p>{query}</p>"
  else:
    return "<h2>Error loading data</h2>"

def main():
  data = load_data(DATA_FILE)

  content = generate_content(data)

  body = os.environ.get('PATH_INFO')
  if body :
    print(body)

  print(content)


if __name__ == "__main__":
  main()
