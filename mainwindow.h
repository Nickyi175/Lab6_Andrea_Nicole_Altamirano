#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QIODevice>
#include <QVector>
#include <QPair>
#include <QStringList>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_agregarEstudiante_clicked();

    void on_btn_guardarProductos_clicked();

    void on_btn_modificar_clicked();

    void on_btn_guardarDatos_clicked();

    void on_btn_procesar_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QPair<QString, double>> estudiantes;
    void cargarArchivo();
    void crearReporte();
    void crearArchivos();
    void inicializarArchivo();
    void guardarProducto(int codigo, const QString& nombre, double precio, int cantidad);
    void actualizarProducto(int codigo, int nuevaCantidad);
    void mostrarProductos();
    void mostrarProductoActualizado(int codigo, int nuevaCantidad);
    const QString archivo = "productos.dat";
    void crearArchivoTexto(const QString& nombreArchivo);
    QVector<QStringList> leerArchivoTexto(const QString& nombreArchivo);
    double calcularPromedioSalario(const QVector<QStringList>& datos);
    int contarRegistrosEdadMayorA(const QVector<QStringList>& datos, int edadMinima);
    QVector<QString> obtenerNacionalidadesUnicas(const QVector<QStringList>& datos);
    QVector<QString> obtenerPuestosUnicos(const QVector<QStringList>& datos);
    int contarPorNacionalidad(const QVector<QStringList>& datos, const QString& nacionalidad);
    int contarPorPuesto(const QVector<QStringList>& datos, const QString& puesto);
    void mostrarDatos(const QVector<QStringList>& datos);
    void mostrarEstadisticas(double promedioSalario, int conteoEdad45, int conteoEdad60, const QVector<QString>& nacionalidades, const QVector<int>& conteosNacionalidad, const QVector<QString>& puestos, const QVector<int>& conteosPuesto);

};
#endif // MAINWINDOW_H
