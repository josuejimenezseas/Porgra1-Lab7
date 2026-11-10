#pragma once
#include <QString>
#include <QList>

struct Contacto
{
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos
{
public:
    QString ruta() const;
    bool guardar(const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    bool leer(QList<Contacto>& contactos) const;
    bool actualizar(int indice, const Contacto& nuevo) const;
    bool eliminar(int indice) const;

private:
    bool reescribir(const QList<Contacto>& contactos) const;
};

//bool devuelve true si se logró escribir y false si hubo un problema.Los parámetros llevan los tres datos que vienen del formulario.
