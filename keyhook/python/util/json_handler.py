try: import simplejson as json
except ImportError: import json

def load_json(filename):
    with open(filename, "r") as f:
        json_data = f.read()
        decoded_json = json.loads(json_data)
        return decoded_json


def write_json(data, fd=None, filename=None):
    """Write the provided json out to a file. If a fd (file descriptor), which in
    python I just mean an open file, is provided then it will be written to that
    file. If a filename is provided it will be opened in write mode with truncation
    and the json data will be dumped. Otherwise nothing will happen.
    """

    if fd is not None:
        fd.write(json.dumps(data))
        return

    if filename is not None:
        with open(filename) as f:
            f.write(json.dumps(data))

        return
