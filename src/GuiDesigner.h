#pragma once
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTreeWidget>

class GuiDesigner : public QWidget {
    Q_OBJECT
public:
    explicit GuiDesigner(QWidget *parent = nullptr);

private slots:
    void addWidget(QString type);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTreeWidget *properties;
};
    QTreeWidgetItem *findPropertyItem(const QString &name);
    void updateProperty(const QString &name, const QString &value);
    void refreshProperties();
};