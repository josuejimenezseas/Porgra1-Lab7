#include "Lab7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QAction>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>

Lab7::Lab7(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnGuardar, &QPushButton::clicked,
        this, &Lab7::guardarContacto);

    connect(ui.actionRegistrar, &QAction::triggered, this,
        [this] { ui.stackedWidget->setCurrentWidget(ui.pageRegistrar); });
    connect(ui.actionHistorial, &QAction::triggered, this,
        [this] { mostrarTabla(Modo::Historial); });
    connect(ui.actionEditar, &QAction::triggered, this,
        [this] { mostrarTabla(Modo::Editar); });
    connect(ui.actionEliminar, &QAction::triggered, this,
        [this] { mostrarTabla(Modo::Eliminar); });

    ui.tblContactos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui.tblContactos, &QTableWidget::itemSelectionChanged,
        this, &Lab7::llenarFormularioEdicion);
    connect(ui.btnActualizar, &QPushButton::clicked,
        this, &Lab7::actualizarContacto);
    connect(ui.btnEliminar, &QPushButton::clicked,
        this, &Lab7::eliminarContacto);

    mostrarTabla(Modo::Historial);
    ui.stackedWidget->setCurrentWidget(ui.pageRegistrar);
}

Lab7::~Lab7()
{}

void Lab7::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
            "Completá los tres campos.");
        return;
    }

    for (const QString& dato : QStringList{ nombre, telefono, correo }) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return;
        }
    }

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}

bool Lab7::datosValidos(const QString& nombre, const QString& telefono,
    const QString& correo)
{
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
            "Completá los tres campos.");
        return false;
    }

    for (const QString& dato : QStringList{ nombre, telefono, correo }) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return false;
        }
    }
    return true;
}

void Lab7::mostrarTabla(Modo nuevoModo)
{
    modo = nuevoModo;

    const bool editar = (modo == Modo::Editar);
    const bool eliminar = (modo == Modo::Eliminar);

    switch (modo) {
    case Modo::Historial:
        ui.lblTituloTabla->setText("Historial de contactos");
        break;
    case Modo::Editar:
        ui.lblTituloTabla->setText("Editar contacto: seleccioná una fila y cambiá los datos");
        break;
    case Modo::Eliminar:
        ui.lblTituloTabla->setText("Eliminar contacto: seleccioná una fila");
        break;
    }

    ui.groupEdicion->setVisible(editar);
    ui.btnActualizar->setVisible(editar);
    ui.btnEliminar->setVisible(eliminar);

    ui.stackedWidget->setCurrentWidget(ui.pageTabla);
    cargarTabla();
}

void Lab7::cargarTabla()
{
    ui.tblContactos->clearSelection();
    ui.tblContactos->setRowCount(0);

    QList<Contacto> contactos;
    if (!archivoContactos.leer(contactos)) {
        QMessageBox::critical(this, "No se pudo leer",
            "No se pudo leer el archivo.\nArchivo: "
            + archivoContactos.ruta());
        ui.lblIndicacion->setText("No se pudo leer el archivo.");
        return;
    }

    ui.tblContactos->setRowCount(contactos.size());
    for (int fila = 0; fila < contactos.size(); ++fila) {
        ui.tblContactos->setItem(fila, 0, new QTableWidgetItem(contactos[fila].nombre));
        ui.tblContactos->setItem(fila, 1, new QTableWidgetItem(contactos[fila].telefono));
        ui.tblContactos->setItem(fila, 2, new QTableWidgetItem(contactos[fila].correo));
    }

    ui.lblIndicacion->setText(contactos.isEmpty()
        ? "No hay contactos guardados todavía."
        : QString("Contactos: %1").arg(contactos.size()));
    llenarFormularioEdicion();
}

int Lab7::filaSeleccionada() const
{
    const QList<QTableWidgetSelectionRange> rangos = ui.tblContactos->selectedRanges();
    if (rangos.isEmpty())
        return -1;
    return rangos.first().topRow();
}

void Lab7::llenarFormularioEdicion()
{
    const int fila = filaSeleccionada();
    if (fila < 0) {
        ui.txtEditNombre->clear();
        ui.txtEditTelefono->clear();
        ui.txtEditCorreo->clear();
        return;
    }
    ui.txtEditNombre->setText(ui.tblContactos->item(fila, 0)->text());
    ui.txtEditTelefono->setText(ui.tblContactos->item(fila, 1)->text());
    ui.txtEditCorreo->setText(ui.tblContactos->item(fila, 2)->text());
}

void Lab7::actualizarContacto()
{
    const int fila = filaSeleccionada();
    if (fila < 0) {
        QMessageBox::information(this, "Sin selección",
            "Seleccioná una fila de la tabla para editarla.");
        return;
    }

    Contacto nuevo;
    nuevo.nombre = ui.txtEditNombre->text().trimmed();
    nuevo.telefono = ui.txtEditTelefono->text().trimmed();
    nuevo.correo = ui.txtEditCorreo->text().trimmed();

    if (!datosValidos(nuevo.nombre, nuevo.telefono, nuevo.correo))
        return;

    if (!archivoContactos.actualizar(fila, nuevo)) {
        QMessageBox::critical(this, "No se pudo editar",
            "No se pudo actualizar el archivo.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    cargarTabla();
    ui.tblContactos->selectRow(fila);
    QMessageBox::information(this, "Edición completa",
        "El contacto se actualizó correctamente.");
}

void Lab7::eliminarContacto()
{
    const int fila = filaSeleccionada();
    if (fila < 0) {
        QMessageBox::information(this, "Sin selección",
            "Seleccioná una fila de la tabla para eliminarla.");
        return;
    }

    const QString nombre = ui.tblContactos->item(fila, 0)->text();
    const auto respuesta = QMessageBox::question(
        this, "Confirmar eliminación",
        "¿Seguro que querés eliminar a " + nombre + "?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (respuesta != QMessageBox::Yes)
        return;

    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "No se pudo eliminar",
            "No se pudo actualizar el archivo.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    cargarTabla();
    QMessageBox::information(this, "Eliminado",
        "El contacto se eliminó correctamente.");
}
