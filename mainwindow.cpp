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
    crearArchivoTexto("datos.txt");
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
        ui->tE_productos->append("Código no encontrado para actualización.");
    }
}

void MainWindow::crearArchivoTexto(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);

    if (!archivo.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo crear el archivo.");
        return;
    }

    QTextStream stream(&archivo);
    stream << "Nombre Edad Salario Nacionalidad Puesto Identidad\n";  // Encabezado
    archivo.close();

    ui->te_sinCalcular->setPlainText("Archivo de texto creado automáticamente.\n");
}

QVector<QStringList> MainWindow::leerArchivoTexto(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);
    QVector<QStringList> datos;

    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        return datos;
    }

    QTextStream stream(&archivo);
    QString linea = stream.readLine(); // Leer y descartar la cabecera
    while (!stream.atEnd()) {
        linea = stream.readLine();
        QStringList campos = linea.split(' ');
        datos.append(campos);
    }

    archivo.close();
    return datos;
}

double MainWindow::calcularPromedioSalario(const QVector<QStringList>& datos) {
    double suma = 0.0;
    int cantidad = 0;

    for (int i = 1; i < datos.size(); ++i) { // Empezamos en 1 para omitir la cabecera
        double salario = datos[i][2].toDouble();
        suma += salario;
        cantidad++;
    }

    return cantidad == 0 ? 0.0 : suma / cantidad;
}

int MainWindow::contarRegistrosEdadMayorA(const QVector<QStringList>& datos, int edadMinima) {
    int conteo = 0;

    for (int i = 1; i < datos.size(); ++i) {
        int edad = datos[i][1].toInt();
        if (edad >= edadMinima) {
            conteo++;
        }
    }

    return conteo;
}

QVector<QString> MainWindow::obtenerNacionalidadesUnicas(const QVector<QStringList>& datos) {
    QVector<QString> nacionalidades;
    for (int i = 1; i < datos.size(); ++i) {
        QString nacionalidad = datos[i][3];
        if (!nacionalidades.contains(nacionalidad)) {
            nacionalidades.append(nacionalidad);
        }
    }
    return nacionalidades;
}

QVector<QString> MainWindow::obtenerPuestosUnicos(const QVector<QStringList>& datos) {
    QVector<QString> puestos;
    for (int i = 1; i < datos.size(); ++i) {
        QString puesto = datos[i][4];
        if (!puestos.contains(puesto)) {
            puestos.append(puesto);
        }
    }
    return puestos;
}

int MainWindow::contarPorNacionalidad(const QVector<QStringList>& datos, const QString& nacionalidad) {
    int conteo = 0;

    for (int i = 1; i < datos.size(); ++i) {
        if (datos[i][3] == nacionalidad) {
            conteo++;
        }
    }

    return conteo;
}

int MainWindow::contarPorPuesto(const QVector<QStringList>& datos, const QString& puesto) {
    int conteo = 0;

    for (int i = 1; i < datos.size(); ++i) {
        if (datos[i][4] == puesto) {
            conteo++;
        }
    }

    return conteo;
}

void MainWindow::mostrarDatos(const QVector<QStringList>& datos) {
    QString contenido;
    for (const QStringList& linea : datos) {
        contenido += linea.join(' ') + "\n";
    }
    ui->te_sinCalcular->setPlainText(contenido);
}

void MainWindow::mostrarEstadisticas(double promedioSalario, int conteoEdad45, int conteoEdad60, const QVector<QString>& nacionalidades, const QVector<int>& conteosNacionalidad, const QVector<QString>& puestos, const QVector<int>& conteosPuesto) {
    QString contenido;
    contenido += QString("Promedio de Salario: %.2f\n").arg(promedioSalario);
    contenido += QString("Cantidad de personas mayores o iguales a 45 años: %1\n").arg(conteoEdad45);
    contenido += QString("Cantidad de personas mayores o iguales a 60 años: %1\n").arg(conteoEdad60);
    contenido += "Cantidad de personas por nacionalidad:\n";
    for (int i = 0; i < nacionalidades.size(); ++i) {
        contenido += QString("%1: %2\n").arg(nacionalidades[i]).arg(conteosNacionalidad[i]);
    }
    contenido += "Cantidad de personas por puesto laboral:\n";
    for (int i = 0; i < puestos.size(); ++i) {
        contenido += QString("%1: %2\n").arg(puestos[i]).arg(conteosPuesto[i]);
    }
    ui->te_calculos->setPlainText(contenido);
}
