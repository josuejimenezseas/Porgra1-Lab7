# Lab 7 - Contactos (Qt Desktop)

Agenda de escritorio hecha con Qt Widgets y Visual Studio. Permite registrar, ver, editar y eliminar contactos (nombre, teléfono y correo) guardados en un archivo .txt.

El proyecto está en `Lab7/` (abrir `Lab7/Lab7.slnx`).

## Archivo de datos

Los contactos se guardan en `C:/Users/<usuario>/Lab7Datos/contactos.txt` (carpeta del usuario, con `QDir::homePath()`). Cada línea es un contacto con el formato `nombre;telefono;correo`. En este repositorio hay un ejemplo en `contactos_ejemplo.txt`.

## Cómo funciona

- Registrar: formulario con nombre, teléfono y correo. Guarda con `ArchivoContactos::guardar` (modo Append). Los campos se limpian y aparece la confirmación solo si se guardó bien.
- Menú Navegación: Registrar, Historial, Editar y Eliminar, con `QMenuBar` y `QStackedWidget`.
- Historial: tabla (`QTableWidget`) con Nombre, Teléfono y Correo. Se lee todo el archivo cada vez que se abre, sin duplicar filas.
- Editar y Eliminar: `ArchivoContactos` lee todas las líneas, cambia o quita la seleccionada y reescribe el archivo con `QSaveFile`. El registro se identifica por su fila en el archivo, no por el nombre.

## Cómo comprobé editar y eliminar

1. Creé tres contactos de prueba y abrí el Historial: aparecieron los tres.
2. En Editar cambié el teléfono del segundo y en Eliminar quité el tercero.
3. Cerré y abrí la aplicación: quedaron dos contactos y el segundo conservó el cambio.
4. Probé cancelar una eliminación, editar sin seleccionar fila y abrir el historial vacío: la aplicación respondió con un aviso y no se dañaron los datos.

## Capturas

- `capturas/guardado.png`: guardado con confirmación.
- `capturas/edicion.png`: edición de un contacto con confirmación.
- `capturas/eliminado.png`: eliminación de un contacto; la tabla queda con los dos contactos restantes.
