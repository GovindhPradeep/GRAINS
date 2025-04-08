import serial
import cv2
import numpy as np
import cv2.aruco as aruco
import time

# --- Serial Setup ---
ser = serial.Serial('COM9', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to reset

# --- ArUco Calibration Function ---

def draw_cartesian_grid(frame):
    height, width = frame.shape[:2]
    center_x, center_y = width // 2, height // 2

    # Draw center lines (X and Y axes)
    cv2.line(frame, (center_x, 0), (center_x, height), (255, 0, 0), 1)  # Y-axis (blue)
    cv2.line(frame, (0, center_y), (width, center_y), (0, 255, 0), 1)   # X-axis (green)

    # Optional: draw small ticks/grid (every 50 pixels)
    for i in range(0, width, 50):
        cv2.line(frame, (i, center_y - 5), (i, center_y + 5), (0, 255, 0), 1)
    for j in range(0, height, 50):
        cv2.line(frame, (center_x - 5, j), (center_x + 5, j), (255, 0, 0), 1)

    # Add axis labels
    cv2.putText(frame, "+X", (width - 50, center_y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,255,0), 1)
    cv2.putText(frame, "-X", (10, center_y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,255,0), 1)
    cv2.putText(frame, "+Y", (center_x + 10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,0,0), 1)
    cv2.putText(frame, "-Y", (center_x + 10, height - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,0,0), 1)


def calibrate_scale_with_aruco(frame, marker_length_mm=50.0):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)

    parameters = cv2.aruco.DetectorParameters()


    corners, ids, _ = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    if ids is not None:
        for marker_corners in corners:
            c = marker_corners[0]
            pixel_width = np.linalg.norm(c[0] - c[1])
            pixels_per_mm = pixel_width / marker_length_mm
            return pixels_per_mm
    return None

# --- Circle Detection ---
def detect_circle(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (9, 9), 2)

    circles = cv2.HoughCircles(
        blurred,
        cv2.HOUGH_GRADIENT,
        dp=1.2,
        minDist=50,
        param1=100,
        param2=30,
        minRadius=20,
        maxRadius=100
    )

    if circles is not None:
        circles = np.round(circles[0, :]).astype("int")
        return circles[0]  # x, y, radius
    return None

# --- Offset Calculation ---
def calculate_offset(circle, frame_shape):
    cx, cy = frame_shape[1] // 2, frame_shape[0] // 2
    dx = circle[0] - cx
    dy = circle[1] - cy
    return dx, dy

# --- Move Decision ---
def center_on_circle(dx, dy, pixels_per_mm, threshold_mm=5):
    dx_mm = dx / pixels_per_mm
    dy_mm = dy / pixels_per_mm

    moved = False

    if abs(dx_mm) > threshold_mm:
        dir_x = 'LEFT' if dx_mm < 0 else 'RIGHT'
        print(f"Move X: {(dx_mm):.1f} mm")
        ser.write(f"MOVE_X {dx_mm}\n".encode())
        moved = True

    if abs(dy_mm) > threshold_mm:
        dir_y = 'UP' if dy_mm < 0 else 'DOWN'
        print(f"Move Y: {dy_mm:.1f} mm")
        ser.write(f"MOVE_Y {dy_mm}\n".encode())
        moved = True

    else:
        ser.write(f"SEED\n".encode())
    return moved

# --- Camera Setup ---
cap = cv2.VideoCapture(1)
if not cap.isOpened():
    print("Error: Cannot open camera")
    exit()

# --- Calibration Loop ---
pixels_per_mm = None
print("Show the ArUco marker to calibrate scale (e.g., 50 mm wide)...")
while pixels_per_mm is None:
    ret, frame = cap.read()
    if not ret:
        continue

    pixels_per_mm = calibrate_scale_with_aruco(frame, marker_length_mm=50)
    cv2.putText(frame, "Show ArUco marker for calibration...", (30, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)
    cv2.imshow("Calibration", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

print(f"Calibration complete: {pixels_per_mm:.2f} pixels/mm")

# --- Main Loop ---
print("Starting circle detection...")
line = ""
while True:
    ret, frame = cap.read()
    if not ret:
        continue
    circle = detect_circle(frame)
    draw_cartesian_grid(frame)
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()

    if circle is not None and (line == "moved in Y" or line == "moved in X" or line == "Started"):
        dx, dy = calculate_offset(circle, frame.shape)
        center_on_circle(dx, dy, pixels_per_mm)

        # Draw feedback
        cv2.circle(frame, (circle[0], circle[1]), circle[2], (0, 255, 0), 2)
        cv2.circle(frame, (circle[0], circle[1]), 2, (0, 0, 255), 3)
    
    # Draw center point
    cv2.circle(frame, (frame.shape[1]//2, frame.shape[0]//2), 4, (255, 0, 0), -1)

    cv2.imshow("Tracking", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
ser.close()

