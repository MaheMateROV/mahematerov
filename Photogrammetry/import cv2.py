import cv2

# Global variables to store the selected points
point1_ref = None
point2_ref = None
point1_obj = None
point2_obj = None
clicked_ref = False
clicked_obj = False
reference_length_px = None
reference_breadth_px = None

# Function to handle mouse events for reference rectangle
def mouse_callback_ref(event, x, y, flags, param):
    global point1_ref, point2_ref, clicked_ref, reference_length_px, reference_breadth_px

    if event == cv2.EVENT_LBUTTONDOWN:
        if not clicked_ref:
            point1_ref = (x, y)
            clicked_ref = True
        else:
            point2_ref = (x, y)
            clicked_ref = False
            reference_length_px, reference_breadth_px = calculate_length_breadth(point1_ref, point2_ref)

# Function to handle mouse events for object rectangle
def mouse_callback_obj(event, x, y, flags, param):
    global point1_obj, point2_obj, clicked_obj

    if event == cv2.EVENT_LBUTTONDOWN:
        if not clicked_obj:
            point1_obj = (x, y)
            clicked_obj = True
        else:
            point2_obj = (x, y)
            clicked_obj = False
            length_px, breadth_px = calculate_length_breadth(point1_obj, point2_obj)
            length_cm = (length_px / reference_length_px) * 93  # Convert pixels to cm using reference scale
            breadth_cm = (breadth_px / reference_breadth_px) * 66  # Convert pixels to cm using reference scale
            print(f"Object dimensions: Length: {length_cm} cm, Breadth: {breadth_cm} cm")

# Calculate length and breadth of rectangle
def calculate_length_breadth(point1, point2):
    length = abs(point2[0] - point1[0])
    breadth = abs(point2[1] - point1[1])
    return length, breadth

# Open webcam with increased frame size
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)  # Set width to 1280 pixels
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)  # Set height to 720 pixels

cv2.namedWindow('Webcam')
cv2.setMouseCallback('Webcam', mouse_callback_ref)  # Use reference mouse callback initially

while True:
    ret, frame = cap.read()

    # Draw reference rectangle if both points are selected
    if point1_ref and point2_ref:
        cv2.rectangle(frame, point1_ref, point2_ref, (0, 255, 0), 2)

    # Draw object rectangle if both points are selected
    if point1_obj and point2_obj:
        cv2.rectangle(frame, point1_obj, point2_obj, (255, 0, 0), 2)

    cv2.imshow('Webcam', frame)

    # Switch mouse callback when reference rectangle is drawn
    if reference_length_px is not None and reference_breadth_px is not None:
        cv2.setMouseCallback('Webcam', mouse_callback_obj)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
