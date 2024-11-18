import os
import cgi
import urllib.parse
import sys

def handle_get():
    # Retrieve query string parameters from the environment
    query_string = os.environ.get("QUERY_STRING", "")
    # Use urllib.parse.parse_qs to parse the query string
    params = urllib.parse.parse_qs(query_string)
    return params

def handle_post():
    # Read the raw POST body from stdin
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    post_data = sys.stdin.read(content_length)
    # Parse the POST data
    params = urllib.parse.parse_qs(post_data)
    return params

def handle_chunked():
    body = ""
    while True:
        # Read the next chunk size (in hexadecimal)
        chunk_size_hex = sys.stdin.readline().strip()
        
        if not chunk_size_hex:  # If empty, break the loop
            break
        
        try:
            chunk_size = int(chunk_size_hex, 16)
        except ValueError:
            print(f"Invalid chunk size: {chunk_size_hex}")
            break
        
        if chunk_size == 0:
            break  # End of chunks
        
        # Read the chunk data
        chunk_data = sys.stdin.read(chunk_size)
        
        # Append chunk data to body
        body += chunk_data
        
        # Skip the CRLF (carriage return + line feed) after each chunk
        sys.stdin.readline()  # Read the CRLF that follows the chunk
        
    # After collecting all the chunks, we can parse the body
    params = urllib.parse.parse_qs(body)
    return params

def main():
    # Get the request method (GET or POST)
    request_method = os.environ.get("REQUEST_METHOD")
    
    params = {}

    if request_method == "GET":
        params = handle_get()
    elif request_method == "POST":
        if os.environ.get("HTTP_TRANSFER_ENCODING", "").lower() == "chunked":
            params = handle_chunked()
        else:
            params = handle_post()

    # Print the params or do further processing
    print("Content-Type: text/html\n")
    print("<html><body>")
    print("<h1>Request Method: {}</h1>".format(request_method))
    print("<p>Parameters: {}</p>".format(params))
    print("</body></html>")

if __name__ == "__main__":
    main()
