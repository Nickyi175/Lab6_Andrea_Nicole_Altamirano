#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    crearArchivos();
    cargarArchivo();
    inicializarArchivo();
    mostrarProductos();
    QIntValidator *validator = new QIntValidator(0, 1000000, this);
    ui->lE_codigo->setValidator(validator);
    ui->lE_id->setValidator(validator);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_agregarEstudiante_clicked()
{
    crearReporte();
}

void MainWindow::crearArchivos() {
    QFile archivoEntrada("estudiantes.txt");

    if (!archivoEntrada.exists()) {
        if (archivoEntrada.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivoEntrada);
            out << "Andrea Maria Medina Quin,75\n";
            out << "Maria Isabel Sabillon Espinoza,55\n";
            out << "Carlos Gabriel Gomez Villatoro,80.78\n";
            out << "Ever Moises Maldonado Amador,88.34\n";
            out << "Luis Armando Paredes Fernandez,43.67\n";
            out << "Jaun Margarito Flores Alvarado,68.00\n";
            out << "Monica Elizabeth Cartagena Ruiz,59.6\n";
            out << "Javier Antonio Castro Prieto,59.4\n";
            archivoEntrada.close();
        } else {
            QMessageBox::warning(this, "Error", "No se pudo crear el archivo de nombres y notas.");
        }
    }

    QFile archivoSalida("reporteNotas.txt");

    if (!archivoSalida.exists()) {
        if (archivoSalida.open(QIODevice::WriteOnly | QIODevice::Text)) {
            archivoSalida.close();
        } else {
            QMessageBox::warning(this, "Error", "No se pudo crear el archivo de reporte.");
        }
    }
}

void MainWindow::cargarArchivo() {
    QFile archivo("estudiantes.txt");

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de nombres y notas.");
        return;
    }

    QTextStream in(&archivo);
    ui->textEdit->clear();

    while (!in.atEnd()) {
        QString linea = in.readLine();
        ui->textEdit->append(linea);
    }

    archivo.close();
}

void MainWindow::crearReporte() {
    QFile archivoEntrada("estudiantes.txt");
    QFile archivoSalida("reporteNotas.txt");

    if (!archivoEntrada.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de los estudiantes");
        return;
    }

    if (!archivoSalida.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo crear o abrir el archivo de reportes");
        return;
    }

    QTextStream in(&archivoEntrada);
    QTextStream out(&archivoSalida);
    ui->textEdit_2->clear();

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(",");
        if (partes.size() == 2) {
            QString nombre = partes.at(0).trimmed();
            double nota = partes.at(1).toDouble();
            QString status = (nota >= 59.5) ? "Aprobado" : "Desaprobado";

            QString reporteLinea = nombre + ": " + QString::number(nota) + " - " + status;
            out << reporteLinea << "\n";
            ui->textEdit_2->append(reporteLinea);
        }
    }

    archivoEntrada.close();
    archivoSalida.close();
}


//EJERCICIO 2

void MainWindow::inicializarArchivo() {
    QFile file(archivo);


    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Error", "No se pudo crear el archivo de productos");
            return;
        }
        file.close();
        ui->tE_productos->append("Archivo creado exitosamente");
    } else {
        ui->tE_productos->append("Archivo de productos ya existe");
    }
}

void MainWindow::guardarProducto(int codigo, const QString& nombre, double precio, int cantidad) {
    QFile file(archivo);

    if (!file.open(QIODevice::Append)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para guardar el producto");
        return;
    }

    QDataStream out(&file);
    out << codigo << nombre << precio << cantidad;
    file.close();

    QString info = QString("Código: %1\nNombre: %2\nPrecio: Lps. %3\nCantidad: %4\n\n")
                       .arg(codigo)
                       .arg(nombre)
                       .arg(precio)
                       .arg(cantidad);
    ui->tE_productos->append(info);
}

void MainWindow::actualizarProducto(int codigo, int nuevaCantidad) {
    QFile file(archivo);
    QFile tempFile("temp.dat");

    if (!file.open(QIODevice::ReadOnly) || !tempFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir los archivos");
        return;
    }

    QDataStream in(&file);
    QDataStream out(&tempFile);

    int fileCodigo;
    QString nombre;
    double precio;
    int cantidad;
    bool actualizado = false;

    while (!in.atEnd()) {
        in >> fileCodigo >> nombre >> precio >> cantidad;
        if (fileCodigo == codigo) {
            cantidad = nuevaCantidad;
            actualizado = true;
        }
        out << fileCodigo << nombre << precio << cantidad;
    }

    file.close();
    tempFile.close();

    QFile::remove(archivo);
    QFile::rename("temp.dat", archivo);

    if (actualizado) {
        mostrarProductoActualizado(codigo, nuevaCantidad);
    } else {
        ui->tE_productos->append("Código no encontrado");
    }
}

void MainWindow::mostrarProductos() {
    QFile file(archivo);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para mostrar los productos");
        return;
    }

    QDataStream in(&file);
    ui->tE_productos->clear();

    int codigo;
    QString nombre;
    double precio;
    int cantidad;

    QString info;
    while (!in.atEnd()) {
        in >> codigo >> nombre >> precio >> cantidad;
        info += QString("Código: %1\nNombre: %2\nPrecio: Lps. %3\nCantidad: %4\n\n")
                    .arg(codigo)
                    .arg(nombre)
                    .arg(precio)
                    .arg(cantidad);
    }

    file.close();
    ui->tE_productos->setText(info);
}

void MainWindow::mostrarProductoActualizado(int codigo, int nuevaCantidad) {
    QFile file(archivo);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo");
        return;
    }

    QDataStream in(&file);
    ui->tE_productos->clear();

    int fileCodigo;
    QString nombre;
    double precio;
    int cantidad;

    QString info;
    while (!in.atEnd()) {
        in >> fileCodigo >> nombre >> precio >> cantidad;
        if (fileCodigo == codigo) {
            cantidad = nuevaCantidad;
        }
        info += QString("Código: %1\nNombre: %2\nPrecio: Lps. %3\nCantidad: %4\n\n")
                    .arg(fileCodigo)
                    .arg(nombre)
                    .arg(precio)
                    .arg(cantidad);
    }

    file.close();
    ui->tE_productos->setText(info);
}

void MainWindow::on_btn_guardarProductos_clicked()
{
    bool ok;
    int codigo = ui->lE_codigo->text().toInt(&ok);
    QString nombre = ui->lE_nombreProducto->text().trimmed();
    double precio = ui->dSpinBox_precio->value();
    int cantidad = ui->spinBox_cantidad->value();

    if (!ok || nombre.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe de llenar todos los campos");
        return;
    }

    QFile file(archivo);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo");
        return;
    }

    QDataStream in(&file);

    int fileCodigo;
    QString fileNombre;
    double filePrecio;
    int fileCantidad;
    bool codigoExistente = false;

    while (!in.atEnd()) {
        in >> fileCodigo >> fileNombre >> filePrecio >> fileCantidad;
        if (fileCodigo == codigo) {
            codigoExistente = true;
            break;
        }
    }

    file.close();

    if (codigoExistente) {
        QMessageBox::information(this, "Código Existente", "El código ya existe");
    } else {
        guardarProducto(codigo, nombre, precio, cantidad);
    }

    ui->lE_codigo->clear();
    ui->lE_nombreProducto->clear();
    ui->dSpinBox_precio->setValue(0.00);
    ui->spinBox_cantidad->setValue(1);
}


void MainWindow::on_btn_modificar_clicked()
{
    bool ok;
    int codigo = QInputDialog::getInt(this, "Modificar Cantidad", "Ingrese el código del producto:", 0, 0, 1000000, 1, &ok);

    if (!ok) {
        QMessageBox::warning(this, "Error", "Código inválido.");
        return;
    }

    QFile file(archivo);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo");
        return;
    }

    QDataStream in(&file);

    int fileCodigo;
    QString nombre;
    double precio;
    int cantidad;
    bool encontrado = false;


    while (!in.atEnd()) {
        in >> fileCodigo >> nombre >> precio >> cantidad;
        if (fileCodigo == codigo) {
            encontrado = true;
            int nuevaCantidad = QInputDialog::getInt(this, "Modificar Cantidad", QString("Cantidad actual: %1\nIngrese la nueva cantidad:").arg(cantidad), cantidad, 0, 1000000, 1, &ok);
            if (ok) {
                actualizarProducto(codigo, nuevaCantidad);
            }
            break;
        }
    }

    file.close();

    if (!encontrado) {
        ui->tE_productos->append("Código no encontrado");
    }
}

//EJERCICIO 3
void MainWindow::on_btn_guardarDatos_clicked()
{
    QStringList empleado;
    int id = ui->lE_id->text().toInt();
    empleado << ui->lE_nombre->text()
             << QString::number(ui->spinBox_edad->value())
             << QString::number(ui->doubleSpinBox_salario->value())
             << QString::number(ui->spinBox_years->value())
             << QString::number(ui->spinBox_horasSemanal->value())
             << QString::number(ui->spinBox_rendimiento->value())
             << QString::number(id);

    if (verificarIdExistente(id)) {
        QMessageBox::warning(this, "Error", "El ID ya existe");
        return;
    }
    guardarDatosEnArchivo(empleado);
    datos.append(empleado);
    mostrarDatosEnTextEdit();
    realizarCalculos();
}

bool MainWindow::verificarIdExistente(int id)
{
    for (const QStringList &empleado : datos) {
        if (empleado[6].toInt() == id) {
            return true;
        }
    }
    return false;
}

void MainWindow::guardarDatosEnArchivo(const QStringList &empleado)
{
    QFile file("datos.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << empleado.join(",") << "\n";
        file.close();
    }
}

void MainWindow::mostrarDatosEnTextEdit()
{
    ui->te_sinCalcular->clear();
    for (const QStringList &empleado : datos) {
        ui->te_sinCalcular->append(empleado.join(", "));
    }
}

void MainWindow::realizarCalculos()
{
    QString empleadoMasHoras, empleadoMenosHoras;
    int conteoEdad45 = contarEdadMayorOIgualA(datos, 45);
    int conteoEdad60 = contarEdadMayorOIgualA(datos, 60);
    QString empleadoMasAntiguo, empleadoMenosAntiguo;
    double totalPagoSemanal = calcularTotalPagoSemanal(datos);

    calcularHorasExtremos(datos, empleadoMasHoras, empleadoMenosHoras);
    calcularAntiguedadExtremos(datos, empleadoMasAntiguo, empleadoMenosAntiguo);

    guardarCalculosEnBinario(empleadoMasHoras, empleadoMenosHoras, conteoEdad45, conteoEdad60,
                             empleadoMasAntiguo, empleadoMenosAntiguo, totalPagoSemanal);

    ui->te_calculos->clear();
    ui->te_calculos->append("Empleado con más horas trabajadas: " + empleadoMasHoras);
    ui->te_calculos->append("Empleado con menos horas trabajadas: " + empleadoMenosHoras);
    ui->te_calculos->append("Empleados de 45 años o más: " + QString::number(conteoEdad45));
    ui->te_calculos->append("Empleados de 60 años o más: " + QString::number(conteoEdad60));
    ui->te_calculos->append("Empleado con más antigüedad: " + empleadoMasAntiguo);
    ui->te_calculos->append("Empleado con menos antigüedad: " + empleadoMenosAntiguo);
    ui->te_calculos->append("Total pagado semanalmente: " + QString::number(totalPagoSemanal));
}

void MainWindow::calcularHorasExtremos(const QVector<QStringList> &datos, QString &empleadoMasHoras, QString &empleadoMenosHoras)
{
    if (datos.isEmpty()) return;

    int maxHoras = -1, minHoras = INT_MAX;
    for (const QStringList &empleado : datos) {
        int horas = empleado[4].toInt();
        if (horas > maxHoras) {
            maxHoras = horas;
            empleadoMasHoras = empleado[0];
        }
        if (horas < minHoras) {
            minHoras = horas;
            empleadoMenosHoras = empleado[0];
        }
    }
}

int MainWindow::contarEdadMayorOIgualA(const QVector<QStringList> &datos, int edadLimite)
{
    int conteo = 0;
    for (const QStringList &empleado : datos) {
        int edad = empleado[1].toInt();
        if (edad >= edadLimite) {
            conteo++;
        }
    }
    return conteo;
}

void MainWindow::calcularAntiguedadExtremos(const QVector<QStringList> &datos, QString &empleadoMasAntiguo, QString &empleadoMenosAntiguo)
{
    if (datos.isEmpty()) return;

    int maxAntiguedad = -1, minAntiguedad = INT_MAX;
    for (const QStringList &empleado : datos) {
        int antiguedad = empleado[3].toInt();
        if (antiguedad > maxAntiguedad) {
            maxAntiguedad = antiguedad;
            empleadoMasAntiguo = empleado[0];
        }
        if (antiguedad < minAntiguedad) {
            minAntiguedad = antiguedad;
            empleadoMenosAntiguo = empleado[0];
        }
    }
}

double MainWindow::calcularTotalPagoSemanal(const QVector<QStringList> &datos)
{
    double total = 0;
    for (const QStringList &empleado : datos) {
        double salario = empleado[2].toDouble();
        total += salario;
    }
    return total;
}

void MainWindow::guardarCalculosEnBinario(const QString &empleadoMasHoras, const QString &empleadoMenosHoras,
                                          int conteoEdad45, int conteoEdad60,
                                          const QString &empleadoMasAntiguo, const QString &empleadoMenosAntiguo,
                                          double totalPagoSemanal)
{
    QFile file("calculos.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << empleadoMasHoras << empleadoMenosHoras << conteoEdad45 << conteoEdad60
            << empleadoMasAntiguo << empleadoMenosAntiguo << totalPagoSemanal;
        file.close();
    }
}
