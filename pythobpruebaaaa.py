from picamera import PiCamera
import time
import cv2

# Inicializamos la cámara con resolución 640x480
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

# Tiempo de espera para que la cámara arranque
time.sleep(0.5)

# Inicializamos el primer frame a vacío.
# Nos servirá para obtener el fondo
fondo = None

# Capturamos frame a frame de la cámara
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    # Obtenemos el array en formato NumPy
    image = frame.array

    # Convertimos a escala de grises
    gris = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Aplicamos suavizado para eliminar ruido
    gris = cv2.GaussianBlur(gris, (21, 21), 0)

    # Si todavía no hemos obtenido el fondo, lo obtenemos
    # Será el primer frame que obtengamos
    if fondo is None:
        fondo = gris

    # Calculo de la diferencia entre el fondo y el frame actual
    resta = cv2.absdiff(fondo, gris)

    # Aplicamos un umbral
    umbral = cv2.threshold(resta, 25, 255, cv2.THRESH_BINARY)[1]

    # Dilatamos el umbral para tapar agujeros
    umbral = cv2.dilate(umbral, None, iterations=2)

    # Copiamos el umbral para detectar los contornos
    contornosimg = umbral.copy()

    # Buscamos contorno en la imagen
    contornos, hierarchy = cv2.findContours(contornosimg, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Recorremos todos los contornos encontrados
    for c in contornos:
        # Eliminamos los contornos más pequeños
        if cv2.contourArea(c) < 500:
            continue

        # Obtenemos el bounds del contorno, el rectángulo mayor que engloba al contorno
        (x, y, w, h) = cv2.boundingRect(c)
        # Dibujamos el rectángulo del bounds
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Mostramos las diferentes capturas
    cv2.imshow("Imagen Movimiento", image)
    cv2.imshow("Umbral", umbral)
    cv2.imshow("Resta", resta)
    cv2.imshow("Contornos", contornosimg)
    key = cv2.waitKey(1) & 0xFF

    # Reseteamos el archivo raw para la siguiente captura
    rawCapture.truncate(0)

    # Con la letra s salimos de la aplicación
    if key == ord("s"):
        break
