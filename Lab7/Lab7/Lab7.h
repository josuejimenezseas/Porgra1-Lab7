#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab7.h"
#include "ArchivoContactos.h"


class Lab7 : public QMainWindow
{
    Q_OBJECT

public:
    Lab7(QWidget *parent = nullptr);
    ~Lab7();

private:
    enum class Modo { Historial, Editar, Eliminar };

    Ui::Lab7Class ui;
    ArchivoContactos archivoContactos;
    Modo modo = Modo::Historial;

    void guardarContacto();
    void mostrarTabla(Modo nuevoModo);
    void cargarTabla();
    void llenarFormularioEdicion();
    void actualizarContacto();
    void eliminarContacto();
    int filaSeleccionada() const;
    bool datosValidos(const QString& nombre, const QString& telefono,
        const QString& correo);

};

