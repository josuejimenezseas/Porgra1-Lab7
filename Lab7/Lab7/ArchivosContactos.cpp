#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QSaveFile>
#include <QStringList>

QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
    const QString& telefono,
    const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

bool ArchivoContactos::leer(QList<Contacto>& contactos) const
{
    contactos.clear();
    QFile archivo(ruta());
    if (!archivo.exists())
        return true;
    if (!archivo.open(QIODevice::ReadOnly))
        return false;

    const QString texto = QString::fromUtf8(archivo.readAll());
    archivo.close();

    const QStringList lineas = texto.split('\n');
    for (QString linea : lineas) {
        if (linea.endsWith('\r'))
            linea.chop(1);
        if (linea.trimmed().isEmpty())
            continue;
        const QStringList partes = linea.split(';');
        Contacto c;
        c.nombre = partes.value(0);
        c.telefono = partes.value(1);
        c.correo = partes.mid(2).join(";");
        contactos.append(c);
    }
    return true;
}

bool ArchivoContactos::reescribir(const QList<Contacto>& contactos) const
{
    if (!QDir().mkpath(QDir::homePath() + "/Lab7Datos"))
        return false;

    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly))
        return false;

    for (const Contacto& c : contactos) {
        QByteArray datos = (c.nombre + ";" + c.telefono + ";" + c.correo + "\n").toUtf8();
        if (archivo.write(datos) != datos.size()) {
            archivo.cancelWriting();
            return false;
        }
    }
    return archivo.commit();
}

bool ArchivoContactos::actualizar(int indice, const Contacto& nuevo) const
{
    QList<Contacto> contactos;
    if (!leer(contactos) || indice < 0 || indice >= contactos.size())
        return false;
    contactos[indice] = nuevo;
    return reescribir(contactos);
}

bool ArchivoContactos::eliminar(int indice) const
{
    QList<Contacto> contactos;
    if (!leer(contactos) || indice < 0 || indice >= contactos.size())
        return false;
    contactos.removeAt(indice);
    return reescribir(contactos);
}
