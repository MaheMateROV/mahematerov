import cv2
import threading
import keyboard
import time

class ImageProcessor:
    def __init__(self, image_path):
        self.image_path = "captured_image.jpg"
        self.point1_ref = None
        self.point2_ref = None
        self.point1_obj = None
        self.point2_obj = None
        self.clicked_ref = False
        self.clicked_obj = False
        self.reference_length_px = None
        self.reference_breadth_px = None
        self.mouse_position = (0, 0)
        self.reference_drawn = False

    def reset_points(self):
        self.point1_ref = None
        self.point2_ref = None
        self.point1_obj = None
        self.point2_obj = None
        self.clicked_ref = False
        self.clicked_obj = False
        self.reference_length_px = None
        self.reference_breadth_px = None
        self.reference_drawn = False
        cv2.setMouseCallback('Image', self.mouse_callback_ref)

    def mouse_callback_ref(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            if not self.clicked_ref:
                self.point1_ref = (x, y)
                self.clicked_ref = True
            else:
                self.point2_ref = (x, y)
                self.clicked_ref = False
                self.reference_length_px, self.reference_breadth_px = self.calculate_length_breadth(self.point1_ref, self.point2_ref)
                self.reference_drawn = True
                cv2.setMouseCallback('Image', self.mouse_callback_obj)

        self.mouse_position = (x, y)

    def mouse_callback_obj(self, event, x, y, flags, param):
        if self.reference_drawn:
            if event == cv2.EVENT_LBUTTONDOWN:
                if not self.clicked_obj:
                    self.point1_obj = (x, y)
                    self.clicked_obj = True
                else:
                    self.point2_obj = (x, y)
                    self.clicked_obj = False

            self.mouse_position = (x, y)

    def draw_dimensions(self, frame, length_cm, breadth_cm, point1, point2):
        cv2.putText(frame, f"Length: {length_cm:.2f} cm", (point1[0], point1[1] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
        cv2.putText(frame, f"Breadth: {breadth_cm:.2f} cm", (point2[0], point2[1] + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

    def calculate_length_breadth(self, point1, point2):
        length = abs(point2[0] - point1[0])
        breadth = abs(point2[1] - point1[1])
        return length, breadth

    def run(self):
        image = cv2.imread(self.image_path)
        cv2.namedWindow('Image', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('Image', 1280, 720)
        cv2.setMouseCallback('Image', self.mouse_callback_ref)

        while True:
            try:
                window_size = cv2.getWindowImageRect('Image')[2:]
                frame_copy = image.copy()

                if self.point1_ref and self.point2_ref:
                    cv2.rectangle(frame_copy, self.point1_ref, self.point2_ref, (0, 255, 0), 2)

                if self.point1_obj and self.point2_obj:
                    cv2.rectangle(frame_copy, self.point1_obj, self.point2_obj, (255, 0, 0), 2)
                    length_px, breadth_px = self.calculate_length_breadth(self.point1_obj, self.point2_obj)
                    length_cm = (length_px / self.reference_length_px) * 29.7
                    breadth_cm = (breadth_px / self.reference_breadth_px) * 21
                    self.draw_dimensions(frame_copy, length_cm, breadth_cm, self.point1_obj, self.point2_obj)

                cv2.line(frame_copy, (self.mouse_position[0], 0), (self.mouse_position[0], window_size[1]), (0, 255, 255), 1)
                cv2.line(frame_copy, (0, self.mouse_position[1]), (window_size[0], self.mouse_position[1]), (0, 255, 255), 1)

                cv2.imshow('Image', frame_copy)

                key = cv2.waitKey(1)
                if key == ord('r'):
                    self.reset_points()
            except:
                return

        cv2.destroyAllWindows()

class CameraThread(threading.Thread):
    def __init__(self, thread_id, name, capture):
        threading.Thread.__init__(self)
        self.thread_id = thread_id
        self.name = name
        self.capture = capture
        self.running = threading.Event()

    def run(self):
        print("Starting " + self.name)
        self.running.set()
        while self.running.is_set():
            ret, frame = self.capture.read()
            if ret:
                cv2.imshow(self.name, frame)
            key = cv2.waitKey(1)
            if key == ord('q'):
                self.running.clear()
                break
            if key == ord('p'):
                print("saved file")
                cv2.imwrite("captured_image.jpg", frame)

def main():
    video_feed_url="http://192.168.2.21:5000/video_feed"
    cap = cv2.VideoCapture(video_feed_url)
    thread = CameraThread(1, "CameraThread", cap)
    thread.start()

    while not keyboard.is_pressed('q'):
        if keyboard.is_pressed('p'):
            print("pressed p")
            time.sleep(0.5)
            obj = ImageProcessor("captured_image.jpg")
            obj.run()

if __name__ == "__main__":
    main()
