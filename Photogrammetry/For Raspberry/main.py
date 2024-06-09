from flask import Flask, render_template, Response
from camera import VideoCamera
import cv2
import os
import sys

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

def gen(camera):
    while True:
        try:
            frame = camera.get_frame()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')
        except Exception as e:
            print(f"Error in generating frame: {e}", file=sys.stderr)
            terminate_app()

@app.route('/video_feed')
def video_feed():
    try:
        return Response(gen(VideoCamera()),
                        mimetype='multipart/x-mixed-replace; boundary=frame')
    except Exception as e:
        print(f"Error in video feed: {e}", file=sys.stderr)
        terminate_app()

@app.errorhandler(Exception)
def handle_exception(e):
    print(f"Unhandled exception: {e}", file=sys.stderr)
    terminate_app()
    return "An error occurred, and the application is shutting down.", 500

def terminate_app():
    print("Terminating application...", file=sys.stderr)
    os._exit(1)

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=False)
