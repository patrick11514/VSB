import cv2 as cv
from ultralytics import YOLO

model = YOLO("yolo26m.pt")
model2 = YOLO("yolo26m-seg.pt")

cam = cv.VideoCapture("https://stream.ovanet.cz/ovanet/camera22/playlist.m3u8?auth=b64%3AY2FtZXJhMjI6OjoxMDg0NzUxMDAyOjE3ODk0Nzc2NDcwNTc6MTc4OTQ4MTI0NzoxNTguMTk2LjIzNi4xMTM6MDc2ZjhhZjYwOGJhMWIzMjAzYjg1MWRhNzlhYjY2NDU%3D")
#cam.set(cv.CAP_PROP_FRAME_WIDTH, 1280)
#cam.set(cv.CAP_PROP_FRAME_HEIGHT, 720)

cv.namedWindow("Camera", cv.WINDOW_FREERATIO)
#cv.namedWindow("Edge", cv.WINDOW_FREERATIO)


writer = cv.VideoWriter("out.avi", cv.VideoWriter_fourcc(*"MJPG"), cam.get(cv.CAP_PROP_FPS), (1280, 720), False)

detector = cv.CascadeClassifier("haarcascades/haarcascade_frontalface_default.xml") 

while True:
    ret, frame = cam.read()

    if not ret:
        print("Failed to grab frame")
        break    


    frame = cv.resize(frame, (1280, 720))

    #edge = cv.Canny(frame, 30, 120)

    #faces = detector.detectMultiScale(frame, 1.2, 2)

    #for r in faces:
    #    cv.rectangle(frame, r, (255, 0, 0), 2)
    results = model.predict(frame, conf=0.25, verbose=False)
    boxes = results[0].boxes

    for box in boxes:
        cls_id = int(box.cls[0])
        conf = box.conf[0]
        x1, y1, x2, y2 = box.xyxy[0]
        cv.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
        cv.putText(frame, f"{model.names[cls_id]} {conf:.2f}", (int(x1), int(y1) - 10), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    results2 = model2.predict(frame, conf=0.25, verbose=False)
    img2 = results2[0].plot()

    print(f"img shape: {frame.shape}")

    cv.imshow("Camera", frame)
    cv.imshow("Segmentation", img2)
    #cv.imshow("Edge", edge)

    key = cv.waitKey(2)

    #writer.write(edge)

    if key == ord('q'):
        break

#writer.release()