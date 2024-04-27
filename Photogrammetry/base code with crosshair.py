import cv2

# Global variables to store the selected points and mouse position
point1_ref = None
point2_ref = None
point1_obj = None
point2_obj = None
clicked_ref = False
clicked_obj = False
reference_length_px = None
reference_breadth_px = None
mouse_position = (0, 0)

# Function to reset selected points
def reset_points():
    global point1_ref, point2_ref, point1_obj, point2_obj, clicked_ref, clicked_obj, reference_length_px, reference_breadth_px
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
    global point1_ref, point2_ref, clicked_ref, reference_length_px, reference_breadth_px, mouse_position

    if event == cv2.EVENT_LBUTTONDOWN:
        if not clicked_ref:
            point1_ref = (x, y)
            clicked_ref = True
        else:
            point2_ref = (x, y)
            clicked_ref = False
            reference_length_px, reference_breadth_px = calculate_length_breadth(point1_ref, point2_ref)
    mouse_position = (x, y)

# Function to handle mouse events for object rectangle
def mouse_callback_obj(event, x, y, flags, param):
    global point1_obj, point2_obj, clicked_obj, mouse_position

    if event == cv2.EVENT_LBUTTONDOWN:
        if not clicked_obj:
            point1_obj = (x, y)
            clicked_obj = True
        else:
            point2_obj = (x, y)
            clicked_obj = False
            length_px, breadth_px = calculate_length_breadth(point1_obj, point2_obj)
            length_cm = (length_px / reference_length_px) * 29.7  # Convert pixels to cm using reference scale
            breadth_cm = (breadth_px / reference_breadth_px) * 21  # Convert pixels to cm using reference scale
            print(f"Object dimensions: Length: {length_cm} cm, Breadth: {breadth_cm} cm")
            # Draw dimensions on the frame
            draw_dimensions(frame, length_cm, breadth_cm)

    mouse_position = (x, y)

# Function to draw length and breadth on the frame
def draw_dimensions(frame, length_cm, breadth_cm):
    cv2.putText(frame, f"Length: {length_cm:.2f} cm", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
    cv2.putText(frame, f"Breadth: {breadth_cm:.2f} cm", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

# Calculate length and breadth of rectangle
def calculate_length_breadth(point1, point2):
    length = abs(point2[0] - point1[0])
    breadth = abs(point2[1] - point1[1])
    return length, breadth

# Open webcam
cap = cv2.VideoCapture(0)

cv2.namedWindow('Webcam', cv2.WINDOW_NORMAL)  # Create a resizable window
cv2.resizeWindow('Webcam', 800, 600)  # Initial size

cv2.setMouseCallback('Webcam', mouse_callback_ref)  # Use reference mouse callback initially

while True:
    ret, frame = cap.read()

    # Get the size of the window
    window_size = cv2.getWindowImageRect('Webcam')[2:]

    # Draw reference rectangle if both points are selected
    if point1_ref and point2_ref:
        cv2.rectangle(frame, point1_ref, point2_ref, (0, 255, 0), 2)

    # Draw object rectangle if both points are selected
    if point1_obj and point2_obj:
        cv2.rectangle(frame, point1_obj, point2_obj, (255, 0, 0), 2)
        # Calculate dimensions
        length_px, breadth_px = calculate_length_breadth(point1_obj, point2_obj)
        length_cm = (length_px / reference_length_px) * 29.7  # Convert pixels to cm using reference scale
        breadth_cm = (breadth_px / reference_breadth_px) * 21  # Convert pixels to cm using reference scale
        # Draw dimensions on the frame
        draw_dimensions(frame, length_cm, breadth_cm)

    # Draw crosshair at mouse position
    cv2.line(frame, (mouse_position[0], 0), (mouse_position[0], window_size[1]), (0, 255, 255), 1)
    cv2.line(frame, (0, mouse_position[1]), (window_size[0], mouse_position[1]), (0, 255, 255), 1)

    cv2.imshow('Webcam', frame)

    # Switch mouse callback when reference rectangle is drawn
    if reference_length_px is not None and reference_breadth_px is not None:
        cv2.setMouseCallback('Webcam', mouse_callback_obj)

    key = cv2.waitKey(1)
    if key & 0xFF == ord('q'):
        break
    elif key & 0xFF == ord('r'):
        reset_points()

cap.release()
cv2.destroyAllWindows()
