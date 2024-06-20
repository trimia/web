#!/usr/bin/python3

# import json
import os


# DATA_FILE = "./configuration_files/db-config/data.json"

def get_lowercase_env_vars():
  """Returns a dictionary of environment variables with lowercase names."""
  env_vars = os.environ
  lowercase_env_vars = {k: v for k, v in env_vars.items() if k.islower()}
  return lowercase_env_vars

def dict_to_string(dict_obj):
  """Converts a dictionary into a string."""
  return '<br>'.join(f"<code>{k} = {v}</code>" for k, v in dict_obj.items())
def generate_content(data):
  """Generates content based on data from JSON file."""


  if data:
    title = "GET QUERY"
    message = dict_to_string(data)
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
  data = get_lowercase_env_vars()

  data_string = generate_content(data)

  print(data_string)

if __name__ == "__main__":
  main()