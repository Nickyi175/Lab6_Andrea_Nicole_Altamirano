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
    QVector<QStringList> datos;
    void guardarDatosEnArchivo(const QStringList &empleado);
    void mostrarDatosEnTextEdit();
    void realizarCalculos();
    bool verificarIdExistente(int id);
    void guardarCalculosEnBinario(const QString &empleadoMasHoras, const QString &empleadoMenosHoras,int conteoEdad45, int conteoEdad60,
                                  const QString &empleadoMasAntiguo, const QString &empleadoMenosAntiguo,double totalPagoSemanal);
    void calcularHorasExtremos(const QVector<QStringList> &datos, QString &empleadoMasHoras, QString &empleadoMenosHoras);
    int contarEdadMayorOIgualA(const QVector<QStringList> &datos, int edadLimite);
    void calcularAntiguedadExtremos(const QVector<QStringList> &datos, QString &empleadoMasAntiguo, QString &empleadoMenosAntiguo);
    double calcularTotalPagoSemanal(const QVector<QStringList> &datos);
};
#endif // MAINWINDOW_H
