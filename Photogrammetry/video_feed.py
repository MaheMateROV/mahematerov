import cv2
import os

# Create a folder to save images
folder_path = os.path.join(os.path.expanduser('~'), 'Desktop', 'captured_images')
os.makedirs(folder_path, exist_ok=True)

# URL of the video feed
video_feed_url = "http://192.168.2.21:5000/video_feed"

# Create video capture object
cap = cv2.VideoCapture(video_feed_url)

# Check if the camera opened successfully
if not cap.isOpened():
    print("Error: Unable to open video feed.")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # Display the resulting frame
    cv2.imshow('frame', frame)

    # Check for the 'p' key press
    key = cv2.waitKey(1) & 0xFF
    if key == ord('p'):
        # Save the frame as an image
        image_name = f"captured_image_{len(os.listdir(folder_path))}.jpg"
        image_path = os.path.join(folder_path, image_name)
        cv2.imwrite(image_path, frame)
        print(f"Image saved: {image_path}")

    # Check for the 'q' key press to quit
    if key == ord('q'):
        break

# Release the capture
cap.release()
cv2.destroyAllWindows()
