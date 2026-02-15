#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "GenericModel.h"
#include "TraditionalTaskModel.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QVector<Task> tasks{
        {.Title = "Learn reflection", .Done = false},
        {.Title = "Build Qt app", .Done = true}
    };


    TaskModel reflectionModel{&tasks, &app};
    TraditionalTaskModel traditionalModel{tasks, &app};

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ReflectionModel", &reflectionModel);
    engine.rootContext()->setContextProperty("TraditionalModel", &traditionalModel);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QRangeModel", "Main");

    return app.exec();
}
