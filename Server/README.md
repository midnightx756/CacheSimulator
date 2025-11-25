# Server

Quick start (from project root):

```bash
cd Server
python3 app.py
```

Open the UI at: `http://127.0.0.1:5000/`

Notes:
- The server serves `index5.html` and `index6.html` and exposes a POST `/run` endpoint used by the frontend.
- If you host the frontend on a different origin, enable CORS or change the frontend fetch URL to the server origin.
- To troubleshoot, open browser devtools Network tab and inspect the POST to `/run`.
