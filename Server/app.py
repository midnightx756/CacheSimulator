from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import cache_sim
import os

app = Flask(__name__)
CORS(app)

# Path to your Core folder
#CORE_DIR = os.path.join(os.path.dirname(__file__), '..', 'Core')
#BRIDGER = os.path.join(CORE_DIR, 'bridger.cpp')
#EXECUTABLE = os.path.join(CORE_DIR, 'cache_sim')

# (old commented/alternate run logic removed)
@app.route('/')
def serve_index():
    # Serve the main input page
    return send_from_directory(os.path.dirname(__file__), 'index5.html')


@app.route('/index6.html')
def serve_index6():
    return send_from_directory(os.path.dirname(__file__), 'index6.html')


@app.route('/run', methods=['POST'])
def run_simulation():
    data = request.json

    policy = data.get('policy', 'LRU')
    input_text = data.get('input_text', '')
    associativity_str = data.get('associativity')
    block_size_str = data.get('block_size')
    seconds_str = data.get('seconds')

    if not all([associativity_str, block_size_str, seconds_str, input_text]):
        return jsonify({'error': 'Missing policy, configuration, or trace data.'}), 400

    core_dir = os.path.join(os.path.dirname(__file__), '..', 'Core')
    input_file_path = os.path.join(core_dir, 'input_file.txt')
    result = ''

    try:
        # 1. Write the trace file used by the C++ core
        cache_sim.write_trace_file(input_text, input_file_path)

        # 2. Run the C++ core wrapper
        result = cache_sim.run_cache_simulator(
            input_file_path,
            policy,
            associativity_str,
            block_size_str,
            seconds_str
        )

        return jsonify({'result': result})

    except Exception as e:
        if os.path.exists(input_file_path):
            os.remove(input_file_path)
        return jsonify({'error': f"Simulation Error: {str(e)}. Check your inputs and trace format."}), 500


if __name__ == '__main__':
    # Bind to localhost:5000 explicitly
    app.run(host='127.0.0.1', port=5000, debug=True)
